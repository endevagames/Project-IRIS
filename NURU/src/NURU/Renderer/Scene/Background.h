#pragma once

#include "SceneNode.h"

namespace NURU 
{
    class TextureCube;
    class Material;
    class Shader;
    class Renderer;
    class Cube;

    /* 

      A SkyBox represented as a scene node for easy scene management. The Background Scene Node
      is set up in such a way that when passed to the renderer it'll automatically render behind
      all visible geometry (with no camera parallax).

    */
    class Background : public SceneNode
    {
    private:
        TextureCube *m_CubeMap;
        Shader      *m_Shader;

    public:
        Background();
        ~Background();

        void SetCubemap(TextureCube* cubemap);
    };
}