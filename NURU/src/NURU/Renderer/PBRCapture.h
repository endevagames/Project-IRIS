#pragma once

#include "Shading/TextureCube.h"
#include "Math/Linear/Vector.h"

namespace NURU
{
    /* 

      Container object for holding all per-environment specific pre-computed PBR data.

    */
    struct PBRCapture
    {
        TextureCube* Irradiance  = nullptr;
        TextureCube* Prefiltered = nullptr;

        Vec3 Position;
        float      Radius;
    };
}