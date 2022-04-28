#include "leds.h"
//adding this line, to be able to use some useful functions
//examples like TOGGLE_BIT
#include "common.h"

/* Scope is limited to this file. Used to indicate if LED is initialized. */
static uint8_t bYellowInit = 0;
static uint8_t bRedInit = 0;
static uint8_t bGreenInit = 0;


/* configure the data direction for the specified on-board led.
 */
void configure_led(IO_struct * color) {

  SET_BIT(*color->ddr, color->pin);
}

void initialize_led(int color) {
  switch(color) {
    case (YELLOW) :
    _yellow = (IO_struct) { &DDRC, &PORTC, YELLOW, &PINC };
    configure_led(&_yellow);
    bYellowInit = 1;
    break;

    case(GREEN):
    _green = (IO_struct) { &DDRD, &PORTD, GREEN, &PIND };
    configure_led(&_green);
    bGreenInit = 1;
    break;

    case(RED):
    _red = (IO_struct) { &DDRB, &PORTB, RED, &PINB };
    configure_led(&_red);
    bRedInit = 1;
    break;
  }
}


void led_on(IO_struct * color, int inverted) {
  // define this. (look at flash_led for inspiration)
    
    // if this is not the greeen led, which is inverted
    // meaning it needs a 0 to turn on the led
    // this if/else structure is to turn on the bit to 1
    // and to turn off the bit as 0
    
    //adding this test.
    if (!inverted) {
        SET_BIT(*color->port, color->pin);

    } else {
        CLEAR_BIT(*color->port, color->pin);
    }
}

void led_off(IO_struct * color, int inverted) {
  // define this.
    
    // to turn off the led we want to clear the bit, if not inverted
    // else we send the led the comman to turn on if inverted
    
    //adding this test.
    
    if (!inverted) {
        CLEAR_BIT(*color->port, color->pin);
    } else {
        SET_BIT(*color->port, color->pin);
    }
    
}

void led_toggle(IO_struct * color) {
    
    //added include "common.h" file to be able to use TOGGLE_Bit
    //think working correctly
    
    TOGGLE_BIT(*color->port, color->pin);
    
}

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_led(IO_struct * color, int inverted) {
  if (!inverted) {
    SET_BIT(*color->port, color->pin);
  } else {
    CLEAR_BIT(*color->port, color->pin);
  }
  _delay_ms(250);
  TOGGLE_BIT(*color->port, color->pin);
  _delay_ms(250);
}

/* Flash all the initialized leds for a sanity check light show */
void light_show() {
  int i;
  for (i = 0; i < 2; i++) {
    if (bYellowInit) flash_led(&_yellow, 0);  // not inverted (1 turns led on)
    if (bRedInit) flash_led(&_red, 1);  // inverted (0 turns led on)
    if (bGreenInit) flash_led(&_green, 1);
  }
}
