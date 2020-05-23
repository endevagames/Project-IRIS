#include <NURUPCH.h>
#include <stb_image.h>

#include "Platforms/Windows/WindowsWindow.h"

#include "NURU/Events/ApplicationEvent.h"
#include "NURU/Events/KeyEvent.h"
#include "NURU/Events/MouseEvent.h"

namespace NURU 
{
    
    static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		NURU_CORE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}

    Window* Window :: Create(const WindowProps& props) 
	{
		return new WindowsWindow(props);
    }

    WindowsWindow :: WindowsWindow(const WindowProps& props) 
	{
		Init(props);
    }

    WindowsWindow :: ~WindowsWindow() 
	{
		Shutdown();
    }

    void WindowsWindow :: Init(const WindowProps& props) 
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		NURU_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) 
		{
			int success = glfwInit();
			NURU_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_RESIZABLE, false);
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		GLFWimage images[1]; images[0].pixels = stbi_load("data/icon.png", &images[0].width, &images[0].height, 0, 4);
		glfwSetWindowIcon(m_Window, 1, images);
		stbi_image_free(images[0].pixels);
		SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch(action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch(action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float) xOffset, (float)yOffset);
			data.EventCallback(event);
		});


		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
    }

    void WindowsWindow :: Shutdown() 
	{
		glfwDestroyWindow(m_Window);
    }

    void WindowsWindow :: OnUpdate() 
	{
		if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
    }

    void WindowsWindow :: SetVSync(bool enabled) 
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
    }

    bool WindowsWindow :: IsVSync() const 
	{
		return m_Data.VSync;
    }

	void WindowsWindow :: Maximise() const 
	{
		glfwMaximizeWindow(m_Window);
	}

	inline void* WindowsWindow :: GetNativeWindow() const 
	{
		return m_Window;
	}

	double WindowsWindow :: UpdateDeltaTime()
	{
        double currentFrameTime = glfwGetTime();
        DeltaTime     = currentFrameTime - LastFrameTime;
        LastFrameTime = currentFrameTime;
		return DeltaTime;
	}
}
