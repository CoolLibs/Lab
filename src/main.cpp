#include <Cool/Core/main.h>
#include "App.h"

int main()
{
    Cool::default_init();
    Cool::run<App>({Cool::WindowConfig{.title = "Cool Lab"}});
}