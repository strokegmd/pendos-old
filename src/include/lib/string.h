#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "memory.h"
#include "../system.h"
#include "../drivers/tty.h"

// не используй uint8_t вместо char, это разные вещи
// мамут рахал
// кукува ты чо еблан uint8_t это от 0 до 255 как раз для асции таблицы заебись
// а чар это от -127 до +127 мамут рахал твоююю

void strcpy(char *st1, char *st2);
bool strcmp(char *s1, char *s2);
char *itoa(uint32_t value);
inline char* strrem(uint8_t* string, uint16_t amount);
bool strstartswith(char * string, char * startwith, uint16_t amount);
bool strncmp(char *s1, char *s2, uint16_t amount);
char *strtok(char *str,  char *delimeter);
char** splstr(char* str,  char* delim);

/**
 * Returns the length of a null-terminated string.
 *
 * @param str The string to measure.
 * @return The length of the string.
 */
size_t strlen(uint8_t* str) {
    size_t i = 0;
    while (str[i]) i++;
    return i;
}

/**
 * Copies a null-terminated string from one location to another.
 *
 * @param st1 The source string.
 * @param st2 The destination string.
 */
void strcpy(char *st1, char *st2) {
    int i = 0;
    for (i = 0; st1[i]!= '\0'; i++) {
        st2[i] = st1[i];
    }
    st2[i] = '\0';
}

/**
 * Compares two null-terminated strings for equality.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @return true if the strings are equal, false otherwise.
 */
bool strcmp(char* s1, char* s2) {
    if (strlen(s1)!= strlen(s2)) return false;
    for (size_t i = 0; i < strlen(s1); i++) {
        if (s1[i]!= s2[i]) return false;
    }
    return true;
}

/**
 * Converts an unsigned 32-bit integer to a null-terminated string.
 *
 * @param value The integer to convert.
 * @return A pointer to the resulting string.
 */
char *itoa(uint32_t value) {
    static char buff[33];

    char *ptr;
    ptr = &buff[sizeof(buff) - 1];
    *ptr = '\0';

    do {
        *--ptr="0123456789abcdef"[value % 10];
        value /= 10;
    } while(value!= 0);

    return ptr;
}

/**
 * Returns a pointer to a substring of the original string, starting from the specified offset.
 *
 * @param string The original string.
 * @param amount The offset from the beginning of the string.
 * @return A pointer to the substring.
 */
inline char* strrem(uint8_t* string, uint16_t amount) {
    return string + amount;
}

/**
 * Checks if a string starts with a specified prefix.
 *
 * @param string The string to check.
 * @param startwith The prefix to look for.
 * @param amount The length of the prefix.
 * @return true if the string starts with the prefix, false otherwise.
 */
bool strstartswith(char * string, char * startwith, uint16_t amount) {
    for (uint16_t i = 0; i < amount; i++) {
        if (string[i] != startwith[i]) return false;
    }
    return true;
}

/**
 * Compares a specified number of characters from two strings.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @param amount The number of characters to compare.
 * @return true if the characters match, false otherwise.
 */
bool strncmp(char *s1, char *s2, uint16_t amount) {
    for (uint16_t i = 0; i < amount; i++) {
        if (s1[i]!= s2[i]) return false;
    }
    return true;
}

/**
 * Tokenizes a string using a specified delimiter.
 *
 * @param str The string to tokenize.
 * @param delimeter The delimiter to use.
 * @return A pointer to the next token, or NULL if there are no more tokens.
 */
char *strtok(char *str, char *delimeter) {
    static char *save;
    int i = 0;
    if (str!= NULL) {
        save = str;
    }
    if (save == NULL) {
        return NULL;
    }
    while (strcmp(save + i, delimeter)!= 1 && *(save + i)!= '\0') {
        i++;
    }
    if (*(save + i) == '\0') {
        return save;
    }
    save = save + i + strlen(delimeter);
    *(save - strlen(delimeter)) = '\0';
    return save - strlen(delimeter);
}

// ----------------------
// мемориал моим мучениям
// ----------------------

/*
 * Отче наш, Иже еси на небесе́х!
 * Да святится имя Твое, да прии́дет Царствие Твое,
 * да будет воля Твоя, яко на небеси́ и на земли́.
 * Хлеб наш насущный да́ждь нам дне́сь;
 * и оста́ви нам до́лги наша, якоже и мы оставляем должнико́м нашим;
 * и не введи нас во искушение, но изба́ви нас от лукаваго.
 * Яко Твое есть Царство и сила, и слава, Отца, и Сына, и Святаго Духа, ныне и присно, и во веки веков. Аминь.
char** splstr(char* str,  char* delim, uint8_t* num_tokens) {
    char* token = strtok(str, delim);
    uint8_t count = 0;
    char** tokens = (char**)malloc(sizeof(char*));

    tty_write_string(token);
    tty_putchar('\n');

    while (token != NULL) { // чмо
        tokens[count] = (char *)malloc(strlen(token) + 1);
        strcpy(tokens[count], token);
        tty_write_string(tokens[count]);
        tty_putchar('\n');
        count++;
        token = strtok(NULL, delim);
    }

    *num_tokens = count;
    // из-за того что токенов не совсем фиксированное количество, мне пришлось добавить сука реалок
    tokens = (char**)realloc(tokens, sizeof(char*) * (count + 1));
    tokens[count] = NULL;

    return tokens;
}
*/
#endif
