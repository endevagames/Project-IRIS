#include "NURUPCH.h"

#include "Camera.h"

#include "Math/Trigonometry/Conversions.h"
#include "Math/NURUMath.h"

#include <math.h>

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    Camera::Camera()
    {
    }
    // --------------------------------------------------------------------------------------------
    Camera::Camera(Vec3 position, Vec3 forward, Vec3 up) : Position(position), Forward(forward), Up(up)
    {
        UpdateView();
    }
    // --------------------------------------------------------------------------------------------
    void Camera::Update(float dt)
    {
        Frustum.Update(this);
    }
    // --------------------------------------------------------------------------------------------
    void Camera::SetPerspective(float fov, float aspect, float fnear, float ffar)
    {
        Perspective = true;
		Projection = NURU::Perspective(fov, aspect, fnear, ffar);
        FOV    = fov;
        Aspect = aspect;
        Near   = fnear;
        Far    = ffar;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::SetOrthographic(float left, float right, float top, float bottom, float fnear, float ffar)
    {
        Perspective = false;
        Projection = Orthographic<float>(left, right, top, bottom, fnear, ffar);
        Near = fnear;
        Far  = ffar;
    }
    // --------------------------------------------------------------------------------------------
    void Camera::UpdateView()
    {
        View = NURU::LookAt(Position, Position + Forward, Up);
    }
    // --------------------------------------------------------------------------------------------
    float Camera::FrustumHeightAtDistance(float distance)
    {
        if (Perspective)
        {
             return 2.0f * distance * tanf(Deg2Rad(FOV * 0.5));
        }
        else
        {
            return Frustum.Top.D;
        }
    }
    // --------------------------------------------------------------------------------------------
    float Camera::DistanceAtFrustumHeight(float frustumHeight)
    {
        if (Perspective)
        {
            return frustumHeight * 0.5f / tanf(NURU::Deg2Rad(FOV * 0.5f));
        }
        else
        {
            return Frustum.Near.D;
        }
    }

};