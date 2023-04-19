# PicoGuide


This repository includes a project showing an example of how to use the RaspberryPi Pico multicore functionality.

## Build instructions

To build the project, follow the following steps.

**Note: these instructions assume you have configured your VS Code and operating system environemnt, so that you can successfully compile the `pico-examples` directory. Instructions on how to do this are here: https://github.com/sparamesvaran/PicoGuide/wiki/Software-set-up-with-pico-examples-build.**


1. Get a copy of the repositoriy in your preferred location.

```
git clone git@github.com:sparamesvaran/PicoGuide.git
```

2. In VS Code, and open the `PicoGuide` folder containing the repository cloned in the previous step.

3. Configure the project when prompted.

4. Ensure `GCC for arm-none-eabi` is the project kit.

5. Click `Build` to compile the code. The file which can be loaded on the Pico can be found under the `build/hello_multicore` directory.


## Additional information

It is suggested that you use the `hello_multicore/multicore.c` file as a starting point for the `multicore` temperature DAQ codebase.

The `hello_multicore/CMakeLists.txt` file already has the necessary statements to link against the Pico `ADC`, `RTC`, and `multicore` libraries. You should not need to modify `hello_multicore/CMakeLists.txt` to use these functionalities.

If using functions such as [multicore_lockout_start_blocking](https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#ga9a3f4df6b0e4140b0f40899a02c238d8) or [multicore_lockout_start_timeout_us](https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#gaaf40bf0b8b3a704f3178e38977d292be) to pause core 0, take care not to pause it for too long. Core 0 manages the USB communication, and needs to service interrupt requests (IRQs) in a timely manner. The core cannot do this when paused.