#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>

bool loop() {
    refresh();
    getch();

    return true;
}

int main(void) {
    setlocale(LC_ALL, "");
    initscr();

    while (loop()) { /* Empty :3 */ }

    return endwin(); /* Returns OK (0) or ERR (-1). */
}