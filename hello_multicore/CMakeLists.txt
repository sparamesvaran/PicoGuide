add_executable(hello_multicore
        multicore.c
        )

# Add pico_multicore which is required for multicore functionality
target_link_libraries(hello_multicore
        pico_stdlib
        pico_multicore
        hardware_adc
        hardware_rtc)

pico_enable_stdio_usb(hello_multicore 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(hello_multicore)