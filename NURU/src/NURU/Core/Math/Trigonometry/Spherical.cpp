#include "NURUPCH.h"

#include <math.h>

#include "Spherical.h"
#include "NURU/Core/Math/Linear/Operation.h"

namespace NURU
{
	// NOTE(Nabil/htmlboss): using an initializer list is faster than doing: this->rho = Rho, etc.
    Spherical::Spherical(const float Rho, const float Phi, const float Theta) : rho(Rho), phi(Phi), theta(Theta) 
	{

    }

    Spherical::Spherical(const Vec3& cartesian)
    {
        rho   = Length(cartesian);
        phi   = atan2(Length(cartesian.xy), cartesian.z);
        theta = atan2(cartesian.y, cartesian.x);
    }

    Vec3 Spherical::ToCartesian() const
    {
        return Vec3(
            rho * sin(phi) * cos(theta), // NOTE(Joey); project phi onto xy plane and then polar coordinate conversion (r*cos(theta))
            rho * sin(phi) * sin(theta), // NOTE(Joey); project phi onto xy plane and then polar coordinate conversion (r*sin(theta))
            rho * cos(phi)             
        );
    }

} // namespace NURU