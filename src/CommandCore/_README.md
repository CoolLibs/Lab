## Understanding all the abstractions

There are a lot of abstractions to wrap your head around in order to understand how the communication inside the application happens. This might hurt your head a little bit at first, but this gives us a very powerful way to handle user actions, and it minimizes code duplication and boilerplate in other parts of the code. Basically all the systems only have to know about Commands, and then submit them to the Dispatcher; the later is responsible for all the magic like History, Recording, etc.

### Command

Anything that you want to happen, plus the data needed to perform said action.

Examples:
```cpp
// Ask to set the value of the object referenced by `id` to `value`
struct Command_SetValue<int> {
    reg::Id id;
    int value;
}:

// Ask to create an Is0 Module in the current Scene
struct Command_CreateIs0Module {}; // Structs can be empty if there is no specific data associated with the instruction
```

It is important to understand that a Command doesn't contain any implementation: it is merely a request! The execution of the command is done elsewhere (by an Executor) and this is what allows such a flexible and decoupled design.

### Reversible Command

A command with the additional data required to undo it.

### Executor

Does the work that the command asks for. This is where all the implementation happens, where we access the Registries and other top-level objects that are at the root of the App.

### ReversibleExecutor

An Executor that also knows how to revert the (reversible) commands.

### Dispatcher

Does a few things and then forwards to the Executor to run the actual command.
For example it could:
- Push in the history if the command is reversible (or rather if you can construct a reversible command from it, thanks to `make_reversible_command()`)
- Decide to discard some commands based on the current state of the app (e.g. we are exporting a video so we want to block all other commands to prevent parameter values from changing).
- etc.

### Merger

Knows how to merge two similar commands together. For example `set x from 2 to 3` and `set x from 3 to 5` can be merged as `set x from 2 to 5`. This is useful for the History: when we drag a slider we will issue many `SetValue` commands, but in the end we only want one of them in the History: from the value before dragging, to the value when the slider was released. This is achieved by merging all the commands that are submitted during the drag.<br/>
And if we were to start dragging the same slider, those new commands wouldn't be merged with the previous ones because we explicitly told the history not to thanks to the `FinishedEditingValue` command.