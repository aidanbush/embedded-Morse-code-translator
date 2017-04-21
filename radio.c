/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 17
 * Description: this file is used to turn off the radio, and most of the code is
 *  from lab 2
 */

#include <msp430.h>
#include <stdint.h>
#include "radio.h"

//from lab 2
void send_cc1100_bits(uint8_t value, uint8_t bits) { 
  while (bits) {
    RSCLK_LOW;
    if (value & (0x01 << (bits - 1))) {
      RSI_HIGH;
    } else {
      RSI_LOW;
    }

    __delay_cycles(1);

    RSCLK_HIGH;
    __delay_cycles(1);

    bits--;
  }

  RSCLK_LOW;
} 

//from lab 2
void turnOffRadio() {
    //init pins
    INIT_RADIO_PINS;
    //turn off
    CSN_LOW;
    while (P1IN & PIN_RSO);   /* wait for radio's SO to go low */
    send_cc1100_bits(0x39, 8);
    CSN_HIGH;
}

