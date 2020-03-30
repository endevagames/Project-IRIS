#ifndef MATH_TRIGONOMETRY_SPHERICAL_H
#define MATH_TRIGONOMETRY_SPHERICAL_H

#include "NURU/Core/Math/Linear/Vector.h"

namespace NURU
{
    /* NOTE(Joey):

      Spherical coordinates
    
    */
    struct Spherical
    {
        float rho;   // NOTE(Joey): distance coordinate from origin.
        float phi;   // NOTE(Joey): counter-clockwise rotation from xy plane to z (z being up-axis in conventional math).
        float theta; // NOTE(Joey): counter-clockwise rotation from x axis on xy plane.

        Spherical(const float rho, const float phi, const float theta);
        // NOTE(Joey): convert from cartesian.
        Spherical(const Vec3& cartesian); 

        // NOTE(Joey): convert to cartesian.
        Vec3 ToCartesian() const;
    };
} // namespace NURU

#endif