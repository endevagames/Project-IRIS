#pragma once

#include <NURU/Core/Window.h>
#include <GLFW/glfw3.h>

namespace NURU {

    class WindowsWindow : public Window {
    public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		
		void OnUpdate() override;

		virtual double UpdateDeltaTime() override;
		virtual inline double GetDeltaTime() override { return DeltaTime; };

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		// Mouse attributes
		inline void SetCursorEnabled(bool enabled) { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void Maximise() const override;

		inline void* GetNativeWindow() const override;

    private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
		
    private:
		double LastFrameTime = 0.0f;
		double DeltaTime = 0.0f;

		GLFWwindow* m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
    };
}
