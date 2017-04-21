/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 12
 * Description: this file is the main file and contains all the isr's, and the
 *  main function
 */


#include <msp430.h>
#include <stdint.h>
#include "uart.h"
#include "codes.h"
#include "radio.h"

//led macros
#define LED_GREEN       0x02
#define LED_RED         0x04
#define LED_YELLOW      0x08

//timer macros
#define MS120           3932
#define DOT_DELAY       (MS120)
#define DASH_DELAY      (3 * (DOT_DELAY))
#define MORSE_DELAY     (DOT_DELAY)
#define LETTER_DELAY    (3 * (DOT_DELAY))
#define WORD_DELAY      (7 * (DOT_DELAY))

//init led pins
#define INIT_PINS do {                                  \
        P4DIR = (LED_GREEN) | (LED_RED) | (LED_YELLOW); \
        P4OUT = (LED_GREEN) | (LED_RED) | (LED_YELLOW); \
    } while (0)

//basic led macros
#define LED_TOGGLE(led) do {    \
        P4OUT ^= (led);         \
    } while (0)

#define LED_ON(led) do {        \
        P4OUT &= ~(led);        \
    } while (0)

#define LED_OFF(led) do {       \
        P4OUT |= (led);         \
    } while (0)

__attribute__((interrupt(TIMERB0_VECTOR)))
void timerbISR() {
    LED_OFF(LED_RED);
    
    //if between morse code chars
    if (morseCodes.states & STATE_BET_MORSE) {
        morseCodes.states &= ~(STATE_BET_MORSE);//update betMorse
        TBCCR0 += MORSE_DELAY;//set timer
        return;
    }
    
    if (endOfCode()) {//new char or finished a char
        //if at end and more in buffer
        if ((morseCodes.states & STATE_END) && buffer.len > 0) {
            morseCodes.states &= ~(STATE_END);
            
            morseCodes.states |= STATE_IN_MESS;
            morseCodes.curChar = START_INDEX;//assign start sig to cur sig
            morseCodes.pos = 0;
            //restart timer and interupt immediatly
            TBR = 0;
            TBCCR0 = 1;
            TBCTL |= MC_2;
        } else if (buffer.len <= 0) {//if end of buffer or error w/ len
            if (!(morseCodes.states & STATE_END)) {//if end not set
                morseCodes.states |= STATE_BET_CHARS | STATE_END;
                morseCodes.curChar = END_INDEX;
            } else {
                morseCodes.states &= ~(STATE_IN_MESS | STATE_END);
                TBCTL &= ~MC_3;//turn off timerb isr
                return;
            }
        } else {
            morseCodes.states |= STATE_BET_CHARS;
            morseCodes.curChar = isValidChar(retrieveRingBuff());//get index
            
            if (morseCodes.curChar == SPACE_INDEX) {//check for space
                if (buffer.len > 0) {//if next char exists deal w/ space
                    morseCodes.states |= STATE_SPACE;//set space
                    morseCodes.states &= ~(STATE_BET_CHARS);
                    morseCodes.pos = 0;
                    IE1 |= UTXIE0;//turn on tx isr
                    TBCCR0 += (WORD_DELAY);//set delay
                    return;
                } else { //else end sig
                    morseCodes.states |= STATE_BET_CHARS | STATE_END;
                    morseCodes.curChar = END_INDEX;
                }
            } else if (morseCodes.states & STATE_SPACE) {
                morseCodes.states &= ~(STATE_BET_CHARS);
                morseCodes.states &= ~(STATE_SPACE);
            }
        }
        //reset pos to start
        morseCodes.pos = 0;
    }
    
    //turn on led and set delay
    if (!(morseCodes.states & STATE_BET_CHARS)) {
        LED_ON(LED_RED);
        //if char after set bet Morse
        if (moreCode())
            morseCodes.states |= STATE_BET_MORSE;
        //if dash is to be printed
        if (curVal() == 1)
            TBCCR0 += DASH_DELAY;
        else//else dot or space
            TBCCR0 += DOT_DELAY;
    } else {
        TBCCR0 += LETTER_DELAY;
    }
    
    IE1 |= UTXIE0;//turn on tx inturupt
}


__attribute__((interrupt(USART0TX_VECTOR)))
void uart_tx_isr() {
    static char c;
    
    //if between chars
    if (morseCodes.states & STATE_BET_CHARS) {
        c = ' ';
        morseCodes.states &= ~(STATE_BET_CHARS);
    } else if (morseCodes.states & STATE_SPACE) {//if between words
        c = ' ';
        if (morseCodes.pos == 0) {//if first space add to buffer and return
            morseCodes.pos++;
            U0TXBUF = c;
            return;
        }
    } else {
        c = (curVal() == 1) ? '-' : '.' ;
        morseCodes.pos++;//increment morsecode counter
    }
    U0TXBUF = c;
    
    IE1 &= ~UTXIE0;
}


__attribute__((interrupt(USART0RX_VECTOR)))
void uart_rx_isr() {
    static char c;
    c = U0RXBUF;
    
    if (isValidChar(c) != INVALID_CHAR) {
        //if space and first char or already space at the end
        if (c == ' ')
            if ((buffer.len == 0) || (checkLast() == ' '))
                return;
        
        if (buffer.len == 0) {//first character in buffer
            //if not currently printing set start signal to current
            if (!(morseCodes.states & STATE_IN_MESS)) {
                morseCodes.curChar = START_INDEX;
                morseCodes.states |= STATE_IN_MESS;
                morseCodes.pos = 0;
                //start immediatly
                TBR = 0;
                TBCCR0 = 1;
                TBCTL |= MC_2;
            }
        }
        addRingBuff(c);
    }
}

int main() {
    WDTCTL = WDTPW | WDTHOLD;   /* disable watchdog timer */
    
    turnOffRadio();
    
    /* configure the LEDs */
    INIT_PINS;
    
    __eint();                   /* enable global interrupts */
    
    //set timer b in continuous mode
    TBCCR0 = 1;
    TBCCTL0 = CCIE; /* enable interrupts for TBCCR1-3 */

    TBCTL = TBSSEL_1;          /* select ACLK */
    
    
    /* pins for USART0/UART mode:
       P3.4: transmit data out
       P3.5: transmit data in */
    P3SEL |= 0x30;              /* peripheral mode: in this mode,
                                 * USART module should configure the
                                 * pin directions */

    /* USART initialization/reset procedure from UG 13.2.1 (pg. 13-4):
     * Step #1: set SWRST in U0CTL */
     U0CTL = SWRST;

    /* Step #2: initialize all registers */
    U0CTL |= CHAR;              /* 8 in 8-N-1 */
    U0TCTL |= I2CSSEL_1;        /* ACLK */

    U0BR1 = 0;                  /* configure baud rate; UG (pg. 13-16) */
    U0BR0 = 0x0D;               /* 2400 baud */
    U0MCTL = 0x6B;

    /* Step #3: enable USART via ME1 */
    ME1 |= UTXE0 | URXE0;       /* USART0 TX/RX enable */

    /* Step #4: clear USART software reset (SWRST) via software */
    U0CTL &= ~SWRST;

    /* Step #5: enable interrupts (optional) */
    IE1 |= URXIE0;              /* enable UART receive interrupts */
    
    LPM3;//low power mode
    
    return 0;
}
