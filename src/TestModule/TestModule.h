#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include "../InputSlot.h"
#include "../Module.h"
#include "../Registries.h"

namespace Lab {

class TestModule : public Module {
public:
    TestModule() = default;
    explicit TestModule(std::string_view name);

    void render(InputProvider provider) override;
    void imgui_windows(Ui ui) override;

    auto depends_on(reg::AnyId id) const -> bool override
    {
        return _color.id == id;
    }

private:
    Cool::FullscreenPipeline _fullscreen_pipeline{
        Cool::File::to_string(Cool::Path::root() + "/shader-examples/monochrome.frag"),
        "Test Module's Shader"};
    InputSlot<glm::vec3> _color;
    std::string          _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Module>(this), _color, _name);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::TestModule);