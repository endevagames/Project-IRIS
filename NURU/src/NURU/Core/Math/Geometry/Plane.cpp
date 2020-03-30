#include "NURUPCH.h"

#include "Plane.h"
#include "NURU/Core/Math/Linear/Operation.h"

namespace NURU
{
    Plane::Plane(const float a, const float b, const float c, const float d)
    {
        Normal.x = a;
        Normal.y = b;
        Normal.z = c;
        Distance = d;
    }

    Plane::Plane(const Vec3& normal, const Vec3& point)
    {
        Normal   = normal;
        Distance = -Dot(normal, point);
    }
}