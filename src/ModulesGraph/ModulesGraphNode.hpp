#pragma once
#include "Module/Module.h"

namespace Lab {

struct ModulesGraphNode {
    ModulesGraphNode() = default;
    // ~ModulesGraphNode() = default;

    // ModulesGraphNode(const ModulesGraphNode&)                      = delete; // We disable copying
    // ModulesGraphNode& operator=(const ModulesGraphNode&)           = delete; // We disable copying
    // ModulesGraphNode(ModulesGraphNode&& other) noexcept            = default;
    // ModulesGraphNode& operator=(ModulesGraphNode&& other) noexcept = default;

    ModulesGraphNode(std::shared_ptr<Module> module, std::string texture_name_in_shader, std::vector<std::shared_ptr<ModulesGraphNode>> dependencies)
        : module{std::move(module)}
        , texture_name_in_shader{std::move(texture_name_in_shader)}
        , dependencies{std::move(dependencies)}
    {
    }

    std::shared_ptr<Module>                        module;
    std::string                                    texture_name_in_shader{};
    std::vector<std::shared_ptr<ModulesGraphNode>> dependencies{};

    auto needs_to_rerender() const -> bool;

private:
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Module", module)
        );
    }
};

} // namespace Lab