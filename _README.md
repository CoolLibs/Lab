## Tests

### TEST_IS0_NODES cmake option

You can use TEST_IS0_NODES option on cmake to enable a mode that'll test all nodes upon launch of CooLab, display error message on the development console if needed and then close CooLab.

### How to enable TEST_IS0_NODES options

To enable TEST_IS0_NODES option, go to cmake cache(UI) and switch the option TEST_IS0_NODES ON using the checkbox. After checking nodes don't forget to check TEST_IS0_NODES OFF to use CooLab.

## To-Do

- merge similar commands in history
- only submit a {id, value} command and let the dispatcher create a reversible command from this by adding the old value
- give a maximum size to the history
- give a maximum *saved* size to the history, and turn this into an option that shows up in the UI
- world-up axis should also be an option in the UI (or does it ? could be annoying to support, and reduce performance by not knowing this value at compile time)
- properly declare dependencies of the modules