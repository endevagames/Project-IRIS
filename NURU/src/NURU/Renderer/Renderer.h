#pragma once

#include "NURU/Core/Core.h"

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

#include "Mesh/Quad.h"

#include "CommandBuffer.h"
#include "PBRCapture.h"
#include "GLCache.h"

#include <Glad/glad.h>

namespace NURU
{
    /* 

      Forward declare the types, s.t. we don't need to include the required header files here. 
      Seeing as several objects throughout Cell will link to renderer.h we want to reduce as much 
      unnecesary additional header code as much as possible (saves in compilation times). 

    */
    class Mesh;
    class Material;
    class Scene;
    class SceneNode;
    class Camera;
    class RenderTarget;
    class MaterialLibrary;
    class PBR;
    class PostProcessor;
    class Application;

    /*

      The main renderer; responsible for maintaining a render buffer queue, providing the front 
      push commands for filling the buffer, sorting the buffer, manage multiple render passes and 
      render the buffer(s) accordingly.

    */
    class Renderer
    {
        friend PostProcessor;
        friend PBR;
        friend Application;
    public:
        // configuration
        bool IrradianceGI = true;
        bool Shadows      = true;
        bool Lights       = true;
        bool RenderLights = true;
        bool LightVolumes = false;
        bool RenderProbes = false;
        bool Wireframe    = false;
    private:       
        // render state
        CommandBuffer* m_CommandBuffer;
        GLCache        m_GLCache;
        Vec2           m_RenderSize;

        // lighting
        List<DirectionalLight*> m_DirectionalLights;
        List<PointLight*>       m_PointLights;
        RenderTarget* m_GBuffer = nullptr;
        Mesh*         m_DeferredPointMesh;

        // materials
        MaterialLibrary* m_MaterialLibrary;

        // camera
        Camera*    m_Camera;
        Mat4 m_PrevViewProjection;

        // render-targets/post
        List<RenderTarget*> m_RenderTargetsCustom;
        RenderTarget*              m_CurrentRenderTargetCustom = nullptr;
        RenderTarget*              m_CustomTarget;
        RenderTarget*              m_PostProcessTarget1;
        PostProcessor*             m_PostProcessor;
        Quad*                      m_NDCPlane;
        unsigned int m_FramebufferCubemap; 
        unsigned int m_CubemapDepthRBO;

        // shadow buffers
        List<RenderTarget*> m_ShadowRenderTargets;
        List<Mat4>    m_ShadowViewProjections;
       
        // pbr
        PBR* m_PBR;
        unsigned int m_PBREnvironmentIndex;
        List<Vec4> m_ProbeSpatials;

        // ubo
        unsigned int m_GlobalUBO;

        // debug
        Mesh* m_DebugLightMesh;

    public:
        Renderer();
        ~Renderer();

        void Init();

        void SetRenderSize(unsigned int width, unsigned int height);
        Vec2 GetRenderSize();

        void SetTarget(RenderTarget* renderTarget, GLenum target = GL_TEXTURE_2D);

        Camera* GetCamera();
        void    SetCamera(Camera* camera);

        PostProcessor* GetPostProcessor();

        // create either a deferred default material (based on default set of materials available (like glass)), or a custom material (with custom you have to supply your own shader)
        Material* CreateMaterial(std::string base = "default"); // these don't have the custom flag set (default material has default state and uses checkerboard texture as albedo (and black metallic, half roughness, purple normal, white ao)
        Material* CreateCustomMaterial(Shader* shader);         // these have the custom flag set (will be rendered in forward pass)
        Material* CreatePostProcessingMaterial(Shader* shader); // these have the post-processing flag set (will be rendered after deferred/forward pass)

        void PushRender(Mesh* mesh, Material* material, Mat4 transform = Mat4(), Mat4 prevFrameTransform = Mat4());
        void PushRender(SceneNode* node);
        void PushPostProcessor(Material* postProcessor);

        void AddLight(DirectionalLight *light);
        void AddLight(PointLight       *light);        

        void RenderPushedCommands();

        void Blit(Texture* src, RenderTarget* dst = nullptr, Material* material = nullptr, std::string textureUniformName = "TexSrc");

        // pbr
        void        SetSkyCapture(PBRCapture* pbrEnvironment);
        PBRCapture* GetSkypCature();
        void        AddIrradianceProbe(Vec3 position, float radius);
        void        BakeProbes(SceneNode* scene = nullptr);
    private:
        // renderer-specific logic for rendering a custom (forward-pass) command
        void RenderCustomCommand(RenderCommand* command, Camera* customCamera, bool updateGLSettings = true);
        // renderer-specific logic for rendering a list of commands to a target cubemap
        void RenderToCubemap(SceneNode* scene, TextureCube* target, Vec3 position = Vec3(0.0f), unsigned int mipLevel = 0);
        void RenderToCubemap(List<RenderCommand>& renderCommands, TextureCube* target, Vec3 position = Vec3(0.0f), unsigned int mipLevel = 0);
        // minimal render logic to render a mesh 
        void RenderMesh(Mesh* mesh, Shader* shader);
        // updates the global uniform buffer objects
        void UpdateGlobalUBOs();
        // returns the currently active render target
        RenderTarget* GetCurrentRenderTarget();

        // deferred logic:
        // renders all ambient lighting (including indirect IBL)
        void RenderDeferredAmbient();
        // render directional light
        void RenderDeferredDirLight(DirectionalLight* light);
        // render point light
        void RenderDeferredPointLight(PointLight* light);

        // render mesh for shadow buffer generation
        void RenderShadowCastCommand(RenderCommand* command, const Mat4& projection, const Mat4& view);

        // Frame Rendering
        void NewFrame();
        void EndFrame();
    };
}