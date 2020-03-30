#include <NURUPCH.h>

#include <NURU.h>

class ExampleLayer : public NURU::Layer
{
public:
    ExampleLayer() 
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        if(NURU::Input::IsKeyPressed(NURU_KEY_ESCAPE))
            NURU::Application::Get().Stop();
    }

    void OnEvent(NURU::Event& e) override
    {

    }
};

class Sandbox : public NURU::Application 
{
private:
    /* data */
public:
    Sandbox(/* args */) {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {
    }
};


NURU::Application* NURU::CreateApplication() 
{
	return new Sandbox();
}
