#pragma once
#include "Module/Module.h"
#include "Module/ModuleDependencies.h"

namespace Lab {
class Module_FeedbackLoop : public Module {
public:
    Module_FeedbackLoop() = default;
    Module_FeedbackLoop(std::string texture_name_in_shader, std::vector<std::shared_ptr<Module>> deps);
    Module_FeedbackLoop(Module_FeedbackLoop const&)                        = delete;
    auto operator=(Module_FeedbackLoop const&) -> Module_FeedbackLoop&     = delete;
    Module_FeedbackLoop(Module_FeedbackLoop&&) noexcept                    = default;
    auto operator=(Module_FeedbackLoop&&) noexcept -> Module_FeedbackLoop& = default;
    ~Module_FeedbackLoop() override                                        = default;

    void               on_time_reset() override;
    auto               texture() const -> Cool::TextureRef override;
    [[nodiscard]] auto needs_to_rerender() const -> bool override;
    void               before_module_graph_renders() override;

    [[nodiscard]] auto depends_on() const -> ModuleDependencies const& { return _depends_on; }
    void               update_dependencies_from_nodes_graph(Cool::NodesGraph const& graph) override { Lab::update_dependencies_from_nodes_graph(_depends_on, graph); }

private:
    void render(DataToPassToShader const&) override;

private:
    Cool::RenderTarget _render_target{};
    ModuleDependencies _depends_on{};
    bool               _bob{false};
    int                _renders_count{0};
    bool               _rerender_next_frame{false};
    bool               _rerender_this_frame{false};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Base Module", ser20::base_class<Module>(this))
        );
    }
};

} // namespace Lab
