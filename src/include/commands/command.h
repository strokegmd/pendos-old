#ifndef PENDOS_MAIN_1_COMMAND_H
#define PENDOS_MAIN_1_COMMAND_H

#include <stdint.h>
#include "../drivers/tty.h"
#include "../lib/string.h"

#define COMMANDS_SIZE 32

typedef void (*OnCommandCalled)(char*);

typedef struct {
    char *name;
    OnCommandCalled callback;
} command_t;

void push_command(command_t *command, uint8_t *state);
void handle_commands(uint8_t *input);

uint8_t args_buffer[10][128];
uint8_t args;

command_t commands[COMMANDS_SIZE];
int next_free = 0;

void push_command(command_t *command, uint8_t *state) {
    if (next_free == -1) {
        *state = -1;
        return;
    }

    commands[next_free] = *command;
    *state = 0;

    // change next free address for command
    next_free++;
    if (next_free >= COMMANDS_SIZE) {
        next_free = -1;
    }
}

void get_args(char *input, uint8_t *args_ptr) {
    uint8_t args = 0;
    uint8_t char_pos = 0;

    for (uint16_t i = 0; input[i] != '\0'; ++i) {
        if (input[i] == ' ') {
            continue;
        }

        while (input[i] != ' ' && input[i] != '\0') {
            args_buffer[args][char_pos++] = input[i++];
        }
        args_buffer[args][char_pos] = '\0';
        ++args;
        char_pos = 0;
    }

    *args_ptr = args;
}

void handle_commands(uint8_t *input) {
    if (!input[0]) {
        tty_putchar('\n');
        return;
    }

    uint8_t commands_size = next_free == -1 ? COMMANDS_SIZE : next_free;
    get_args(input, &args);
    for (uint8_t i = 0; i < commands_size; i++) {
        command_t command = commands[i];

        if (strcmp(args_buffer[0], command.name)) {
            command.callback(input);
            return;
        }
    }
    tty_set_color(tty_get_color(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK));
    tty_write_string("Unknown command!\n\n");
}

void display_commands() {
    uint16_t max = next_free == -1 ? COMMANDS_SIZE : next_free;
    for (uint16_t i = 0; i < max; i++) {
        command_t command = commands[i];
    }
}


#endif //PENDOS_MAIN_1_COMMAND_H
