#pragma once

#include "Math/Linear/Vector.h"
#include "Math/Linear/Operation.h"

namespace NURU
{
    class Camera;

    /* 
        
      Data container for the 3D plane equation variables in Cartesian space. A plane equation can be
      defined by its Normal (perpendicular to the plane) and a distance value D obtained from any
      point on the plane itself (projected onto the normal vector).

    */
    struct FrustumPlane
    {
        Vec3 Normal;
        float      D;

        void SetNormalD(Vec3 normal, Vec3 point)
        {
            Normal = NURU::Normalize(normal);
            D      = -NURU::Dot(Normal, point);
        }

        float Distance(Vec3 point)
        {
            return NURU::Dot(Normal, point) + D;
        }
    };


    /*

      Container object managing all 6 camera frustum planes as calculated from any Camera object.

      The CameraFrustum allows to quickly determine (using simple collision primitives like point,
      sphere, box) whether an object is within the frustum (or crossing the frustum's edge(s)). 
      This gives us the option to significantly reduce draw calls for objects that aren't visible
      anyways. Note that the frustum needs to be re-calculated every frame.

    */
    class CameraFrustum
    {
    public:
        union
        {
            FrustumPlane Planes[6];
            struct
            {
                FrustumPlane Left;
                FrustumPlane Right;
                FrustumPlane Top;
                FrustumPlane Bottom;
                FrustumPlane Near;
                FrustumPlane Far;
            };
        };

    public:
        CameraFrustum() { } // NOTE(Joey): why do I need to define a constructor here? (otherwise I get deleted constructor error) LOOK IT UP!

        void Update(Camera* camera);

        bool Intersect(Vec3 point);
        bool Intersect(Vec3 point, float radius);
        bool Intersect(Vec3 boxMin, Vec3 boxMax);
    };
}