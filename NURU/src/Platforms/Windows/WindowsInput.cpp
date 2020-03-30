#include <NURUPCH.h>

#include <Platforms/Windows/WindowsInput.h>
#include <GLFW/glfw3.h>
#include <NURU/Core/Application.h>

namespace NURU {

    Input* Input :: s_Instance = new WindowsInput();

    bool WindowsInput :: IsKeyPressedImpl(KeyCode keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(keycode));

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    
    bool WindowsInput :: IsMouseButtonPressedImpl(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));

        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput :: GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float)xPos, (float)yPos };
    }
}