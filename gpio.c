#include "gpio.h"
#include "common.h"

static uint8_t gpioZeroInit = 0;
static uint8_t gpioOneInit = 0;
static uint8_t gpioTwoInit = 0;
static uint8_t gpioThreeInit = 0;
static uint8_t gpioFourInit = 0;
static uint8_t gpioFiveInit = 0;
static uint8_t gpioSixInit = 0;
static uint8_t gpioThirteenInit = 0;

void gpio_initialize_led(int color) {

    switch(color){

        case GPIO_0_GENERAL:
            //using pin 0 on header, red led.
            //use pinout sheet for more clarification
            //Pin 0 on header connects to PD2
            _gpio_zero_general = (IO_struct) {&DDRD, &PORTD, GPIO_0_GENERAL, &PIND};
            //function in leds.c
            //configurs the data direction port for specified led
            configure_led(&_gpio_zero_general);
            //using this value for the light show only function only
            gpioZeroInit = 1;
            break;
/************************************ ADDING THESE FOR PROJECT ****************************************/
        case GPIO_1_GENERAL:
            //using pin 0 on header, red led.
            //use pinout sheet for more clarification
            //Pin 0 on header connects to PD2
            _gpio_one_general = (IO_struct) {&DDRD, &PORTD, GPIO_1_GENERAL, &PIND};
            //function in leds.c
            //configurs the data direction port for specified led
            configure_led(&_gpio_one_general);
            //using this value for the light show only function only
            gpioOneInit = 1;
            break;
/************************************ ^CHECK FOR BUGS HERE^ **********************************************/
        case GPIO_2_GENERAL:
            //using pin 2 on header, yellow led.
            //Pin 2 on header, connects to PD1
            _gpio_two_general = (IO_struct) {&DDRD, &PORTD, GPIO_2_GENERAL, &PIND};
            configure_led(&_gpio_two_general);
            gpioTwoInit = 1;
            break;
/************************************ ADDING THESE FOR PROJECT ****************************************/
        case GPIO_3_GENERAL:
            //Pin 3 on A-Star
            //PD0
            _gpio_three_general = (IO_struct) {&DDRD, &PORTD, GPIO_3_GENERAL, &PIND};
            configure_led(&_gpio_three_general);
            gpioThreeInit = 1;
            break;

/************************************ ^CHECK FOR BUGS HERE^ **********************************************/
        case GPIO_4_GENERAL:
            //using pin 4 on header, green led.
            //Pin 4 on header, connects to PD4
            _gpio_four_general = (IO_struct) {&DDRD, &PORTD, GPIO_4_GENERAL, &PIND};
            configure_led(&_gpio_four_general);
            gpioFourInit = 1;
            break;

/************************************ ADDING THESE FOR PROJECT ****************************************/
        case GPIO_5_GENERAL:
            //Pin 5 on header/A-Star
            //Connects to PC6
            _gpio_five_general = (IO_struct) {&DDRC, &PORTC, GPIO_5_GENERAL, &PINC};
            configure_led(&_gpio_five_general);
            gpioFiveInit = 1;
            break;

        case GPIO_6_GENERAL:
            //Pin 6 on header/A-Star
            //Connects to PD7
            _gpio_six_general = (IO_struct) {&DDRD, &PORTD, GPIO_6_GENERAL, &PIND};
            configure_led(&_gpio_six_general);
            gpioSixInit = 1;
            break;
        
        case GPIO_13_GENERAL:
            //Pin 13 on header/A-Start
            //Connects to PC7
            _gpio_thirteen_general = (IO_struct) {&DDRC, &PORTC, GPIO_13_GENERAL - 1, &PIND};
            configure_led(&_gpio_thirteen_general);
            gpioThirteenInit = 1;
            break;

    }
}
/************************************ ^CHECK FOR BUGS HERE^ **********************************************/

// void gpio_light_show() {
//   int i;
//   for (i = 0; i < 2; i++) {
//     //second parameter, indicates if led is inverted
//     //pretty sure its only 2 leds that are, the rest are not so put 0
//     //can always ask later too
//     if (gpioRedInit) flash_led(&_gpio_red, 0);
//     if (gpioYellowInit) flash_led(&_gpio_yellow, 0);
//     if (gpioGreenInit) flash_led(&_gpio_green, 0);
//   }
// }
