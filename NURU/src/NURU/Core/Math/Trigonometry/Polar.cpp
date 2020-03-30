#include "NURUPCH.h"

#include "Polar.h"

#include <math.h>

#include "NURU/Core/Math/Linear/Operation.h"

namespace NURU
{
    Polar::Polar(float r, float theta)
    {
        this->r     = r;
        this->theta = theta;
    }

    Polar::Polar(Vec2 cartesian)
    {
        r     = Length(cartesian); // NOTE(Joey): length of vec2(x, y).
        theta = atan2(cartesian.y, cartesian.x);
    }

    Vec2 Polar::ToCartesian()
    {
        return Vec2(r * cos(theta), r * sin(theta));
    }
}