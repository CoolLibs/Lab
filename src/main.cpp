#include "App.h"
#include "CommandLineArgs/CommandLineArgs.h"
//
#include <Cool/Core/run.h> // Must be included last otherwise it slows down compilation because it includes <cereal/archives/json.hpp>

auto main(int argc, char** argv) -> int
{
    Lab::command_line_args().init(argc, argv);
    Cool::run<Lab::App>(
        {Cool::WindowConfig{
            .title                  = "Coollab [Unsaved project]",
            .maximize_on_startup_if = true,
        }},
        Cool::InitConfig{
            .default_texture_path = Cool::compute_root_path() / "res/images/logo.png",
        }
    );
}