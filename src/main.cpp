#include "App.h"
//
#include <Cool/Core/run.h> // Must be included last otherwise it slows down compilation

auto main() -> int
{
    Cool::run<Lab::App>({Cool::WindowConfig{
        .title                  = "CoolLab",
        .maximize_on_startup_if = true,
    }});
}