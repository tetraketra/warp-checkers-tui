#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>

#include "modules/error/error.h"

int main(void) {
    atexit(errmsg_print); // print the currently-set error message for feedback
    atexit((void(*)(void))endwin); // ncurses teardown
    
    /* ncurses setup */
    setlocale(LC_ALL, "");
    initscr();
    errmsg_wrap(refresh(), ERR, errmsg_loc"Failed to refresh the screen.");
    errmsg_wrap(raw(), ERR, errmsg_loc"Failed to switch to raw input mode.");
    errmsg_wrap(noecho(), ERR, errmsg_loc"Failed to disable echo.");
    errmsg_wrap(halfdelay(1), ERR, errmsg_loc"Failed to set halfdelay."); // block for 1/10th of a second on getwchar

    /* loop */
    wchar_t input_wchar;
    char    input_cstr[MB_CUR_MAX];
    int     input_cstr_len;
    bool    input;
    while (true) {
        input_wchar = (wchar_t)getwchar();
        input = input_wchar != ERR;
        if (input) {
            memset(input_cstr, 0, MB_CUR_MAX);
            input_cstr_len = wctomb(input_cstr, input_wchar);

            // TESTING FIXME DELETEME
            errmsg_wrap(input_cstr[0], 'q', "LMAO"); // TESTING DELETE THIS LINE
            printw((const char*)input_cstr); // TESTING DELETE THIS LINE
        }

        refresh();
    }

    exit(EXIT_SUCCESS);
}