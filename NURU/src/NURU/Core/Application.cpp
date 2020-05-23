#include <NURUPCH.h>

#include "Math/NURUMath.h"

#include "Application.h"
#include "Input.h"

#include "NURU/ImGui/ImGuiLayer.h"

#include "NURU/Renderer/Camera/FlyCamera.h"
#include "NURU/Renderer/Renderer.h"
#include "NURU/Renderer/Resources/Resources.h"
#include "NURU/Renderer/Scene/Background.h"
#include "NURU/Renderer/Scene/Scene.h"
#include "NURU/Renderer/Mesh/Terrain.h"
#include "NURU/Renderer/Mesh/Cube.h"
#include "NURU/Renderer/Resources/Resources.h"

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
        m_Renderer->SetRenderSize(1600, 900);

        m_Camera = new FlyCamera(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        m_Camera->SetPerspective(Deg2Rad(60.0f), m_Renderer->GetRenderSize().x / m_Renderer->GetRenderSize().y, 0.1f, 100.0f);

        m_Renderer->SetCamera(m_Camera);

        DirectionalLight* dirLight = new DirectionalLight();
        dirLight->Direction = Vec3(-0.2f, -1.0f, -0.25f);
        dirLight->Color = Vec3(0.8f, 0.6f, 1.0f);
        dirLight->Intensity = 100.0f;
        m_Renderer->AddLight(dirLight);

        m_ImGuiLayer = new ImGuiLayer();
        
        PushOverlay(m_ImGuiLayer);

        if (m_Renderer == NULL) 
            m_Running = false;

        Background* background = new Background;
        PBRCapture *pbrEnv = m_Renderer->GetSkypCature();
        background->SetCubemap(pbrEnv->Prefiltered);
	    float lodLevel = 1.5f; 
	    background->Material->SetFloat("lodLevel", lodLevel);
        
        Material * m = m_Renderer->CreateCustomMaterial(Resources::LoadShader("terrain", "shaders/deferred/terrain.vs", "shaders/deferred/terrain.fs"));
        m->Type = MATERIAL_DEFAULT;
        m->SetTexture("TexAlbedo", Resources::LoadTexture("terrain albedo", "C:/Users/Sammi3/_gamedev/_projects/__IRIS/DemoScene/TerrainGreenHill/BaseColor.png", GL_TEXTURE_2D, GL_RGB), 3);
        m->SetTexture("TexNormal", Resources::LoadTexture("terrain normal", "C:/Users/Sammi3/_gamedev/_projects/__IRIS/DemoScene/TerrainGreenHill/BaseNormal.png"), 4);
        m->SetTexture("TexMetallic", Resources::LoadTexture("terrain metallic", "C:/Users/Sammi3/_gamedev/_projects/__IRIS/DemoScene/TerrainGreenHill/BaseMetallic.png"), 5);
        m->SetTexture("TexRoughness", Resources::LoadTexture("terrain roughness", "C:/Users/Sammi3/_gamedev/_projects/__IRIS/DemoScene/TerrainGreenHill/BaseRoughness.png"), 6);
        
        auto terrainMesh = new Terrain();
        auto terrain = Scene::MakeSceneNode(terrainMesh, m);
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
        while (m_Running) 
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

            if (Input::IsKeyPressed(Key::LeftControl))
                m_Window->ToggleCursorEnabled();

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

            // Render a new frame
            m_Renderer->NewFrame();
            m_Renderer->PushRender(Scene::Root);
            m_Renderer->EndFrame();

            m_ImGuiLayer->Begin();
                for(Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            m_ImGuiLayer->End();

            // Update window, swap back and front buffers
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