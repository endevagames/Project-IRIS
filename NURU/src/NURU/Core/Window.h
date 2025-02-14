#pragma once

#include <NURUPCH.h>

#include <NURU/Core/Core.h>
#include <NURU/Events/Event.h>

namespace NURU {
    
    struct WindowProps {
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "NURU Editor",
		    unsigned int width = 1600,
		    unsigned int height = 900)
	    : Title(title), Width(width), Height(height)
	    {
	    }
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;
        virtual ~Window() {};
        virtual void  OnUpdate() = 0;
        
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual double UpdateDeltaTime() = 0;
        virtual double GetDeltaTime() = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual void Maximise() const = 0;

        // Mouse attributes
        virtual void SetCursorEnabled(bool enabled) = 0;
        virtual void ToggleCursorEnabled() = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
	
    };
}
