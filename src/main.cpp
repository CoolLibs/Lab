#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>

#include "App.h"

#include "Clock/Clock_Realtime.h"
#include "Framework/Time.h"
#include "Constants/Textures.h"

int main() {
	Cool::Log::Initialize();
	Clock_Realtime::Initialize();
	Time::Initialize();
	Cool::AppManager appManager("is0");
	Textures::_LoadAll();
	App app;
	int exitCode = appManager.run(app);
	Textures::_DestroyAll();
	return exitCode;
}