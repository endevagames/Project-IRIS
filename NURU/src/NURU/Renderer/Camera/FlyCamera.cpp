#include "NURUPCH.h"

#include "FlyCamera.h"
#include "Math/NURUMath.h"

#include <algorithm>

namespace NURU
{
    // --------------------------------------------------------------------------------------------
    FlyCamera::FlyCamera(Vec3 position, Vec3 forward, Vec3 up) : Camera(position, forward, up)
    {
        Yaw = -90.0f;

        Forward = forward;
        m_WorldUp = Up;
        m_TargetPosition = position;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::Update(float dt)
    {
		Camera::Update(dt);
		// slowly interpolate to target position each frame given some damping factor.
		// this gives smooth camera movement that fades out the closer we are to our target.
		Position = NURU::Lerp(Position, m_TargetPosition, NURU::Clamp01(dt * Damping));
		Yaw      = NURU::Lerp(Yaw, m_TargetYaw, NURU::Clamp01(dt * Damping * 2.0f));
		Pitch    = NURU::Lerp(Pitch, m_TargetPitch, NURU::Clamp01(dt * Damping * 2.0f));

		// calculate new cartesian basis vectors from yaw/pitch pair:
		Vec3 newForward;
		newForward.x = cos(0.0174533 * Pitch) * cos(0.0174533 * Yaw);
		newForward.y = sin(0.0174533 * Pitch);
		newForward.z = cos(0.0174533 * Pitch) * sin(0.0174533 * Yaw);
		Forward = NURU::Normalize(newForward);
		Right = NURU::Normalize(NURU::Cross(Forward, m_WorldUp));
		Up = NURU::Cross(Right, Forward);

		// calculate the new view matrix
		UpdateView();
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputKey(float dt, CAMERA_MOVEMENT direction)
    {
        float speed = MovementSpeed * dt;
        if (direction      == CAMERA_FORWARD)
            m_TargetPosition = m_TargetPosition + Forward*speed;
        else if (direction == CAMERA_BACK)
            m_TargetPosition = m_TargetPosition - Forward*speed;
        else if (direction == CAMERA_LEFT)
            m_TargetPosition = m_TargetPosition - Right*speed;
        else if (direction == CAMERA_RIGHT)
            m_TargetPosition = m_TargetPosition + Right*speed;
        else if (direction == CAMERA_UP)
            m_TargetPosition = m_TargetPosition + m_WorldUp*speed;
        else if (direction == CAMERA_DOWN)
            m_TargetPosition = m_TargetPosition - m_WorldUp*speed;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputMouse(float deltaX, float deltaY)
    {
        float xmovement = deltaX * MouseSensitivty;
        float ymovement = deltaY * MouseSensitivty;

        m_TargetYaw   += xmovement;
        m_TargetPitch += ymovement;

        // prevents calculating the length of the null vector
        if(m_TargetYaw == 0.0f) m_TargetYaw = 0.01f;
        if(m_TargetPitch == 0.0f) m_TargetPitch = 0.01f;

        // it's not allowed to move the pitch above or below 90 degrees asctime the current 
        // world-up vector would break our LookAt calculation.
        if (m_TargetPitch > 89.0f)  m_TargetPitch =  89.0f;
        if (m_TargetPitch < -89.0f) m_TargetPitch = -89.0f;
    }
    // --------------------------------------------------------------------------------------------
    void FlyCamera::InputScroll(float deltaX, float deltaY)
    {
        MovementSpeed = NURU::Clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f); 
        Damping       = NURU::Clamp(Damping       + deltaX * 0.5f, 1.0f, 25.0f);
    }
}