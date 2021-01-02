#include <Cool/Log/Log.h>
#include <Cool/App/AppManager.h>
#include <Cool/Time/Time.h>
#include <Cool/Icons/Icons.h>

#include "App.h"

int main() {
	Cool::Log::Initialize();
	Cool::Time::Initialize();
	Cool::AppManager appManager("is0");
	Cool::Icons::_LoadAll();
	App app;
	int exitCode = appManager.run(app);
	Cool::Icons::_DestroyAll();
	return exitCode;
}