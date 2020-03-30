#pragma once

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

namespace NURU
{
    class Mesh;
    class Material;

    /* 

      All render state required for submitting a render command.

    */
    struct RenderCommand
    {
        Mat4 Transform;
        Mat4 PrevTransform;
        Mesh*      Mesh;
        Material*  Material;
        Vec3 BoxMin;
        Vec3 BoxMax;
    };
}