#pragma once
#include <Cool/Log/OptionalErrorMessage.h>
#include <ser20/types/polymorphic.hpp>
#include "Cool/Gpu/OpenGL/TextureRef.hpp"
#include "Cool/Gpu/RenderTarget.h"
#include "Dependencies/Ui.h"
#include "ShaderBased/DataToPassToShader.hpp"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class ModulesGraphNode;

class Module {
public:
    Module()                                     = default; // TODO(FeedbackLoop) remove?
    Module(Module const&)                        = delete;
    auto operator=(Module const&) -> Module&     = delete;
    Module(Module&&) noexcept                    = default;
    auto operator=(Module&&) noexcept -> Module& = default;
    virtual ~Module()                            = default;

    explicit Module(std::string_view name)
        : _name{name}
    {}

    Cool::NodesGraph const* _nodes_graph{}; // TODO(Particles) Remove

    [[nodiscard]] auto name() const -> const std::string& { return _name; }

    void do_rendering(DataToPassToShader const& data, std::vector<std::shared_ptr<ModulesGraphNode>> const& module_dependencies)
    {
        render(data, module_dependencies);
        _needs_to_rerender_flag.set_clean();
    }
    virtual void imgui_windows(Ui_Ref) const {}; /// The ui() method should be const, because it should only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level). If you really need to mutate one of your member variables, mark it as `mutable`.
    virtual void update() {};
    virtual void on_time_changed() {};
    virtual void on_time_reset() {};
    virtual void update_dependencies_from_nodes_graph(Cool::NodesGraph const&) {};

    [[nodiscard]] virtual auto needs_to_rerender() const -> bool
    {
        return _needs_to_rerender_flag.is_dirty();
    };

    [[nodiscard]] auto needs_to_rerender_flag() const -> Cool::DirtyFlag const& { return _needs_to_rerender_flag; }

    void log_module_error(Cool::OptionalErrorMessage const&, Cool::MessageSender&) const;

    virtual auto texture() const -> Cool::TextureRef { return _render_target.texture_ref(); }

    auto render_target() -> Cool::RenderTarget& { return _render_target; }
    auto render_target() const -> Cool::RenderTarget const& { return _render_target; }

private:
    virtual void render(DataToPassToShader const&, std::vector<std::shared_ptr<ModulesGraphNode>> const& module_dependencies) = 0;

private:
    std::string        _name;
    Cool::DirtyFlag    _needs_to_rerender_flag;
    Cool::RenderTarget _render_target{};

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