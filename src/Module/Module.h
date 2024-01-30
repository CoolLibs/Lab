#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <Dependencies/InputProvider_Ref.h>
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
        InputProvider_Ref provider;
        img::Size         render_target_size; // TODO(Variables) Move to InputProvider?
    };

    Module()                                 = default;
    Module(Module const&)                    = delete;
    auto operator=(Module const&) -> Module& = delete;

protected:
    Module(Module&&) noexcept                    = default;
    auto operator=(Module&&) noexcept -> Module& = default;

public:
    virtual ~Module() = default;

    Module(std::string_view name)
        : _name{name}
    {
    }

    [[nodiscard]] auto name() const -> const std::string& { return _name; }

    void do_rendering(RenderParams params)
    {
        render(params);
        _needs_to_rerender_flag.set_clean();
    }
    virtual void imgui_windows(Ui_Ref, UpdateContext_Ref) const = 0; /// The ui() method should be const, because it should only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level). If you really need to mutate one of your member variables, mark it as `mutable`.
    virtual void update(UpdateContext_Ref){};

    [[nodiscard]] virtual auto needs_to_rerender() const -> bool
    {
        return _needs_to_rerender_flag.is_dirty();
    };

    [[nodiscard]] auto needs_to_rerender_flag() const -> Cool::DirtyFlag const& { return _needs_to_rerender_flag; }

protected:
    void log_module_error(Cool::OptionalErrorMessage const&, Cool::MessageSender&) const;

private:
    virtual void render(RenderParams) = 0;

private:
    std::string     _name;
    Cool::DirtyFlag _needs_to_rerender_flag;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Dirty Flag: needs to rerender", _needs_to_rerender_flag)
        );
    }
};

} // namespace Lab