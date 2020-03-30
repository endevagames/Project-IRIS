#pragma once

#include "NURU/Core/Math/Linear/Vector.h"

namespace NURU
{
	/* NOTE(Joey):

	  Plane as deducted by the generalized plane equation.
	  Only defined in 3-dimensional space.

	*/
	struct Plane
	{
		Vec3 Normal;
		float Distance;

		Plane(const float a, const float b, const float c, const float d);
		Plane(const Vec3& normal, const Vec3& point);
	};
} // namespace NURU