#include "error.h"

static char errmsg[ERROR_DESCRIPTION_LEN_MAX];

/*
    Set the error message.

    @param msg The message to print.
*/
void errmsg_set(char* msg) {
    memset(errmsg, '\0', ERROR_DESCRIPTION_LEN_MAX);
    strncpy(errmsg, msg, ERROR_DESCRIPTION_LEN_MAX - 1);
    if (errmsg[0] != '\0') {
        for (size_t i = 1; i < ERROR_DESCRIPTION_LEN_MAX - 2; i++) {
            if (errmsg[i] == '\0') {
                errmsg[i] = '\n';
                break;
            }
        }
    }
}

/*
    Gets the error message.

    @param msg The message to print.
*/
char* errmsg_get(void) {
    return errmsg;
}

/*
    Prints the error message to `stdout`.
    Encouraged to use with `atexit()`.
*/
void errmsg_print(void) {
    fputs((const char*)errmsg, stdout);
}