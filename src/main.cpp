#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/App/WindowFactory.h>
#include <Cool/Time/Time.h>
#include <Cool/File/File.h>

#include "App.h"

void main() {
	// Init
	Cool::Log::initialize();
#ifndef NDEBUG
	// When launching from an IDE (which almost always means we are in Debug mode)
	// the initial current_path will be set to bin/Debug
	// but I prefer to have it at the root of the project, so that the assets we load
	// are the ones at the root of the project, not the ones that have been copied to the bin folders.
	// This means that if for example we do hot reloading of shaders
	// we can modify the source assets (at the root), instead of the ones that have been copied.
	// This is important because only the ones at the root are tracked in Git.
	// Also, if you modify an asset at the root whithout modifying some code, and then restart the program
	// the asset won't be copied (because I couldn't figure out how to configure CMake to do so)
	// So it is better to always work with the assets at the root while in development.
	auto current_path = std::filesystem::current_path();
	std::filesystem::current_path(current_path.parent_path().parent_path());
#endif
	Cool::File::initialize_root_dir(std::filesystem::current_path().string());
	Cool::Time::initialize();
	// Init Glfw and set OpenGL version
#ifndef __APPLE__
	Cool::WindowFactory window_factory(4, 3);
#else
	Cool::WindowFactory window_factory(3, 3); // OpenGL > 3.3 is not supported on MacOS
#endif
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