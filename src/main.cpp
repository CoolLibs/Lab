#include "App.h"
//
#include <Cool/Core/run.h> // Must be included last otherwise it slows down compilation because it includes <cereal/archives/json.hpp>

auto main() -> int
{
    Cool::run<Lab::App>(
        {Cool::WindowConfig{
            .title                  = "Coollab",
            .maximize_on_startup_if = true,
        }},
        Cool::InitConfig{
            .default_texture_path = Cool::compute_root_path() / "res/images/logo.png",
        }
    );
}