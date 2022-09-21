#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "Motor.h"


//Motor pins
#define EN_PIN 22

//ADC Capture stuff
#define CAPTURE_CHANNEL 0  
#define CAPTURE_DEPTH 200


//PCB component values
#define SHUNT 0.02
#define VREF 3.3
#define ADC_RESOLUTION 4096
#define INA_GAIN 50 

//Sensors trasnfer functions 
#define Current(adc_value) (adc_value*VREF/(ADC_RESOLUTION*SHUNT*INA_GAIN) )   //V = 20
#define Voltage(adc_value) (adc_value*VREF/(ADC_RESOLUTION) *11 )   

//Sensor values buffers
float current_buffer[CAPTURE_DEPTH];
float voltage_buffer[CAPTURE_DEPTH];

int capture_counter = 0;

 bool repeating_timer_callback(struct repeating_timer *t) {
  adc_select_input(0);
  current_buffer[capture_counter] = Current(adc_read());
  adc_select_input(1);
  voltage_buffer[capture_counter] = Voltage(adc_read());
  printf("Current: %-3f\n   ", current_buffer[capture_counter]);
  printf("Voltage: %-3f\n ", voltage_buffer[capture_counter]);
  
  if(capture_counter < CAPTURE_DEPTH - 1){
    capture_counter++;
  }else{
    capture_counter = 0;
  }
 return true;
 }

 





void setupADC() {
  stdio_init_all();
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);
}


int main() {
    stdio_init_all();

    setupADC();
    motor_setup();
    //gpio_set_function(PWM_MOTOR, GPIO_FUNC_PWM);
    gpio_set_function(EN_PIN, GPIO_OUT);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    //uint slice_num = pwm_gpio_to_slice_num(PWM_MOTOR);
    // Set period of n cycles (0 to n inclusive)
    //pwm_set_wrap(slice_num, 6250);
    // Set channel A output high for one cycle before dropping
    //pwm_set_chan_level(slice_num, PWM_CHAN_A, 250);
  
   
    motor_pwm(50,10000);
  

    gpio_put(EN_PIN, 0);

 // Create a repeating timer that calls repeating_timer_callback.
 // If the delay is > 0 then this is the delay between the previous callback ending and the
//next starting.
 // If the delay is negative (see below) then the next call to the callback will be exactly
//500ms after the
 // start of the call to the last callback
 struct repeating_timer timer;
 add_repeating_timer_us(500, repeating_timer_callback, NULL, &timer);
 sleep_ms(3000);
 

 



    while(1){
      //do stuff
    }
    
}



