#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Gpu/FullscreenPipeline.h"
#include "Module/Module.h"

namespace Lab {

class Module_Compositing : public Module {
public:
    Module_Compositing() = default;
    Module_Compositing(std::string texture_name_in_shader, std::vector<std::shared_ptr<Module>> modules_that_we_depend_on, std::vector<Cool::NodeId> nodes_that_we_depend_on);
    Module_Compositing(Module_Compositing const&)                    = delete;
    auto operator=(Module_Compositing const&) -> Module_Compositing& = delete;
    // Module_Compositing(Module_Compositing&&) noexcept                    = default; // TODO(Modules)
    // auto operator=(Module_Compositing&&) noexcept -> Module_Compositing& = default; // TODO(Modules)

    void update() override;
    void imgui_generated_shader_code_tab() override;

    void reset_shader();

    void set_shader_code(tl::expected<std::string, std::string> const& shader_code);

private:
    void render(DataToPassToShader const&) override;
    void log_shader_error(Cool::OptionalErrorMessage const&) const;

private:
    mutable std::string              _shader_code{};
    mutable Cool::FullscreenPipeline _pipeline{};
    mutable Cool::MessageSender      _shader_error_sender{};

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
