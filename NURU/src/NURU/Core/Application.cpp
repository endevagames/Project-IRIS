#include <NURUPCH.h>

#include "NURU/Core/Application.h"
#include "NURU/Core/Input.h"
#include "NURU/Renderer/Renderer.h"
#include "Math/NURUMath.h"

#define IVIEW(x) Do(#x, x);

void Do(const char* xStr, int& x) {
    NURU_CORE_INFO(xStr);
    ++x;
}

namespace NURU 
{

    Application* Application::s_Instance = nullptr;

    Application :: Application(/* args */) 
    {
        NURU_CORE_ASSERT(s_Instance, "Application already exists!");
        
        s_Instance = this;
	    m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(NURU_BIND_EVENT_FN(Application::OnEvent));

        m_Renderer = std::unique_ptr<Renderer>(new Renderer());
        m_ImGuiLayer = new ImGuiLayer();
        
        PushOverlay(m_ImGuiLayer);

        if( m_Renderer == NULL ) 
                m_Running = false;
    }

    Application :: ~Application() 
    {
    }

    void Application :: PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application :: PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application :: OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowResizeEvent>(NURU_BIND_EVENT_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowCloseEvent>(NURU_BIND_EVENT_FN(Application::OnWindowClose));

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if(e.Handled)
                break;
        }
    }

    void Application :: Run() 
    {
        while ( m_Running ) 
        {            
            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
                for(Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
	    }
        
    }

    bool Application :: OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;    
    }

    bool Application :: OnWindowResize(WindowResizeEvent& e)
    {
        double viewPortParams[4];
        glGetDoublev(GL_VIEWPORT, viewPortParams);
        // m_Renderer->OnResize((int)viewPortParams[0], (int)viewPortParams[1], e.GetWidth(), e.GetHeight());
        return true;
    }
}
