## Tests

### TEST_IS0_NODES cmake option

You can use TEST_IS0_NODES option on cmake to enable a mode that'll test all nodes upon launch of CooLab, display error message on the development console if needed and then close CooLab.

### How to enable TEST_IS0_NODES options

To enable TEST_IS0_NODES option, go to cmake cache(UI) and switch the option TEST_IS0_NODES ON using the checkbox. After checking nodes don't forget to check TEST_IS0_NODES OFF to use CooLab.

## To-Do

- world-up axis should also be an option in the UI (or does it ? could be annoying to support, and reduce performance by not knowing this value at compile time)
- properly declare dependencies of the modules
- automatically use the same type list for the Registries and the Commands
- ImGui style selector: allow to create and save your own style, or select amongst many pre-made styles (https://github.com/ocornut/imgui/issues/707)
- Have a menu pop up the first time you ever open the software to select a few options like the color theme.