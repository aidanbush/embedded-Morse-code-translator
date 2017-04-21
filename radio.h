/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 17
 * Description: this file is used to turn off the radio, and most of the code is
 *  from lab 2
 */

#ifndef RADIO_H
#define RADIO_H

#include <msp430.h>

//from lab 2
#define PIN_RSI     0x01
#define RSI_LOW     do {            \
    P1OUT &= ~(PIN_RSI);            \
  } while (0)
#define RSI_HIGH    do {            \
    P1OUT |= (PIN_RSI);             \
  } while (0)

#define PIN_RSCLK   0x02
#define RSCLK_LOW   do {            \
    P1OUT &= ~(PIN_RSCLK);          \
  } while (0)
#define RSCLK_HIGH  do {            \
    P1OUT |= (PIN_RSCLK);           \
  } while (0)

#define PIN_RSO     0x04

#define PIN_CSN     0x20
#define CSN_LOW     do {            \
    P1OUT &= ~(PIN_CSN);            \
  } while (0)
#define CSN_HIGH    do {            \
    P1OUT |= (PIN_CSN);             \
  } while (0)

#define INIT_RADIO_PINS do {        \
    P1DIR |= PIN_RSI;               \
    P1DIR |= PIN_RSCLK;             \
    P1DIR &= ~(PIN_RSO);            \
    P1DIR |= PIN_CSN;               \
    } while (0)

void turnOffRadio();

#endif
