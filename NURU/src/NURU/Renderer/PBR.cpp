#include "NURUPCH.h"

#include "PBR.h"

#include "Math/Linear/Operation.h"

#include "Renderer.h"
#include "RenderTarget.h"
#include "PBRCapture.h"

#include "Resources/Resources.h"
#include "Scene/Scene.h"
#include "Scene/SceneNode.h"

#include "Mesh/Cube.h"
#include "Mesh/Sphere.h"

#include "Camera/Camera.h"

#include "Shading/Material.h"
#include "Shading/Shader.h"
#include "Shading/TextureCube.h"

#include <vector>

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    PBR::PBR(Renderer* renderer)
    {
        m_Renderer = renderer;
        
        m_RenderTargetBRDFLUT = new RenderTarget(128, 128, GL_HALF_FLOAT, 1, true);
        Shader *hdrToCubemap      = Resources::LoadShader("pbr:hdr_to_cubemap", "shaders/pbr/cube_sample.vs", "shaders/pbr/spherical_to_cube.fs");
        Shader *irradianceCapture = Resources::LoadShader("pbr:irradiance", "shaders/pbr/cube_sample.vs", "shaders/pbr/irradiance_capture.fs");
        Shader *prefilterCapture  = Resources::LoadShader("pbr:prefilter", "shaders/pbr/cube_sample.vs", "shaders/pbr/prefilter_capture.fs");
        Shader *integrateBrdf     = Resources::LoadShader("pbr:integrate_brdf", "shaders/screen_quad.vs", "shaders/pbr/integrate_brdf.fs");
        m_PBRHdrToCubemap      = new Material(hdrToCubemap);
        m_PBRIrradianceCapture = new Material(irradianceCapture);
        m_PBRPrefilterCapture  = new Material(prefilterCapture);
        m_PBRIntegrateBRDF     = new Material(integrateBrdf);
        m_PBRHdrToCubemap->DepthCompare      = GL_LEQUAL;
        m_PBRIrradianceCapture->DepthCompare = GL_LEQUAL;
        m_PBRPrefilterCapture->DepthCompare  = GL_LEQUAL;
        m_PBRHdrToCubemap->Cull = false;
        m_PBRIrradianceCapture->Cull = false;
        m_PBRPrefilterCapture->Cull = false;

        m_PBRCaptureCube = new Cube();
        m_SceneEnvCube = new SceneNode(0);
        m_SceneEnvCube->Mesh = m_PBRCaptureCube;
        m_SceneEnvCube->Material = m_PBRHdrToCubemap;

        // - brdf integration
        m_Renderer->Blit(nullptr, m_RenderTargetBRDFLUT, m_PBRIntegrateBRDF);

        // capture
        m_ProbeCaptureShader = Resources::LoadShader("pbr:capture", "shaders/capture.vs", "shaders/capture.fs");
        m_ProbeCaptureShader->Use();
        m_ProbeCaptureShader->SetInt("TexAlbedo", 0);
        m_ProbeCaptureShader->SetInt("TexNormal", 1);
        m_ProbeCaptureShader->SetInt("TexMetallic", 2);
        m_ProbeCaptureShader->SetInt("TexRoughness", 3);
        m_ProbeCaptureBackgroundShader = Resources::LoadShader("pbr:capture background", "shaders/capture_background.vs", "shaders/capture_background.fs");
        m_ProbeCaptureBackgroundShader->Use();
        m_ProbeCaptureBackgroundShader->SetInt("background", 0);

        // debug render
        m_ProbeDebugSphere = new Sphere(32, 32);
        m_ProbeDebugShader = Resources::LoadShader("pbr:probe_render", "shaders/pbr/probe_render.vs", "shaders/pbr/probe_render.fs");
        m_ProbeDebugShader->Use();
        m_ProbeDebugShader->SetInt("PrefilterMap", 0);
    }
    // --------------------------------------------------------------------------------------------
    PBR::~PBR()
    {
        delete m_PBRCaptureCube;
        delete m_SceneEnvCube;
        delete m_RenderTargetBRDFLUT;
        delete m_PBRHdrToCubemap;
        delete m_PBRIrradianceCapture;
        delete m_PBRPrefilterCapture;
        delete m_PBRIntegrateBRDF;
        delete m_SkyCapture;
        for (int i = 0; i < m_CaptureProbes.size(); ++i)
        {
            delete m_CaptureProbes[i]->Irradiance;
            delete m_CaptureProbes[i]->Prefiltered;
            delete m_CaptureProbes[i];
        }
        delete m_ProbeDebugSphere;
    }
    // --------------------------------------------------------------------------------------------
    void PBR::SetSkyCapture(PBRCapture* capture)
    {
        m_SkyCapture = capture;
    }
    // --------------------------------------------------------------------------------------------
    void PBR::AddIrradianceProbe(PBRCapture* capture, Vec3 position, float radius)
    {
        capture->Position = position;
        capture->Radius = radius;
        m_CaptureProbes.push_back(capture);
    }
    // --------------------------------------------------------------------------------------------
    void PBR::ClearIrradianceProbes()
    {
        for (int i = 0; i < m_CaptureProbes.size(); ++i)
        {
            delete m_CaptureProbes[i]->Irradiance;
            delete m_CaptureProbes[i]->Prefiltered;
            delete m_CaptureProbes[i];
        }
        m_CaptureProbes.clear();
    }
    // --------------------------------------------------------------------------------------------
    PBRCapture* PBR::ProcessEquirectangular(Texture* envMap)
    {
        // convert HDR radiance image to HDR environment cubemap
        m_SceneEnvCube->Material = m_PBRHdrToCubemap;
        m_PBRHdrToCubemap->SetTexture("environment", envMap, 0);
        TextureCube hdrEnvMap;
        hdrEnvMap.DefaultInitialize(128, 128, GL_RGB, GL_FLOAT);
        m_Renderer->RenderToCubemap(m_SceneEnvCube, &hdrEnvMap);

        return ProcessCube(&hdrEnvMap);
    }
    // --------------------------------------------------------------------------------------------
    PBRCapture* PBR::ProcessCube(TextureCube* capture, bool prefilter)
    {
        PBRCapture* captureProbe = new PBRCapture;

        // irradiance
        captureProbe->Irradiance = new TextureCube;
        captureProbe->Irradiance->DefaultInitialize(32, 32, GL_RGB, GL_FLOAT);
        m_PBRIrradianceCapture->SetTextureCube("environment", capture, 0);
        m_SceneEnvCube->Material = m_PBRIrradianceCapture;
        m_Renderer->RenderToCubemap(m_SceneEnvCube, captureProbe->Irradiance, Vec3(0.0f), 0);
        // prefilter 
        if (prefilter)
        {
            captureProbe->Prefiltered = new TextureCube;;
            captureProbe->Prefiltered->FilterMin = GL_LINEAR_MIPMAP_LINEAR;
            captureProbe->Prefiltered->DefaultInitialize(128, 128, GL_RGB, GL_FLOAT, true);
            m_PBRPrefilterCapture->SetTextureCube("environment", capture, 0);
            m_SceneEnvCube->Material = m_PBRPrefilterCapture;
            // calculate prefilter for multiple roughness levels
            unsigned int maxMipLevels = 5;
            for (unsigned int i = 0; i < maxMipLevels; ++i)
            {
                m_PBRPrefilterCapture->SetFloat("roughness", (float)i / (float)(maxMipLevels - 1));
                m_Renderer->RenderToCubemap(m_SceneEnvCube, captureProbe->Prefiltered, Vec3(0.0f), i);

            }
        }
        return captureProbe;
    }
    // --------------------------------------------------------------------------------------------
    PBRCapture* PBR::GetSkyCapture()
    {
        return m_SkyCapture;
    }
    // --------------------------------------------------------------------------------------------
    std::vector<PBRCapture*> PBR::GetIrradianceProbes(Vec3 queryPos, float queryRadius)
    {
        // retrieve all irradiance probes in proximity to queryPos and queryRadius
        std::vector<PBRCapture*> capturesProximity;
        for (int i = 0; i < m_CaptureProbes.size(); ++i)
        {
            float lengthSq = NURU::LengthSquared(m_CaptureProbes[i]->Position - queryPos);
            if (lengthSq < queryRadius*queryRadius)
            {
                m_CaptureProbes.push_back(m_CaptureProbes[i]);
            }
        }
        // fall back to sky capture if no irradiance probe was found
        if (!capturesProximity.size() == 0)
        {
            capturesProximity.push_back(m_SkyCapture);
        }
        return capturesProximity;
    }
    // --------------------------------------------------------------------------------------------
    void PBR::RenderProbes()
    {
        m_ProbeDebugShader->Use();
        m_ProbeDebugShader->SetMatrix("projection", m_Renderer->GetCamera()->Projection);
        m_ProbeDebugShader->SetMatrix("view", m_Renderer->GetCamera()->View);
        m_ProbeDebugShader->SetVector("CamPos", m_Renderer->GetCamera()->Position);

        // first render the sky capture
        m_ProbeDebugShader->SetVector("Position", Vec3(0.0f, 2.0, 0.0f));
        m_SkyCapture->Prefiltered->Bind(0);
        m_Renderer->RenderMesh(m_ProbeDebugSphere, m_ProbeDebugShader);

        // then do the same for each capture probe (at their respective location)
        for (int i = 0; i < m_CaptureProbes.size(); ++i)
        {
            m_ProbeDebugShader->SetVector("Position", m_CaptureProbes[i]->Position);
            if (m_CaptureProbes[i]->Prefiltered)
            {
                m_CaptureProbes[i]->Prefiltered->Bind(0);
            }
            else
            {
                m_CaptureProbes[i]->Irradiance->Bind(0);
            }
            m_Renderer->RenderMesh(m_ProbeDebugSphere, m_ProbeDebugShader);
        }
    }
}
