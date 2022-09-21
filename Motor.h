#ifndef MOTOR_H
    #define MOTOR_H

    #include <stdio.h>
    #include "hardware/pwm.h"
    #include "hardware/dma.h"
    #include "hardware/adc.h"
    #include "hardware/gpio.h"

    #define PWM_MOTOR 20
    #define PWM_SLICE 2 
    #define COUNTER_PERIOD 0.000000008  //CLOCK PERIOD
   
    void motor_setup();
    void motor_pwm(uint16_t duty, uint16_t frec);  
        
#endif