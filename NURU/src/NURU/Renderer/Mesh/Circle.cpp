#include "NURUPCH.h"

#include "Circle.h"
#include "Math/Linear/Vector.h"

#include <math.h>

namespace NURU
{
	const float TAU = 6.28318530717f;
    // --------------------------------------------------------------------------------------------
    Circle::Circle(unsigned int edgeSegments, unsigned int ringSegments)
    {
        for (unsigned int y = 0; y <= ringSegments; ++y)
        {
            for (unsigned int x = 0; x <= edgeSegments; ++x)
            {
                float xSegment = (float)x / (float)edgeSegments;
                float ringDepth = (float)y / (float)ringSegments;
                float xPos = std::cos(xSegment * TAU); // TAU is 2PI
                float yPos = std::sin(xSegment * TAU);

                Positions.push_back(Vec3(xPos * ringDepth, yPos * ringDepth, 0.0f));
            }
        }
        // indices are exactly the same as for the plane, only the positions differ for a circle
        bool oddRow = false;
        for (int y = 0; y < ringSegments; ++y)
        {
            if (!oddRow) // NOTE(Joey): even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= edgeSegments; ++x)
                {
                    Indices.push_back(y       * (edgeSegments + 1) + x);
                    Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                }
            }
            else
            {
                for (int x = edgeSegments; x >= 0; --x)
                {
                    Indices.push_back((y + 1) * (edgeSegments + 1) + x);
                    Indices.push_back(y       * (edgeSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        Topology = TRIANGLE_STRIP;
        Finalize();
    }
}