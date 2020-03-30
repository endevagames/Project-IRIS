#pragma once

void EnterGameLoop()
{
	NURU::Log::Init();

	NURU_CORE_WARN("PROJECT NURU INITIALISING");

	auto app = NURU::CreateApplication();
	app->Run();
}

#ifdef NURU_PLATFORM_WINDOWS

extern NURU::Application* CreateApplication();

int main(int argc, char** argv) {
	
	EnterGameLoop();
	return 0;

}

int WINAPI WinMain( HINSTANCE hInstance,    // HANDLE TO AN INSTANCE.  This is the "handle" to YOUR PROGRAM ITSELF.
                    HINSTANCE hPrevInstance,// USELESS on modern windows (totally ignore hPrevInstance)
                    LPSTR szCmdLine,        // Command line arguments.  similar to argv in standard C programs
                    int iCmdShow ) {

	EnterGameLoop();
	return 0;

}

#endif