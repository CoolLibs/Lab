#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/App/WindowFactory.h>
#include <Cool/Time/Time.h>

#include "App.h"

int main() {
	// Init
	Cool::Log::Initialize();
	Cool::Time::Initialize();
	// Init Glfw and set OpenGL version to 4.3
	Cool::WindowFactory window_factory(4, 3);
	// Create the main window and init OpenGL
	Cool::Window& mainWindow = window_factory.create("Cool Lab", 1280, 720);
#ifdef NDEBUG
	glfwMaximizeWindow(mainWindow.get());
#endif
	// App
	App app(mainWindow);
	Cool::AppManager appManager(mainWindow, app);
	appManager.run();
}