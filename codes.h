/* Name: Aidan Bush
 * Course: 464
 * Date: Oct 5
 * Description: this file deals with converting to morse code as weel as storing
 * the currently printing character. it also verifies characters and
 * returning their position in the lookup table.
 */

#ifndef CODES_H
#define CODES_H

#include <stdint.h>

//ascii modifiers
#define ASCII_a_VAL         97
#define ASCII_0_VAL         48
#define DIGIT_START_INDEX   26

//punctuation and misc sign signals
#define DOT_INDEX           36
#define COMMA_INDEX         37
#define COLON_INDEX         38
#define QUESTION_MARK_INDEX 39
#define APOSTROPHE_INDEX    40
#define DASH_INDEX          41
#define FSLASH_INDEX        42
#define LPAREN_INDEX        43
#define RPAREN_INDEX        44
#define QUOTE_INDEX         45
#define EQUAL_INDEX         46
#define PLUS_INDEX          47
#define AT_INDEX            48

//special case signals
#define START_INDEX         49
#define END_INDEX           50
#define SPACE_INDEX         51
#define INVALID_CHAR        255

//states defines
#define STATE_IN_MESS       1
#define STATE_BET_CHARS     2
#define STATE_BET_MORSE     4
#define STATE_END           8
#define STATE_SPACE         16

typedef struct codes_ {
    uint8_t curChar;    //index of current char
    uint8_t pos;        //position in current char
    uint8_t states;     //stores information about the current state
    uint8_t codes[52][2];
} codes;

extern codes morseCodes;

uint8_t isValidChar(char c);

uint8_t curVal();

uint8_t endOfCode();

uint8_t moreCode();

#endif
