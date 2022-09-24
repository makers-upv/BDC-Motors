#ifndef MOTOR_H
    #define MOTOR_H

    #include <stdio.h>
    #include "hardware/pwm.h"
    #include "hardware/dma.h"
    #include "hardware/adc.h"
    #include "hardware/gpio.h"
    #include "hardware/spi.h"

    //Motor pins
    #define EN_PIN 22   //ENABLE MOTOR DRIVER PIN
    #define PWM_MOTOR 20    
    #define PWM_SLICE 2 
    #define COUNTER_PERIOD 0.000000008  //CLOCK PERIOD


    #define MISO 16
    #define CS 17
    #define SCLK 18
    #define MOSI 19
    
    #define SPI_PORT spi0
   
   
    
    

    int diagnosis();
    void motor_setup();
    void motor_pwm(uint16_t duty, uint16_t frec);  
        
    
#endif