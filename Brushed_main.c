#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "Motor.h"
#include "math.h"




//PCB component values
#define SHUNT 0.02
#define VREF 3.3
#define ADC_RESOLUTION 4095
#define INA_GAIN 50 
#define DIVIDER_GAIN 11

//Sensors trasnfer functions 
#define Current(adc_value) (adc_value*VREF/(ADC_RESOLUTION*SHUNT*INA_GAIN) )   //V = 20
#define Voltage(adc_value) (adc_value*VREF/(ADC_RESOLUTION) *DIVIDER_GAIN )   

//Sensor values buffers
#define CAPTURE_DEPTH 200

float current_buffer[CAPTURE_DEPTH];
float voltage_buffer[CAPTURE_DEPTH];

int capture_counter = 0;



bool repeating_timer_callback(struct repeating_timer *t) {
  adc_select_input(0);
  current_buffer[capture_counter] = Current(adc_read());
  adc_select_input(1);
  voltage_buffer[capture_counter] = Voltage(adc_read());
  //printf("Current: %-3f\n   ", current_buffer[capture_counter]);
  //printf("Voltage: %-3f\n ", voltage_buffer[capture_counter]);
  
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
     
    motor_pwm(50,10000);  //Turn on PWM. 
  
  
    struct repeating_timer timer;
    add_repeating_timer_us(500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);



    while(1){

      
      diagnosis();
      sleep_ms(10);
      

      //Code to read USB uart in a non blocking way
      int value = getchar_timeout_us(100);

      //If a msg arrives, do something depending on payload
      //This probably will be hidden on a function
      if(value != PICO_ERROR_TIMEOUT){  
        switch (value)
        {
        case 'A':
          printf("A");
          break;
        case 'B':
          printf("B");
          break;
        default:
         printf("%d\n",value);
          break;
        }
      }
       
    }
    
}



