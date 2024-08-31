#pragma once
#include "Module/Module.h"
#include "Module/ModuleDependencies.h"
#include "Module_Compositing/Module_Compositing.h"

namespace Lab {
class Module_FeedbackLoop : public Module {
public:
    Module_FeedbackLoop();
    Module_FeedbackLoop(Module_FeedbackLoop const&)                        = delete;
    auto operator=(Module_FeedbackLoop const&) -> Module_FeedbackLoop&     = delete;
    Module_FeedbackLoop(Module_FeedbackLoop&&) noexcept                    = default;
    auto operator=(Module_FeedbackLoop&&) noexcept -> Module_FeedbackLoop& = default;
    ~Module_FeedbackLoop() override                                        = default;

    void on_time_reset() override;

    [[nodiscard]] auto depends_on() const -> ModuleDependencies const& { return _depends_on; }
    void               update_dependencies_from_nodes_graph(Cool::NodesGraph const& graph) override { Lab::update_dependencies_from_nodes_graph(_depends_on, graph); }

private:
    void render(DataToPassToShader const&) override;

private:
    // Module_Compositing _compositing_module{};
    ModuleDependencies _depends_on{};

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
