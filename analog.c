#include <avr/io.h>
#include "analog.h"
#include "serial.h"
#include <math.h>
volatile int flag = 0;
//* clear the first 5 bits in ADMUX register. Bit 6 (MUX5) in ADCSRB *//
volatile uint8_t channel_mask = 0b0000;

// initialize adc
void adc_init() {

  // AREF = AVcc
  ADMUX = (1<<REFS0);

  // ADC Enable and prescaler of 128
  // 16000000/128 = 125000
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

}

// read adc value, passing channel as uint8_t 0000 0000 
uint16_t adc_read(uint8_t channel) {
  //* ADMUX --> 10 0000 ADMUX |= channel; Bottom pg. 314 **/
  //* MUX5 in ADCSRB, only manipulating MUX4...0 in ADMUX **/
  //* For our purposes, MUX5 always used because dont have access to channels < 8 *//
  //* Never manipulating MUX4 either from table below, only MUX3...0 *//
  //* Make a mask for first 4 bits, MUX3...0, then clear upon calling again *//
  // channel: (ADMUX |= (channel - 8)) --> What its doing when traced
  // 13 (0000 1101) --> (13-8 =5) (0000 0101) ADMUX --> 00 10 0101 --> Channel 13
  // 12 (0000 1101) --> (12-8 =4) (0000 0100) ADMUX --> 00 10 0100 --> Channel 12
  // 11 (0000 1011) --> (11-8 =3) (0000 0011) ADNUX --> 00 10 0011 --> Channel 11
  // 10 (0000 1010) --> (10-8 =2) (0000 0010) ADMUX --> 00 10 0010 --> Channel 10
  // 9  (0000 1001) --> (9-8  =1) (0000 0001) ADMUX --> 00 10 0001 --> Channel 9
  // 8  (0000 1000) --> (8-8  =0) (0000 0000) ADMUX --> 00 10 0000 --> Channel 8
  // if ever coming back to this, 13 - 8 = 5, does mean to just set bit 5, sets all corresponding bits eg. 1001 == 5
  // I thought line was setting 5th bit only, but actually setting the first 4 bits 1001. 4 bits gives you 0 - 15

  // clear first 4 bits from ADMUX, determines channel
  // first iteration channel_mask always 0b0000
  // second iteration channel_mask == channel we chose from first iteration, ex 13 == 0b0101
  // &= ~() a trick to clear certain bits, we dont care about last 4
  // clear the channel on 2nd iteration if using different channels
  ADMUX &= ~(channel_mask);

  if (channel >= 8) {
    // set MUX5 if greater than channel 8 (8, 9, 10, 11, 12, 13)
    // only ADC channels we have access too
    //* Need to set channels below to use ADC channels 8 - 13 **//
    ADCSRB |= (1<<MUX5); //MUX 5 (ADCSRB), (ADMUX) MUX4,MUX3,MUX2,MUX1,MUX0 

    ////ADMUX |= (channel - 8);

    //* I think doing ADMUX |= (channel - 8)
    //*               channel_mask |= (channel - 8) and then keeping line before if does the same
    //*               Not sure what >0000< 0101, what happens with >....<, 0000, prob because then wouldnt be expected value
    //*               would be the same, but does mask still work the same?
    //*               doesnt matter I guess, but adds overhead by having to check cases

    switch(channel){
      //* For some reason doing it this way changes the potentiometer increase direction **//
      // ^ Depends on where you plug in ground and power, switch to change direction
      //* Forward approaches 0, Backwards approaches 1024) **//
      case 13:
        ADMUX |= 0b0101;
        channel_mask |= 0b0101;
        break;
      case 12:
        ADMUX |= 0b0100;
        channel_mask |= 0b0100;
        break;
      case 11:
        ADMUX |= 0b0011;
        channel_mask |= 0b0011;
        break;
      case 10:
        ADMUX |= 0b0010;
        channel_mask |= 0b0010;
        break;
      case 9:
        ADMUX |= 0b0001;
        channel_mask |= 0b0001;
        break;
      case 8:
        ADMUX |= 0b0000;
        channel_mask |= 0b0000;
        break;
    }
  }
  else {
    // else clear the register, because we are not using these ADC channels
    ADCSRB &= ~(1<<MUX5);
    ADMUX |= channel;
  }

  // PG. 316 of the datasheet
  // start single conversion
  // write '1' to ADSC
  ADCSRA |= (1<<ADSC);

  // wait for conversion to complete
  // ADSC becomes '0' again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));

  return (ADC);
}


