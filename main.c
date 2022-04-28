#include "servo.h"
#include "HCSR04.h"
//* Use serial h when trying to debug, avoid using when trying to balance ball, too slow *//
#include "serial.h"
#include "gpio.h"
#include "analog.h"
//* Led flags *//
volatile uint8_t led_on_one = 0;
volatile uint8_t led_on_five = 0;
volatile uint8_t led_on_six = 0;
//* pid_control stuff *//
volatile int setPoint = 0;
volatile int pri_error = 0;
volatile int error_build_up = 0;
//* Try to reduce global variables when there is more time *//
//* Extern to be able to print out values to serial comm. use for debugging / tunning *//
extern volatile double kP = 8.10;//10 worked, so half 5, .0040, 3300
//extern volatile double kI = 0.00019;//.001 works good with kp 8, kd 4500 best kd?
extern volatile double kI = 0.00099;
//extern volatile double kI = 0;
extern volatile int kD = 4500;//used to be 5000
//extern volatile int kD = 0;
volatile int error_buildup = 0;

//* Helper functions for pid_control, checking pots *//
void pid_control(int temp_distance);
void check_led_adc(int adc_results);
void tune_kp(int adc_results);
void tune_ki(int adc_results);
void tune_kd(int adc_results);

void blink_yellow() {
  // YELLOW LED on C7 - inverted ??
  PORTC |= (1 << PORTC7);
  _delay_ms(300);
  PORTC &= ~(1 << PORTC7);
  _delay_ms(300);
}
void pid_control(int temp_distance){

  int error = setPoint - temp_distance;
  
  double p_value = error * kP;
  double i_value = error_build_up * kI;
  double d_value = (error - pri_error) * kD;

  double pid_value = p_value + i_value + d_value;
  pri_error = error; //current error
  error_build_up += error; //add build up until significatnt change, similar to lab 7. Integrate error build up

  int f_value = (int)pid_value; //convert to servo motor degrees
  double map_value = 0.0;
  int temp_f_value = f_value;

  //https://srituhobby.com/what-is-a-pid-controller-and-how-does-it-work-with-an-arduino/
  //http://electronoobs.com/eng_arduino_tut100_code1.php
  //https://www.arduino.cc/reference/en/language/functions/math/map/
  //https://www.youtube.com/watch?v=HRJiow_k-V0
  /*
  * All the arduino tutorials used the map function.
  * Not exactly sure how it works yet
  * But presuming I was using an arduino board
  * F_value = map(F_value,-180,180,0,180);
  * Servo has 180 degrees of motion, so 0 - 180 makes sense
  * Not too sure where they get -180, but every tutorial with different vals
  * Has it like this, they take theyre upper bound and cast it to negative for lower bound
  * Map (-180,180) to (0-180)
  * -180 should be 0, 180 should be 180
  * 
  */ 
  if (f_value < 0){
    temp_f_value = -(f_value);
    temp_f_value = 180 - temp_f_value;
    map_value = temp_f_value * .50;
    f_value = f_value + map_value;
  }else{
    map_value = f_value * .5;
    f_value = f_value + map_value;
  }
  //servo motor can move from 0 to 180 degrees
  if(f_value < 0) {f_value = 0;}
  if(f_value > 174) {f_value = 174;}

  set_servo_angle_one(f_value);

}

