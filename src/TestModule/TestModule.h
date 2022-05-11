#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include "../Module.h"
#include "../Registries.h"

namespace Lab {

class TestModule : public Module {
public:
    TestModule() = default;
    TestModule(std::string_view name, Registries& registries);

    void render(const Registries& registries) override;
    void imgui_windows(Ui ui) override;

private:
    Cool::FullscreenPipeline _fullscreen_pipeline{
        Cool::File::to_string(Cool::Path::root() + "/shader-examples/monochrome.frag"),
        "Test Module's Shader"};
    reg::Id<glm::vec3> _colorId;
    std::string        _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Module>(this), _colorId, _name);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::TestModule);