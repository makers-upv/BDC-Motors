cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico-projects)

pico_sdk_init()

add_executable(Brushed_main
 Brushed_main.c
 Motor.c
 )

target_link_libraries(Brushed_main
 pico_stdlib 
 hardware_pwm 
 hardware_adc
 hardware_dma)

pico_add_extra_outputs(Brushed_main)

pico_enable_stdio_usb(Brushed_main 1)
pico_enable_stdio_uart(Brushed_main 0)
