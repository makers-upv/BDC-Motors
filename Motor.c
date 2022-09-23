#include "Motor.h"

void motor_setup(){
    gpio_set_function(PWM_MOTOR, GPIO_FUNC_PWM);    //Declare PWM pin
    gpio_set_function(EN_PIN, GPIO_OUT);  //Declare enable pin as output  
    gpio_put(EN_PIN, 0);        //Make driver enable by defualt. (Maybe is not a good idea).

    //SPI
    spi_init(SPI_PORT,100000);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);

    spi_set_format(SPI_PORT,8,SPI_CPOL_0,SPI_CPHA_1,SPI_MSB_FIRST);

    gpio_init(CS);  //Initialise CS pin
    gpio_set_dir(CS, GPIO_OUT); //Set CS as outpput
    gpio_put(CS,1); //Set CS to high to indicate no current SPI comunication


}




void motor_pwm(uint16_t duty, uint16_t frec){
 
    uint16_t counts = (1.0/frec)/COUNTER_PERIOD; //Calculate how many clocks counts are needed for the desired frec
    pwm_set_wrap(PWM_SLICE, counts);             
    uint16_t setpoint = counts*(duty/100.0);      //Calculate how many clocks counts are needed for the desired duty cycle
    pwm_set_chan_level(PWM_SLICE, PWM_CHAN_A, setpoint);
    pwm_set_enabled(PWM_SLICE, true);              //Start PWM
    
    //printf("Frequency: %d \n", frec);
    //printf("Counts: %d \n", counts);
    //printf("Setpoint: %d \n", setpoint);

}