#pragma once

#include "NURU/Core/Layer.h"
#include "NURU/Core/Input.h"

namespace NURU
{
    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
        void OnEvent(Event& e) override;
    private:
        float m_Time = 0.0f;
    };
}