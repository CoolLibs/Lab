#pragma once

#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ICommand.h"
#include "CommandCore/make_reversible_command.h"

namespace Lab {

namespace internal {

template<ConcreteCommandThatCanBeMadeReversible CommandT>
auto try_make_reversible(CommandT command, const MakeReversibleCommandContext_Ref& ctx)
    -> std::optional<ReversibleCommand>
{
    return make_reversible_command(command.make_reversible(ctx));
}

template<ConcreteCommand CommandT>
auto try_make_reversible(CommandT , const MakeReversibleCommandContext_Ref& )
    -> std::optional<ReversibleCommand>
{
    return {};
}

} // namespace internal

template<ConcreteCommand ConcreteCommandT>
class PolymorphicCommand final : public ICommand {
public:
    explicit PolymorphicCommand(ConcreteCommandT concrete_command)
        : _concrete_command{concrete_command}
    {
    }

    void execute(CommandExecutionContext_Ref& ctx) const override
    {
        _concrete_command.execute(ctx);
    }
    auto to_string() const -> std::string override
    {
        return _concrete_command.to_string();
    }
    auto try_make_reversible(const MakeReversibleCommandContext_Ref& ctx) const -> std::optional<ReversibleCommand> override
    {
        return internal::try_make_reversible(_concrete_command, ctx);
    }

private:
    ConcreteCommandT _concrete_command;
};

} // namespace Lab
