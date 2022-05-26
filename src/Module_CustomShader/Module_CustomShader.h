#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include <range/v3/core.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/single.hpp>
#include "../Module.h"
#include "Dependencies/InputSlot.h"
#include "Dependencies/Registries.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    explicit Module_CustomShader(DirtyFlagFactory);

    void render(InputProvider provider, DirtyManager dirty_manager) override;
    void imgui_windows(Ui ui) override;

    // auto dependencies()
    // {
    //     return ranges::views::concat(
    //         ranges::single_view(AnyInputSlotRef(_testFloat)),
    //         ranges::single_view(AnyInputSlotRef(_color)),
    //         ranges::single_view(AnyInputSlotRef(_color2)));
    // }

    // auto dependencies() const
    // {
    //     return ranges::views::view(_parameters);
    // }

    auto depends_on(reg::AnyId id) const -> bool override
    {
        return true;
        // return std::ranges::any_of(_parameters, [&id](auto&& dep) {
        //     return std::visit([&id](auto&& dep) { return dep.id == id; }, dep);
        // });
    }

    auto is_dirty(DirtyManager dirty_manager) const -> bool override
    {
        return _is_dirty || dirty_manager.is_dirty(_shader_is_dirty);
    }

private:
    void refresh_pipeline_if_necessary(InputProvider provider, DirtyManager dirty_manager);
    void compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name);
    void parse_shader_for_params(std::string_view fragment_shader_source_code);

private:
    Cool::FullscreenPipeline  _fullscreen_pipeline{};
    std::vector<AnyInputSlot> _parameters;
    DirtyFlag                 _shader_is_dirty; // Must be before _file because it is used to construct it
    InputSlot_File            _file;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::base_class<Module>(this),
                _parameters,
                _shader_is_dirty,
                _file);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);