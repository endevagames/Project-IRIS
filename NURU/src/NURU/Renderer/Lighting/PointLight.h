#pragma once

#include "Math/Linear/Vector.h"

#include "Shading/Material.h"
#include "Mesh/Sphere.h"

namespace NURU
{
    /*

      Light container object for any 3D point light source. Point lights range are solely 
      determined by a radius value which is used for their frustum culling and attenuation
      properties. Attenuation is calculated based on a slightly tweaked point light attenuation
      equation derived by Epic Games (for use in UE4).

    */
    class PointLight
    {
    public:
        Vec3 Position   = Vec3(0.0f);
        Vec3 Color      = Vec3(1.0f);
        float      Intensity  = 1.0f;
        float      Radius     = 1.0f;
        bool       Visible    = true;
        bool       RenderMesh = false;
    };
}