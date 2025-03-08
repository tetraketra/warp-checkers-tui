#ifndef WC_ERROR_H
#define WC_ERROR_H

#include <string.h>
#include <stdio.h>

#define ERROR_DESCRIPTION_LEN_MAX 1024

extern void  errmsg_set(char* msg);
extern char* errmsg_get(void);
extern void  errmsg_print(void);

/*
    If `func` evaluates to `errval`, sets `errmsg` to `msg` then exits.

    @param func The function to run (with arguments!).
    @param msg The accompanying `errmsg` if the function errors.
    @param errval The failure state for `func`.

    @note This is a macro so you can call `wrefresh(WINDOW)` 
    or similar as your function if you want.
*/
#define errmsg_wrap(func, errval, msg) do { if ((func) == (errval)) { errmsg_set((msg)); exit(EXIT_FAILURE); } } while (0)

/*
    File and line in "[file:line]" format as a string literal.
    See note for expected usage.

    @note
    ```c
    errmsg_wrap(func(), errmsg_loc"Failed to [whatever].");
    ```
*/
#define errmsg_loc "["_em_stringify(__FILE__)":"_em_stringify(__LINE__)"]"
#define __em_stringify(x) #x
#define _em_stringify(x) __em_stringify(x)

#endif