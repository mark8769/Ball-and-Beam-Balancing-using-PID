#include "servo.h"

void setup_timer1(){
    
  TCCR1A = 0;
  TCCR1B = 0;
  // Prescaler 64 = clock select pins 011
  TCCR1B |= (1 << CS10) | (1 << CS11);

  // Waveform Generation 14 ==> 1110
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // The TOP calculation is ...
  // (16000000 / prescaler ) / 50Hz ==> 5000.

  // Set top (WGM 14 uses ICR1)
  ICR1 = 5000;
}

void setup_motor1() {
  // ******* MUST SET UP TIMER1 AS WELL ***********

  // Output for Motor 1
  DDRB |= (1 << PORTB5);

  // COM to clear on match = 10
  TCCR1A |= (1 << COM1A1);

  // Start with servo in neutral position (I think 1.78ms ==> 1.78/20 8.9% duty)
  //motor1_servo_angle = 445; // 8.9% duty cycle based on 5000 top
  motor1_servo_angle = min_angle;
}

void setup_motor2() {
  // ******* MUST SET UP TIMER1 AS WELL ***********

  // Output for Motor 1
  DDRB |= (1 << PORTB6);

  // COM to clear on match = 10
  TCCR1A |= (1 << COM1B1);

  // Start with servo in neutral position (I think 1.78ms ==> 1.78/20 8.9% duty)
  //motor2_servo_angle = 445; // 8.9% duty cycle based on 5000 top
  motor2_servo_angle = min_angle;
}

// min = 0 degree position
// 180 degrees of motion for servo (0 - 180)
// 125 to 750. Min_angle to Max_angle = range for angles.
// (585 - 73) / 180 = 2.84 . 2.84 = delta for every 1 degree.
// want this number to be closer to an integer value to be more accurate
// use for now, and change if PID doesnt look smooth
void set_servo_angle_one(int angle) {

	//if motor 1 on
	if (PINB && (1 << PINB5)){
		motor1_servo_angle = (int)(min_angle + (angle * 2.84));
	}else{
		//prinf("Servo motor not on.")
	}
}
void set_servo_angle_two(int angle) {

	//if motor 2 on
	if (PINB && (1 << PINB6)){
		motor2_servo_angle = (int)(min_angle + (angle * 2.84));
	}else{
		//printf("Servo motor not on");
	}
}

void set_ocra(int value){
    
    motor1_servo_angle = value;
}

void set_ocrb(int value){
    
    motor2_servo_angle = value;
}
