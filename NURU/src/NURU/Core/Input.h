#pragma once

#include <NURUPCH.h>

#include "NURU/Core/Core.h"

#include "NURU/Core/Keycodes.h"
#include "NURU/Core/MouseCodes.h"

#include "NURU/Events/Event.h"
#include "NURU/Events/KeyEvent.h"
#include "NURU/Events/MouseEvent.h"

namespace NURU {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;

    private:
        static Input* s_Instance;
    };
};