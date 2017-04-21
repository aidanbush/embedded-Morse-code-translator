/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 5
 * Description: this file deals with converting to morse code as weel as storing
 *  the currently printing character. it also verifies characters and
 *  returning their position in the lookup table.
 * Globals: morseCodes which holds the lookuptable for the morse codes as well
 *  as what is currently printing, and what stage of printing
 */

#include <ctype.h>
#include <stdint.h>
#include "codes.h"


codes morseCodes = {
    .curChar = 0,
    .pos = 0,
    .states = 0,
    .codes = { 
    {2, 2}, // a
    {4, 1}, // b
    {4, 5}, // c
    {3, 1}, // d
    {1, 0}, // e
    {4, 4}, // f
    {3, 3}, // g
    {4, 0}, // h
    {2, 0}, // i
    {4,14}, // j
    {3, 5}, // k
    {4, 2}, // l
    {2, 3}, // m
    {2, 1}, // n
    {3, 7}, // o
    {4, 6}, // p
    {4,11}, // q
    {3, 2}, // r
    {3, 0}, // s
    {1, 1}, // t
    {3, 4}, // u
    {4, 8}, // v
    {3, 6}, // w
    {4, 9}, // x
    {4,13}, // y
    {4, 3}, // z
    {5,31}, // 0
    {5,30}, // 1
    {5,28}, // 2
    {5,24}, // 3
    {5,16}, // 4
    {5, 0}, // 5
    {5, 1}, // 6
    {5, 3}, // 7
    {5, 7}, // 8
    {5,15}, // 9
    {6,42}, // .
    {6,51}, // ,
    {6, 7}, // :
    {6,12}, // ?
    {6,30}, // '
    {6,33}, // -
    {5, 9}, // /
    {5,13}, // (
    {6,45}, // )
    {6,18}, // "
    {5,17}, // =
    {5,10}, // +
    {6,22}, // @
    {5,21}, // start sig
    {6,40}, // end sig
    {0, 0}  // space
    } 
};


/*checks if a character is valid, if so it converts it to the index in the
  struct if its not valid returns -1*/
uint8_t isValidChar(char c) {
    if (isalpha(c)) {
        c = tolower(c);
        return c - ASCII_a_VAL;//converts to index
    } else if (isdigit(c)) {
        return c - ASCII_0_VAL + DIGIT_START_INDEX;//converts to index
    } else {
        switch (c) {
            case '.':
                return DOT_INDEX;
            case ',':
                return COMMA_INDEX;
            case ':':
                return COLON_INDEX;
            case '?':
                return QUESTION_MARK_INDEX;
            case '\'':
                return APOSTROPHE_INDEX;
            case '-':
                return DASH_INDEX;
            case '/':
                return FSLASH_INDEX;
            case '(':
                return LPAREN_INDEX;
            case ')':
                return RPAREN_INDEX;
            case '\"':
                return QUOTE_INDEX;
            case '=':
                return EQUAL_INDEX;
            case '+':
                return PLUS_INDEX;
            case '@':
                return AT_INDEX;
            case ' ':
                return SPACE_INDEX;
            default:
                return INVALID_CHAR;
        }
    }
}


//returns the value of the bit/morse symbol for the current
uint8_t curVal() {
    return ((morseCodes.codes[morseCodes.curChar][1] >> morseCodes.pos) & 1);
}

//returns if it is at the end of the current morse code signal
uint8_t endOfCode() {
    return (morseCodes.pos >= morseCodes.codes[morseCodes.curChar][0]);
}

//returns if there is another character past the current one
uint8_t moreCode() {
    return (morseCodes.pos < (morseCodes.codes[morseCodes.curChar][0] - 1));
}
