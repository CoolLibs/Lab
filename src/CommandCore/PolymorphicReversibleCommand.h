#pragma once

#include "CommandCore/ConcreteReversibleCommand.h"
#include "CommandCore/IReversibleCommand.h"

namespace Lab {

template<ConcreteReversibleCommand ConcreteReversibleCommandT>
class PolymorphicReversibleCommand final : public IReversibleCommand {
public:
    PolymorphicReversibleCommand() = default;

    explicit PolymorphicReversibleCommand(const ConcreteReversibleCommandT& concrete_reversible_command)
        : _concrete_reversible_command{concrete_reversible_command}
    {
    }

    void execute(CommandExecutionContext_Ref const& ctx) const override
    {
        _concrete_reversible_command.execute(ctx);
    }
    void revert(CommandExecutionContext_Ref const& ctx) const override
    {
        _concrete_reversible_command.revert(ctx);
    }
    auto to_string() const -> std::string override
    {
        return _concrete_reversible_command.to_string();
    }
    auto merge(IReversibleCommand const& previous) const -> std::optional<ReversibleCommand> override
    {
        const auto* const prev =
            dynamic_cast<const PolymorphicReversibleCommand<ConcreteReversibleCommandT>*>(&previous);
        if (prev) // Only forward to the underlying command if `previous` has the same type as `_concrete_reversible_command`
        {
            const auto maybe_merged = _concrete_reversible_command.merge(prev->_concrete_reversible_command);
            if (maybe_merged)
            {
                return make_reversible_command(*maybe_merged);
            }
        }
        return {};
    }

private:
    ConcreteReversibleCommandT _concrete_reversible_command;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(_concrete_reversible_command);
    }
};

} // namespace Lab