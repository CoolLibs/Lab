#pragma once
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Path/Path.h>
#include "Dependencies/InputSlot.h"
#include "Dependencies/Module.h"

namespace Lab {

class Module_CustomShader : public Module {
public:
    Module_CustomShader() = default;
    explicit Module_CustomShader(DirtyFlagFactory);

    void render(RenderParams) override;
    void imgui_windows(Ui ui) override;

    auto all_input_slots()
    {
        return _parameters;
    }

    auto all_input_slots() const -> AllInputSlots override
    {
        AllInputSlots slots;
        slots.push_back(AnyInputSlotRefToConst{_camera_slot});
        for (const auto& x : _parameters) {
            slots.push_back(
                std::visit([](auto&& x) { return AnyInputSlotRefToConst{x}; }, x));
        }
        return slots;
    }

    auto is_dirty(DirtyManager dirty_manager) const -> bool override
    {
        return Module::is_dirty(dirty_manager) ||
               dirty_manager.is_dirty(_shader_is_dirty);
    }

private:
    void refresh_pipeline_if_necessary(InputProvider provider, DirtyManager dirty_manager, InputSlotDestructorRef input_slot_destructor);
    void compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name);
    void parse_shader_for_params(std::string_view fragment_shader_source_code, InputSlotDestructorRef input_slot_destructor);

private:
    Cool::FullscreenPipeline  _fullscreen_pipeline{};
    std::vector<AnyInputSlot> _parameters;
    InputSlot<Cool::Camera>   _camera_slot;
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
                _camera_slot,
                _shader_is_dirty,
                _file);
    }
};

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Module_CustomShader);