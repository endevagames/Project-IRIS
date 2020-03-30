#pragma once

#include "NURU/Core/Math/Linear/Vector.h"

namespace NURU
{
    /* NOTE(Joey): 

      Polar coordinates w/ relevant conversion for
      converting to and from cartesian coordinates.

    */
    struct Polar
    {
        float r;     // NOTE(Joey): distance coordinate from origin.
        float theta; // NOTE(Joey): counter-clockwise rotation from x-axis.

        Polar(float r, float theta);
        // NOTE(Joey): convert from cartesian.
        Polar(Vec2 cartesian); 

        // NOTE(Joey): convert to cartesian.
        Vec2 ToCartesian();
    };
}