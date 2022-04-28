#include "HCSR04.h"

volatile uint64_t sonic_counter = 0;
extern volatile int sonic_distance = 0;

void setup_sonic_sensor(){

    //set trig to output (1) in Data Direction Register
    trig_ddrx |= (1 << trig_ddr_pin);
    //set echo to input (0) in Data Direction Register
    echo_ddrx &= ~(1 << echo_ddr_pin);
    _delay_ms(50);

    //Setting interrupt to happen on rising edge, (echo pin), when echo goes high
    //eg. echo pin (0 --> 1)
    interrupt_logic_register |= (1 << ISC10);
    interrupt_logic_register |= (1 << ISC11);
    //enable interrupt
    EIMSK |= (1 << interrupt_n);

}
void trigger_sonic_sensor(){
    //https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
    trig_port |= (1 << trig_pin);
    //triggering trigger pin for 15us
    //mininum time needed for trigger is 10us.
    _delay_us(15);
    trig_port &= ~(1 << trig_pin);
    //echo goes high after done sending sonic burst (trigger_pin = 0)
    //give time for echo pin to go high, and jump into interrupt
    _delay_us(15);
    sonic_distance = pulse;
}
int get_sonic_reading(){

    return sonic_distance;
}
ISR(INT1_vect){
    //starts internal counter of microcontroller
    //TCCR1B |= (1 << CS10);
    TCCR3B |= (1 << CS30);
    while (PIND & (1 << PIND1) ){
        //wait until echo pin is off, eg. goes from high to low
        //sendString("waiting for pulse\r\n");
    }
    //turn off timer
    //TCCR1B = 0; //timer 1
    TCCR3B = 0; //timer 3
    //tutorial online uses a 1MHZ crystal, which is inline with 1_000_000 microseconds in a second
    //which is why they do not use a prescaler for the timer
    //using a prescaler just means to count 1 for every 8/16/128/256 clock ticks
    //theres no 16 prescaler, so just divide by 16, not sure if this is the same?
    //then you can still divide by 58
    //pulse = TCNT1 / 16;//14.1 P 111 //timer 1
    pulse = (int)((TCNT3 / 16) / 58 + 1.3); //timer 3, adding .5 for offset
    //TCNT1 = 0;
    TCNT3 = 0;//reset counter timer.
    sonic_counter++;
    //sendString("doneISR\r\n");
    //resetting the counter memory
}
