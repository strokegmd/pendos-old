#ifndef __ECHO_H
#define __ECHO_H

void handle_echo(char* buffer) {
    tty_write_string(strrem(buffer, 5));
    tty_write_string("\n\n");
}

#endif
