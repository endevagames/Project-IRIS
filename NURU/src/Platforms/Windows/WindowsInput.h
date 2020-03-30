#pragma once

#include <NURUPCH.h>
#include <NURU/Core/Input.h>

namespace NURU {
    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(KeyCode keycode) override;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
    };
}