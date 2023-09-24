#include "App.h"
#include "CommandLineArgs/CommandLineArgs.h"
#include "Cool/Path/Path.h"
#include "Cool/Path/PathsConfig.h"
//
#include <Cool/Core/run.h> // Must be included last otherwise it slows down compilation because it includes <cereal/archives/json.hpp>

class PathsConfig : public Cool::PathsConfig {
public:
    PathsConfig()
        : Cool::PathsConfig{"Coollab"}
    {}

    [[nodiscard]] auto default_texture() const -> std::filesystem::path override
    {
        return Cool::Path::root() / "res/images/logo.png";
    }
};

auto main(int argc, char** argv) -> int
{
    Lab::command_line_args().init(argc, argv);
    Cool::Path::initialize<PathsConfig>();

    Cool::run<Lab::App>({
        .windows_configs   = {Cool::WindowConfig{
              .title                  = "", // This is set when we load a project.
              .maximize_on_startup_if = true,
        }},
        .imgui_ini_version = 0,
    });
}