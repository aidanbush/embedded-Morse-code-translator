/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 5
 * Description: this file convers works with uart and holds the ring buffer
 */

#include "uart.h"

ringBuffer buffer = {
    .len = 0,
    .pos = 0
};

void addRingBuff (char c) {
    if (buffer.len == MAX_BUFFER_LEN)
        return;
    buffer.chars[((buffer.pos + buffer.len) % MAX_BUFFER_LEN)] = c;
    buffer.len++;
}

//removes the next char in the buffer, does not check if there is an element
char retrieveRingBuff () {
    char c = buffer.chars[buffer.pos++];//retrieve char
    buffer.pos %= MAX_BUFFER_LEN;//make sure pos is in the correct spot
    buffer.len--;
    return c;
}

//returns next char if exists and returns it else returns NULL
char checkNext () {
    if (buffer.len > 0)
        return buffer.chars[(buffer.pos + 1) % MAX_BUFFER_LEN];
    return 0;
}

//returns the last element in the buffer
char checkLast () {
    return buffer.chars[(buffer.pos + buffer.len - 1) % MAX_BUFFER_LEN];
}
