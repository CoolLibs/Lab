## Tests

### TEST_IS0_NODES cmake option

You can use TEST_IS0_NODES option on cmake to enable a mode that'll test all nodes upon launch of CooLab, display error message on the development console if needed and then close CooLab.

### How to enable TEST_IS0_NODES options

To enable TEST_IS0_NODES option, go to cmake cache(UI) and switch the option TEST_IS0_NODES ON using the checkbox. After checking nodes don't forget to check TEST_IS0_NODES OFF to use CooLab.

### Simulation modules

Modules that need physics simulation step by step can store a `lastTimepoint` and when the need to rerender with a new timepoint they now how long has elapsed, and can run as many steps as needed, based of a step size. This both allwos to query those modules at any timepoint, and to have a step size decorelated from the update rate of the application. PB: when a parameter of those system changes,we need to run them all the way to the current timepoint, which can be slow. (can we do a preview were it runs as fast as possible to reach the current time ? But this might be confusing if time is paused but simulation still seems to run)
