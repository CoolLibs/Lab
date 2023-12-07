#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <img/src/Size.h>
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>
#include <stringify/stringify.hpp>
#include "Dependencies/History.h"
#include "Dependencies/Ui.h"
#include "Dependencies/UpdateContext_Ref.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Module {
public:
    struct RenderParams {
        Cool::InputProvider_Ref                          provider;
        Cool::InputFactory_Ref                           input_factory;
        Cool::IsDirty_Ref                                is_dirty;
        Cool::SetClean_Ref                               set_clean;
        std::reference_wrapper<Cool::VariableRegistries> variable_registries;
        img::Size                                        render_target_size;
    };

    Module()                                 = default;
    Module(Module const&)                    = delete;
    auto operator=(Module const&) -> Module& = delete;

protected:
    Module(Module&&) noexcept                    = default;
    auto operator=(Module&&) noexcept -> Module& = default;

public:
    virtual ~Module() = default;

    Module(std::string_view name, Cool::DirtyFlagFactory_Ref dirty_flag_factory)
        : _name{name}
        , _dirty_flag{dirty_flag_factory.make()}
    {
    }

    [[nodiscard]] auto name() const -> const std::string& { return _name; }

    void do_rendering(RenderParams params, UpdateContext_Ref update_ctx)
    {
        render(params, update_ctx);
        params.set_clean(_dirty_flag);
    }
    virtual void imgui_windows(Ui_Ref, UpdateContext_Ref) const = 0; /// The ui() method should be const, because it should only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level). If you really need to mutate one of your member variables, mark it as `mutable`.
    virtual void update(UpdateContext_Ref){};

    [[nodiscard]] virtual auto is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
    {
        return check_dirty(_dirty_flag);
    };

    [[nodiscard]] auto dirty_flag() const { return _dirty_flag; }

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