#pragma once
#include <Cool/Input/KeyboardEvent.h>
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>
#include <stringify/stringify.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Dependencies/InputFactory_Ref.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Dependencies/History.h"
#include "Ui.h"
#include "UpdateContext_Ref.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Module {
public:
    struct RenderParams {
        Cool::InputProvider_Ref   provider;
        Cool::InputFactory_Ref    input_factory;
        Cool::InputDestructor_Ref input_destructor;
        Cool::IsDirty_Ref         is_dirty;
        Cool::SetClean_Ref        set_clean;
    };

    Module() = default;
    Module(std::string_view name, Cool::DirtyFlagFactory_Ref dirty_flag_factory)
        : _name{name}
        , _dirty_flag{dirty_flag_factory.make()}
    {
    }

    virtual ~Module() = default;

    auto name() const -> const std::string& { return _name; }

    void do_rendering(RenderParams params, UpdateContext_Ref update_ctx)
    {
        render(params, update_ctx);
        params.set_clean(_dirty_flag);
    }
    virtual void imgui_windows(Ui_Ref ui) const = 0; /// The ui() method should be const, because it sould only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level). If you really need to mutate one of your member variables, mark it as `mutable`.
    virtual void update(UpdateContext_Ref){};

    virtual auto all_inputs() const -> Cool::AllInputRefsToConst = 0;

    virtual auto is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
    {
        return check_dirty(_dirty_flag);
    };

    auto dirty_flag() { return _dirty_flag; }

private:
    virtual void render(RenderParams, UpdateContext_Ref) = 0;

private:
    std::string     _name;
    Cool::DirtyFlag _dirty_flag;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Dirty Flag", _dirty_flag)
        );
    }
};

} // namespace Lab