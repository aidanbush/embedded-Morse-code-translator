
#include <stdio.h>
#include <stdint.h>
#include "codes.h"


int main() {
    uint8_t val;
    char c;
    
    /*for (int i = 0; i < 256; i++) {// able to write over all code
        printf("len:%i, int:%i\n", morseCodes.codes[i][0],
                                   morseCodes.codes[i][1]);
    }*/
    while (scanf("%c",&c) != EOF){
        val = isValidChar(c);
        if (val != 255) {
            printf("len:%i, int:%i\n", morseCodes.codes[val][0],
                                       morseCodes.codes[val][1]);
        }
    }
    return 0;
}


