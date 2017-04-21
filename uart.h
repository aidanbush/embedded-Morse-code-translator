/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 5
 * Description: this file convers works with uart and holds the ring buffer
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

#define MAX_BUFFER_LEN 24

typedef struct ringBuffer_ {
    char chars[MAX_BUFFER_LEN];//actual buffer
    uint8_t len;//length of current buffer
    uint8_t pos;//current position in buffer
} ringBuffer;

/*typedef struct charBuffer_ {
    uint8_t c;
    uint8_t pos;
    uint8_t len;
} charBuffer;
*/

extern ringBuffer buffer;
//extern charBuffer curChar;


void addRingBuff(char);

char retrieveRingBuff();

char checkNext();

char checkLast();

#endif
