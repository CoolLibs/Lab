#include <Cool/Core/run.h>
#include <filesystem>
#include "App.h"

int main()
{
    Cool::run<Lab::App>({Cool::WindowConfig{
        .title                  = "CoolLab",
        .icon_path              = "windows-app-ressources/lab.png",
        .maximize_on_startup_if = true,
    }});
}