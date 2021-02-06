#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/App/OpenGLWindowingSystem.h>
#include <Cool/Time/Time.h>
#include <Cool/Icons/Icons.h>

#include "App.h"

int main() {
	// Init
	Cool::Log::Initialize();
	Cool::Time::Initialize();
	// Init Glfw and set OpenGL version to 4.3
	Cool::OpenGLWindowingSystem windowingSystem(4, 3);
	// Create the main window and init OpenGL
	Cool::OpenGLWindow mainWindow = windowingSystem.createWindow("is0", 1280, 720);
#ifdef NDEBUG
	glfwMaximizeWindow(mainWindow.get());
#endif
	// Load Icons
	Cool::Icons::_LoadAll();
	// App
	App app(mainWindow);
	Cool::AppManager appManager(mainWindow, app);
	appManager.run();
}