#include "NURUPCH.h"

#include "Plane.h" 

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    Plane :: Plane(unsigned int xSegments, unsigned int ySegments, unsigned int size)
    {
        bool oddRow = false;

        float dX = (float)size  / xSegments;
        float dY = (float)size / ySegments;

        for (int y = 0; y <= ySegments; ++y)
        {
            for (int x = 0; x <= xSegments; ++x)
            {
                Positions.push_back(Vec3(dX * x * 2.0f - 1.0f, 0.0f, dY * y * 2.0f - 1.0f));
                UV.push_back(Vec2(dX * x, 1.0f - y * dY));
                Normals.push_back(Vec3::UP);
            }
        }

        for (int y = 0; y < ySegments; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= xSegments; ++x)
                {
                    Indices.push_back(y       * (xSegments + 1) + x);
                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                }
            }
            else
            {
                for (int x = xSegments; x >= 0; --x)
                {
                    Indices.push_back((y + 1) * (xSegments + 1) + x);
                    Indices.push_back(y       * (xSegments + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        Topology = TRIANGLE_STRIP;
        Finalize();
    }
}