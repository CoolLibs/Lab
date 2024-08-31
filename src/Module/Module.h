#pragma once
#include <Cool/Log/OptionalErrorMessage.h>
#include <ser20/types/polymorphic.hpp>
#include "Dependencies/Ui.h"
#include "ShaderBased/DataToPassToShader.hpp"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Module {
public:
    Module()                                 = default;
    Module(Module const&)                    = delete;
    auto operator=(Module const&) -> Module& = delete;

protected:
    Module(Module&&) noexcept                    = default;
    auto operator=(Module&&) noexcept -> Module& = default;

public:
    virtual ~Module() = default;

    explicit Module(std::string_view name)
        : _name{name}
    {
    }

    [[nodiscard]] auto name() const -> const std::string& { return _name; }

    void do_rendering(DataToPassToShader const& data)
    {
        render(data);
        _needs_to_rerender_flag.set_clean();
    }
    virtual void imgui_windows(Ui_Ref) const = 0; /// The ui() method should be const, because it should only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level). If you really need to mutate one of your member variables, mark it as `mutable`.
    virtual void update() {};

    [[nodiscard]] virtual auto needs_to_rerender() const -> bool
    {
        return _needs_to_rerender_flag.is_dirty();
    };

    [[nodiscard]] auto needs_to_rerender_flag() const -> Cool::DirtyFlag const& { return _needs_to_rerender_flag; }

protected:
    void log_module_error(Cool::OptionalErrorMessage const&, Cool::MessageSender&) const;

private:
    virtual void render(DataToPassToShader const&) = 0;

private:
    std::string     _name;
    Cool::DirtyFlag _needs_to_rerender_flag;

private:
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Name", _name),
            ser20::make_nvp("Dirty Flag: needs to rerender", _needs_to_rerender_flag)
        );
    }
};

} // namespace Lab