void main() {
  // This prevents the need to reset after flashing
  // For some reason without this, sei() will cause resets
  // annoying to find
  USBCON = 0;
  // set yellow to OUTPUT
  DDRC |= (1 << 7);
  int i = 0;
  for (i=0; i<5; i++) {
    blink_yellow();
  }
  // enable power supply to power breadboard
  // using it to power ultrasonic sensor
  // not using breadboard
  DDRB |= ( 1 << DDB7 );
  PORTB |= ( 1 << PORTB7 );

  // // enabling power for potentiometer. Moved for more pin outs
  // DDRE |= (1 << DDE6);
  // PORTE |= (1 << PORTE6);
     
  setup_timer1();
  setup_motor1();
  setup_sonic_sensor();
  //iniatialize potentiometer
  adc_init();
  ////gpio_initialize_led(GPIO_4_GENERAL);//not using anymore, switched white to GPIO_1
  gpio_initialize_led(GPIO_1_GENERAL);//white led
  gpio_initialize_led(GPIO_3_GENERAL);//blue led
  gpio_initialize_led(GPIO_5_GENERAL);//is now red instead of white
  gpio_initialize_led(GPIO_0_GENERAL);//green led now  
  ////gpio_initialize_led(GPIO_13_GENERAL);//switching trigger here to clear up adc channel
 
  //cli();

  //encoder only up to 1000 something
  volatile uint16_t adc_results;
  volatile uint8_t adc_led = adc_read(11);
  volatile int distance_measured = 0;
  volatile uint8_t adc_tune = adc_read(13);
  volatile uint16_t adc_tune_results;

  volatile uint8_t adc_tunekI = adc_read(8);
  volatile uint16_t adc_tunekI_results;

  volatile uint8_t adc_tunekD = adc_read(9);
  volatile uint16_t adc_tunekD_results;

  setupUART();
  _delay_ms(50);
  sei();

  sendString("HERE WE GO!\r\n");

  while(1) {

      //* to display sonic sensor status, ISR being fired * //
      if (sonic_counter > 200){
        led_toggle(&_gpio_three_general);
        sonic_counter = 0;
        _delay_us(10);
      }
      if (user_command_ready) {
		    sendChar('B');
		    handleInput();
	    }

      //* Get sonic reading and apply pid_control function to servo angle *//
      trigger_sonic_sensor();
      distance_measured = sonic_distance;
      pid_control(distance_measured);

      //* read pots for leds and tunning *//
      adc_results = adc_read(11);
      adc_tune_results = adc_read(13);
      adc_tunekI_results = adc_read(8);
      adc_tunekD_results = adc_read(9);
      
      //** Usually wouldnt be moving potent the whole time, so check when substantial change **//
      //** Affects balancing if too much overhead like in check_led_adc **//

      if ( (adc_results > (adc_led + 200)) || (adc_results < (adc_led - 200) )){
        adc_led = adc_results;
        check_led_adc(adc_results);
      }
      if ( (adc_tune > (adc_tune_results + 200)) || (adc_tune < (adc_tune_results - 200) )){
        adc_tune = adc_tune_results;
        tune_kp(adc_tune_results);
      }
      if ( (adc_tunekI > (adc_tunekI_results + 200)) || (adc_tunekI < (adc_tunekI_results - 200) )){
        adc_tunekI  = adc_tunekI_results;
        tune_ki(adc_tunekI_results);
      }
      if ( (adc_tunekD > (adc_tunekD_results + 200)) || (adc_tunekD < (adc_tunekD_results - 200) )){
        adc_tunekD = adc_tunekD_results;
        tune_kd(adc_tunekD_results);
      }
	
      _delay_us(10);

      //sprintf(output_buffer, "%d sonic distance \r\n", sonic_distance);
      //sendString(output_buffer);
      // kP = (adc_tune_results / 100) * .10; 
      // kI = (adc_tunekI_results / 100) * .00001 + .00099;
      // kD = (adc_tunekD_results / 100) * 100 + 5000;
      // sprintf(output_buffer,"TUNE KI: %d\r\n",adc_tune_kI_results);
      // sendString(output_buffer);
      // sprintf(output_buffer,"TUNE kD: %d\r\n",adc_tune_kD_results);
      // sendString(output_buffer);
      // _delay_ms(1000);
      
  } // end while
} // end main

check_led_adc(int adc_results){

  //* WHITE LED *//
  if( (!led_on_one) && (adc_results < 340)){
    led_toggle(&_gpio_one_general);
    led_on_one = 1;
    setPoint = 5;
  }else if (led_on_one && (! (adc_results < 340)) ) {
    led_toggle(&_gpio_one_general);
    led_on_one = 0;
  }

  //* RED LED *//
  if( (!led_on_five) && (adc_results > 340) && (adc_results < 680) ){
    led_toggle(&_gpio_five_general);
    led_on_five = 1;
    setPoint = 12;
  }else if (led_on_five && ( ( adc_results > 680) || (adc_results < 340)  )){
    led_toggle(&_gpio_five_general);
    led_on_five = 0;
  }

  //* GREEN LED *//
  if ( (!led_on_six) && (adc_results > 680) ){
    led_toggle(&_gpio_zero_general);
    led_on_six = 1;
    setPoint = 20;
  }else if (led_on_six && (! (adc_results > 680) ) ){
    led_toggle(&_gpio_zero_general);
    led_on_six = 0;
  }
}

//* Dont want to check pot too often, have to settle with less options for now **//
//* Can add 1 option at a time and see how it affects the PID system **//
tune_kp(int adc_results){

  if (adc_results < 340){
      kP = 0;
      
  }else if ( (adc_results > 340) && (adc_results < 680) ){
      kP = 8.10;

  }else if (adc_results > 680){
      kP = 16.20;
  }
}
tune_ki(int adc_results){

  if (adc_results < 340){
      kI = 0;
      
  }else if ( (adc_results > 340) && (adc_results < 680) ){
      kI = 0.00099;
      //   0.00100
      
  }else if (adc_results > 680){
      kI = .00198;
  }

}

tune_kd(int adc_results){

  if (adc_results < 340){
      kD = 0;
      
  }else if ( (adc_results > 340) && (adc_results < 680) ){
      kD = 5000;
      
  }else if (adc_results > 680){
      kD = 12000;
  }

}