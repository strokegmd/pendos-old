#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>
#include <stdint.h>

/*
Calculates length of string
*/
size_t strlen(uint8_t* str) {
    size_t i = 0;
    while (str[i]) i++;
    return i;
}

/*
Converts int to ascii
*/
uint8_t* itoa(uint32_t value) {
    static uint8_t buff[33];  

    uint8_t *ptr;    
    ptr = &buff[sizeof(buff) - 1];    
    *ptr = '\0';

    do {
        *--ptr="0123456789abcdef"[value % 10];
        value /= 10;
    } while(value != 0);

    return ptr;
}

#endif
