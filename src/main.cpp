#include <Cool/Core/run.h>
#include <Cool/Default/DefaultInit.h>
#include "App.h"

int main()
{
    CoolDefault::init();
    Cool::run<App>({Cool::WindowConfig{.title                  = "Cool Lab",
                                       .maximize_on_startup_if = Cool::is_building_as(Cool::BuildMode::Release)}});
}