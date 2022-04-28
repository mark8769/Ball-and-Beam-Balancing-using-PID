/* Credits: Dr. Larson */

#ifndef SERVO_H_
#define SERVO_H_
//avr atmega32U4 robot controller board
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
#include "common.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * sending a PWM signal to move the servo motors
 * Can use these pins for signal wire on servo motors
 */
#define motor1_pwm_port DDRB
#define motor1_pwm_pin PORTB5

#define motor2_pwm_port DDRB
#define motor2_pwm_pin PORTB6

/*
 * The TOP calculation is ...
 * (16000000 / 64 ) / 50Hz ==> 5000.
 * 50HZ is the frequency needed for the servo motor
 */
#define motor1_servo_angle OCR1A
#define motor2_servo_angle OCR1B

void setup_timer1();
void setup_motor1();
void setup_motor2();
void set_servo_angle_one(int angle);
void set_servo_angle_two(int angle);
//use these to fine tune angles
//180 degrees grinds
//0 degrees does not, could go more?
//HAVE TO KNOW MIN AND MAX, AND HOW MUCH 1 DEGREE IS IN TERMS OF (MAX_ANGLE-MIN_ANGLE)/180
void set_ocra(int value);
void set_ocrb(int value);

/* https://dac.ge/pdfs/06309.pdf */
// find these values by looking up servo motor datasheet, this is Futaba S3003
// MIN width is .500 ms/500US .500/20 ==> .025% duty cycle = 125 // .025 * TOP
//static volatile int min_angle = 125;
static volatile int min_angle = 73;//found through testing
// MAX width is 3 ms/3000us 3/20 ==> 15.00% duty cycle = 750 // .15 * TOP
//static volatile int max_angle = 750;
static volatile int max_angle = 585;

#endif