#include "Motor.h"

uint8_t reg = 0x00, buffer[1], diagnosis_buffer[5];     //SPI Diagonsis (0x00) register and buffer variable


void motor_setup(){
    gpio_set_function(PWM_MOTOR, GPIO_FUNC_PWM);    //Declare PWM pin
    gpio_set_function(EN_PIN, GPIO_OUT);  //Declare enable pin as output  
    gpio_put(EN_PIN, 0);        //Make driver enable by defualt. (Maybe is not a good idea).

    //SPI
    spi_init(SPI_PORT,100000); //Initialise SPI PORT at 100kHz
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCLK, GPIO_FUNC_SPI);

    spi_set_format(SPI_PORT,8,SPI_CPOL_0,SPI_CPHA_1,SPI_MSB_FIRST);     //Confuguration of SPI com parameters. 
    //See analog devices SPI tutorial or Rp2040 SDK C documentation to know more.

    gpio_init(CS);  //Initialise CS pin
    gpio_set_dir(CS, GPIO_OUT); //Set CS as outpput
    gpio_put(CS,1); //Set CS to high to indicate no current SPI comunication


}


//This function is used to detect if driver is working properly 
//If not it returns a 1. You can use it to stop system safely
int diagnosis(){
    gpio_put(CS,0); //Start SPI comunication
    //spi_write_blocking(SPI_PORT, &reg, 1); //Write a register
    spi_read_blocking(SPI_PORT, reg, buffer, 1);  //Read register "reg" in buffer "buffer"
    gpio_put(CS,1); //Stop SPI comms
    //printf("%d\n" , buffer[0]);

    //Detect if friver is working properly (Happens when data is 11011111 {223} or 0101111 {93})
    //Se driver datasheet for more info
    if(buffer[0] != 223 && buffer[0] != 93){
        printf("Something is WRONG... Analysing data...\n");
        diagnosis_buffer[0] = buffer[0] & 0x0F;                 //Extract first nibbel using a 0x0F mask

        //Split 4 reamining bits in different array position
        diagnosis_buffer[1] = (buffer[0] & 0x10)>>4;            
        diagnosis_buffer[2] = (buffer[0] & 0x20)>>5;
        diagnosis_buffer[3] = (buffer[0] & 0x40)>>6;
        diagnosis_buffer[4] = (buffer[0] & 0x80)>>7;
        printf("Data arrived.\nError code:"); 

        //Print error code in binary
        for(int i = 4; i>0;i--){
          printf("%b",diagnosis_buffer[i]); 
        }
        printf("%04b",diagnosis_buffer[0]); //"b" means that the number is printed in binary (b) and
        // 04 means that the minimum space printed will be for digits. If nunmber < 4 digits a 0 will be placed instead of a space.

        printf("\nThat means:\n ");
        

        //Error diagnostic conversion see datasheet table for more info
        if(diagnosis_buffer[3] == 0){
            printf("\tTOO HOT\n ");
        }
        if(diagnosis_buffer[1] == 0){
            printf("\tLIMITING CURRENT\n");
        }
        printf("\t");

   switch (diagnosis_buffer[0])
    {
    case 0x3:
        printf("UNDER VOLTAGE\n");
        break;
    case 0x5:
        printf("SHORT TO BAT AT OUT 1 AND OUT 2\n");
        break;
    case 0x6:
        printf("SHORT TO BAT AT OUT 2 AND SHORT TO GND AT OUT 1\n");
        break;
    case 0x7:
        printf("SHORT TO BAT AT OUT 2\n");
        break;
    case 0x9:
        printf("SHORT TO BAT AT OUT 1 AND SHORT TO GND AT OUT 2\n");
        break;
    case 0xA:
        printf("SHORT TO GND AT OUT 1 AND OUT 2\n");
        break;
    case 0xB:
        printf("SHORT TO GND AT OUT 2\n");
        break;
    case 0xC:
        printf("OPEN LOAD\n");
        break;
    case 0xD:
        printf("SHORT TO BATTERY AT OUT 1\n");
        break;
    case 0xE:
        printf("SHORT TO GND AT OUT 1\n");
        break;
    
    default:
        printf("\n");
        break;
    }

    sleep_ms(2000);
    return 1;   //If something is wrong return a 1


    }
    return 0;  //If not return 0
}



//Given a duty cycle and a frequency this function drives motor dirver.
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