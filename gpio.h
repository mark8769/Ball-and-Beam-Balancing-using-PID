#ifndef GPIO_H_

#define GPIO_H_

#include "common.h"
#include <util/delay.h>
#include <inttypes.h>
#include "leds.h"

//GPIO_pinOnPoluluBoardHeader_colorConnected = PortD pin #
//General Purpose IO (GPIO)
//PortD 2, Pin 0 on A-Star
#define GPIO_0_GENERAL 2//PREV RED
//Port D 3, Pin 1 on A-Star
#define GPIO_1_GENERAL 3
//PortD 1, Pin 2 on A-Star
#define GPIO_2_GENERAL 1//PREV YELLOW
//PortD 4, Pin 4 on A-Star
#define GPIO_4_GENERAL 4//PREV GREEN
//PORTC 6, PIN 5 on A-Star
#define GPIO_5_GENERAL 6
//PORTD 7, PIN 6 on A-Star
#define GPIO_6_GENERAL 7
//PORTC 7, PIN 13, make sure to subtract one in case statement
//Cant use 7 again, just a quick fix instead of renaming things or enuming names
#define GPIO_13_GENERAL 8
//PORTD 0, PIN3 on A-Star
#define GPIO_3_GENERAL 0


//USE THESE FOR ADC CHANNELS, EXAMPLE POTENTIOMETERS
// //PORTE 6, PIN 7
// #define GPIO_7_GENERAL 6

// //PORTB 4, PIN 8
// #define GPIO_8_GENERAL 4

// //PORTB 6, PIN 10
// #define GPIO_B_6_GENERAL 6
//Global Variables, define with initialized led (<color>)

IO_struct _gpio_zero_general;
IO_struct _gpio_one_general;
IO_struct _gpio_two_general;
IO_struct _gpio_four_general;

IO_struct _gpio_three_general; 
IO_struct _gpio_five_general;
IO_struct _gpio_six_general;
IO_struct _gpio_thirteen_general;

void gpio_initialize_led(int color);

//void gpio_light_show();




#endif