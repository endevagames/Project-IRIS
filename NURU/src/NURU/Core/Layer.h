#pragma once

#include <NURUPCH.h>

#include "NURU/Core/Core.h"
#include "NURU/Events/Event.h"

using std::string;

namespace NURU
{

    class Layer
    {
    public:
        Layer(const string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const string& GetName() const { return m_DebugName; } 

    protected:
        string m_DebugName;  
    };
}