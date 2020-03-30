#include "NURUPCH.h"

#include "Background.h"

#include "Scene.h"
#include "Renderer.h"
#include "Resources/Resources.h"

#include "Shading/Material.h"
#include "Shading/Shader.h"
#include "Shading/TextureCube.h"

#include "Mesh/Cube.h"

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    Background::Background() : SceneNode(Scene::CounterID++)
    {
        Scene::Root->AddChild(this);

        m_Shader  = Resources::LoadShader("background", "shaders/background.vs", "shaders/background.fs");
        Material  = new NURU::Material(m_Shader);
        Mesh      = new Cube;
        BoxMin    = Vec3(-99999.0);
        BoxMax    = Vec3( 99999.0);

        // default material configuration
        Material->SetFloat("Exposure", 1.0f);
        Material->DepthCompare = GL_LEQUAL;
        Material->Cull = false;
        Material->ShadowCast = false;
        Material->ShadowReceive = false;
    }
    // --------------------------------------------------------------------------------------------
    Background::~Background()
    {

    }
    // --------------------------------------------------------------------------------------------
    void Background::SetCubemap(TextureCube* cubemap)
    {
        m_CubeMap = cubemap;
        Material->SetTextureCube("background", m_CubeMap, 0);
    }
};