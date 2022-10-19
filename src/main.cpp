#include <Cool/Core/run.h>
#include "App.h"

int main()
{
    int a;
    Cool::run<Lab::App>({Cool::WindowConfig{
        .title                  = "CoolLab",
        .maximize_on_startup_if = true,
    }});
}