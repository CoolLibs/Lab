#pragma once
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>
#include <stringify/stringify.hpp>
#include "History.h"
#include "InputProvider.h"
#include "InputSlot.h"
#include "Ui.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// The ui() method should be const, because it sould only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level)
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Module {
public:
    struct RenderParams {
        InputProvider          provider;
        InputSlotDestructorRef input_slot_destructor;
        DirtyManager           dirty_manager;
    };

    Module() = default;
    explicit Module(DirtyFlagFactory dirty_flag_factory)
        : _dirty_flag{dirty_flag_factory.create()}
    {
    }

    virtual ~Module() = default;

    void do_rendering(RenderParams params)
    {
        render(params);
        params.dirty_manager.set_clean(_dirty_flag);
    }
    virtual void imgui_windows(Ui ui) = 0;
    virtual void update(){};

    virtual auto all_input_slots() const -> AllInputSlots = 0;

    virtual auto is_dirty(DirtyManager dirty_manager) const -> bool // No need for the whole DirtyManager, just DirtyChecker
    {
        return dirty_manager.is_dirty(_dirty_flag);
    };

    auto dirty_flag() { return _dirty_flag; }

protected:
    virtual void render(RenderParams) = 0;

private:
    DirtyFlag _dirty_flag;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Dirty Flag", _dirty_flag));
    }
};

} // namespace Lab