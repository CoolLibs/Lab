#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include <range/v3/core.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/single.hpp>
#include "Dependencies/Dirty.h"
#include "Dependencies/InputSlot.h"
#include "Dependencies/Registries.h"
#include "Module.h"

namespace Lab {

class TestModule : public Module {
public:
    TestModule() = default;
    TestModule(std::string_view name, DirtyFlagFactory dirty_flag_factory);

    void render(RenderParams) override;
    void imgui_windows(Ui ui) override;

    auto dependencies()
    {
        return ranges::views::concat(
            ranges::single_view(AnyInputSlotRef(_testFloat)),
            ranges::single_view(AnyInputSlotRef(_color)),
            ranges::single_view(AnyInputSlotRef(_color2)));
    }

    auto dependencies() const
    {
        return ranges::views::concat(
            ranges::single_view(AnyInputSlotRefToConst(_testFloat)),
            ranges::single_view(AnyInputSlotRefToConst(_color)),
            ranges::single_view(AnyInputSlotRefToConst(_color2)));
    }

    auto depends_on(reg::AnyId id) const -> bool override
    {
        return std::ranges::any_of(dependencies(), [&id](auto&& dep) {
            return std::visit([&id](auto&& dep) { return dep.get().id == id; }, dep);
        });
    }

private:
    Cool::FullscreenPipeline _fullscreen_pipeline{
        Cool::File::to_string(Cool::Path::root() + "/shader-examples/test_module.frag"),
        "Test Module's Shader"};
    InputSlot<glm::vec3> _color{dirty_flag(), "Color"};
    InputSlot<glm::vec3> _color2{dirty_flag(), "Color2"};
    InputSlot<float>     _testFloat{dirty_flag(), "Radius"};
    std::string          _name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Module>(this),
                _color,
                _color2,
                _testFloat,
                _name);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::TestModule);