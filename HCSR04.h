#ifndef HCSR04_H_
#define HCSR04_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/* 
 * Ground to ground, last row on header
 * Echo pin, to an external interrupt. This code is A-Star Pin 2 (PD1 has special function INT1)
 * Trig pin, any I/O pin, this code is A-Star pin 12 (PD6)
 * VCC, to 5V power
 */

//Using Atmega32U4
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
//MHZ 16_000_000 ticks/second
//using this for timer 1, timer1 counts at same rate as F_CPU
#define F_CPU 16000000

/* Note: microseconds = 1_000_000us/second */
/* our cpu is 16 times faster than that, so will have to divide pulse by 16 before computing distance */
//store pulse inside this variable, Timer will get in microseconds * 16(because of F_CPU rate). After storing pulse. Divide by 16
static volatile int pulse = 0;
//compute distance, store here. (microseconds / 58)
extern volatile int sonic_distance;

/* Can change trig to any pin on A-Star, will need to change defines accordingly */
/* Can change echo to any pin on A-Star with external interrupt, will need to change defines accordingly */
/* Help for changing defines, 10.4 Register Description for I/O ports. p. 84 */
//Data Direction Registers for echo, and trig pins
#define echo_ddrx DDRD
// #define trig_ddrx DDRD
#define trig_ddrx DDRC
//IO pins for IO ports being used
#define echo_ddr_pin DDD1
//#define trig_ddr_pin DDD6
#define trig_ddr_pin DDC7
//after setting DDRx port to output, can start triggering with trigger_function
//#define trig_port PORTD
//#define trig_pin PIND6

#define trig_port PORTC
#define trig_pin PINC7 //moved trigger to non-adc channel

//EICRA for external interrupt 0-3
//EICRB for external interrupt 6
//change accordingly
//11.1.1 p.89 Datasheet
#define interrupt_logic_register EICRA
//choose from external interrupts
//External Interrupts INTn (0-3, 6)
#define interrupt_n INT1

void setup_sonic_sensor();
void trigger_sonic_sensor();
int get_sonic_reading();


extern volatile uint64_t sonic_counter;

#endif