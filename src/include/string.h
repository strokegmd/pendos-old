#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "memory.h"
#include "system.h"

/*
Calculates length of string
*/
size_t strlen(char* str) {
    size_t i = 0;
    while (str[i]) i++;
    return i;
}

/*
Compares two string
*/
bool strcmp(char* s1, char* s2) {
    if (strlen(s1) != strlen(s2)) return false;
    for (size_t i = 0; i < strlen(s1); i++) {
        if (s1[i] != s2[i]) return false;
    }
    return true;
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

/*
Removes first X characters from string
*/
uint8_t* strrem(uint8_t* string, uint16_t amount) {
    return string + amount;
}

/*
Checks if string starts with X
*/
bool strstartswith(uint8_t* string, uint8_t* startwith, uint16_t amount) {
    for (uint16_t i = 0; i < amount; i++) {
        if (string[i] != startwith[i]) return false;
    }
    return true;
}

/*
Compares amount of characters in two strings
*/
bool strncmp(uint8_t* s1, uint8_t* s2, uint16_t amount) {
    for (uint16_t i = 0; i < amount; i++) {
        if (s1[i] != s2[i]) return false;
    }
    return true;
}

#endif
