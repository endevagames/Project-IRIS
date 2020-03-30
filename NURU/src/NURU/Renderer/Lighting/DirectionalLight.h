#ifndef CELL_LIGHTING_DIRECTIONAL_LIGHT_H
#define CELL_LIGHTING_DIRECTIONAL_LIGHT_H

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

namespace NURU
{
    class RenderTarget;

    /*

      Light container object for any 3D directional light source. Directional light types support
      shadow casting, holding a reference to the RenderTarget (and the relevant light space view 
      projection matrix) used for its shadow map generation.

    */
    class DirectionalLight
    {
    public:
        Vec3 Direction = Vec3(0.0f);
        Vec3 Color     = Vec3(1.0f);
        float Intensity      = 1.0f;

        bool CastShadows = true;
        RenderTarget* ShadowMapRT = NULL;
        Mat4    LightSpaceViewProjection;
    };
}

#endif