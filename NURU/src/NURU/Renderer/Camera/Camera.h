#pragma once

#include "Math/Linear/Vector.h"
#include "Math/Linear/Matrix.h"

#include "CameraFrustum.h"

namespace NURU
{
    // defines several possible options for camera movement. Used as abstraction to stay away from 
    // window-system specific input methods.
    enum CAMERA_MOVEMENT {
        CAMERA_FORWARD,
        CAMERA_BACK,
        CAMERA_LEFT,
        CAMERA_RIGHT,
        CAMERA_UP,
        CAMERA_DOWN,
    };

    /* 

      Basic root camera. Only does relevant camera calculations with manual forced direction 
      setters. This camera should only be used in code and not respond to user input; the
      derived cameras are for user/player interaction.

    */
    // TODO(Joey): do we want the camera to be a SceneNode as well so we can attach it to other entities?
    // what about FPS cameras then? 
    class Camera
    {
    public:
        Mat4 Projection;
        Mat4 View;

        Vec3 Position = Vec3(0.0f, 0.0f,  0.0f);
        Vec3 Forward  = Vec3(0.0f, 0.0f, -1.0f);
        Vec3 Up       = Vec3(0.0f, 1.0f,  0.0f);
        Vec3 Right    = Vec3(1.0f, 0.0f,  0.0f);

        float FOV;
        float Aspect;
        float Near;
        float Far;
        bool  Perspective;

        CameraFrustum Frustum;
    private:
    public:
        Camera(); 
        Camera(Vec3 position, Vec3 forward, Vec3 up);

        void Update(float dt);
        
        void SetPerspective(float fov, float aspect, float near, float far);
        void SetOrthographic(float left, float right, float top, float bottom, float near, float far);

        void UpdateView();

        float FrustumHeightAtDistance(float distance);
        float DistanceAtFrustumHeight(float frustumHeight);

    };
}