#include <Cool/Core/run.h>
#include "App.h"

int main()
{
    Cool::run<Lab::App>({Cool::WindowConfig{
        .title                  = "Cool Lab",
        .maximize_on_startup_if = true,
    }});
}