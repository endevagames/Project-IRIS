#include <NURUPCH.h>

#include "NURU/Core/Application.h"
#include "NURU/Core/Input.h"

#include "Math/NURUMath.h"

#include "NURU/Renderer/Camera/FlyCamera.h"
#include "NURU/Renderer/Renderer.h"
#include "NURU/Renderer/Resources/Resources.h"
#include "NURU/Renderer/Scene/Scene.h"
#include "NURU/Renderer/Scene/SceneNode.h"

#define IVIEW(x) Do(#x, x);

void Do(const char* xStr, int& x) {
    NURU_CORE_INFO("{0} : {1}", xStr, x);
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
        m_Window->SetCursorEnabled(false);

        m_Renderer = new Renderer();
        m_Renderer->Init();
        m_Renderer->SetRenderSize(1920, 1080);

        m_Camera = new FlyCamera(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        m_Camera->SetPerspective(Deg2Rad(60.0f), m_Renderer->GetRenderSize().x / m_Renderer->GetRenderSize().y, 0.1f, 100.0f);
        m_Renderer->SetCamera(m_Camera);

        DirectionalLight* dirLight = new DirectionalLight();
        dirLight->Direction = Vec3(0.2f, -1.0f, 0.25f);
        dirLight->Color = NURU::Vec3(1.0f, 0.89f, 0.7f);
        dirLight->Intensity = 50.0f;
        m_Renderer->AddLight(dirLight);

        m_ImGuiLayer = new ImGuiLayer();
        
        PushOverlay(m_ImGuiLayer);

        if (m_Renderer == NULL) 
                m_Running = false;

        SceneNode* sponza = Resources::LoadMesh(m_Renderer, "sponza", "meshes/sponza/sponza.obj");
        sponza->SetPosition(NURU::Vec3(0.0, -1.0, 0.0));
        sponza->SetScale(0.01f);
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
            auto dt = (float)m_Window->UpdateDeltaTime();

            if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
                m_Camera->InputKey(dt, CAMERA_FORWARD);
            if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
                m_Camera->InputKey(dt, CAMERA_BACK);
            if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
                m_Camera->InputKey(dt, CAMERA_LEFT);
            if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
                m_Camera->InputKey(dt, CAMERA_RIGHT);
            if (Input::IsKeyPressed(Key::E))
                m_Camera->InputKey(dt, CAMERA_UP);
            if (Input::IsKeyPressed(Key::Q))
                m_Camera->InputKey(dt, CAMERA_DOWN);

            auto [mouseX, mouseY] = Input::GetMousePosition();
            auto [xOffset, yOffset] = [](float xpos, float ypos) {
                static bool firstMouse = true;
                static float lastX = 960.0f;
                static float lastY = 520.0f;

                if (firstMouse)
                {
                    lastX = xpos;
                    lastY = ypos;
                    firstMouse = false;
                }

                float xoffset = xpos - lastX;
                float yoffset = lastY - ypos;

                lastX = xpos;
                lastY = ypos;
                
                return std::pair<float, float>(xoffset, yoffset);

            }(mouseX, mouseY);
            
            m_Camera->InputMouse(xOffset, yOffset);

            m_Camera->Update(dt);

            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
                for(Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Renderer->NewFrame();

            m_Renderer->PushRender(Scene::Root);

            m_Renderer->EndFrame();

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
        return true;
    }
}