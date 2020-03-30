#pragma once

#include "Core.h"

#include "Window.h"
#include "NURU/Core/LayerStack.h"
#include "NURU/Events/ApplicationEvent.h"
#include "NURU/Events/KeyEvent.h"
#include "NURU/Renderer/Renderer.h"
#include "NURU/ImGui/ImGuiLayer.h"

namespace NURU {

    class Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Renderer> m_Renderer;
        bool m_Running = true;
    public:
        Application(/* args */);
        virtual ~Application();

        void OnEvent(Event& e);
        void Run();

        inline Window& GetWindow() { return *m_Window; }
        inline void Stop() { m_Running = false; }
        inline static Application& Get() { return *s_Instance; }

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        static Application* s_Instance;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    };

	//Defined in client
	Application* CreateApplication();

}