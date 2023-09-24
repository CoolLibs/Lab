#pragma once

namespace Lab {

class MakeReversibleCommandContext_Ref;
class CommandExecutionContext_Ref;

template<typename T>
concept ConcreteCommand = requires(const T& a, const T& b, CommandExecutionContext_Ref& command_execution_context, const MakeReversibleCommandContext_Ref& make_reversible_command_context) {
                              // Has an execute() method
                              a.execute(command_execution_context);
                              // Has a to_string() method
                              {
                                  a.to_string()
                                  } -> std::convertible_to<std::string>;

                              // TODO require a serialization function
                          };

template<typename T>
concept ConcreteCommandThatCanBeMadeReversible = ConcreteCommand<T> && requires(const T& a, const MakeReversibleCommandContext_Ref& make_reversible_command_context) {
                                                                           // Has a make_reversible() method
                                                                           a.make_reversible(make_reversible_command_context);
                                                                       };

} // namespace Lab
