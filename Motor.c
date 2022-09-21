#include "Motor.h"

void motor_setup(){
    gpio_set_function(PWM_MOTOR, GPIO_FUNC_PWM);
}

void motor_pwm(uint16_t duty, uint16_t frec){
 
    uint16_t counts = (1.0/frec)/COUNTER_PERIOD;
    pwm_set_wrap(PWM_SLICE, counts);
    uint16_t setpoint = counts*(duty/100.0);
    pwm_set_chan_level(PWM_SLICE, PWM_CHAN_A, setpoint);
    pwm_set_enabled(PWM_SLICE, true);
    
    //printf("Frequency: %d \n", frec);
    //printf("Counts: %d \n", counts);
    //printf("Setpoint: %d \n", setpoint);

}