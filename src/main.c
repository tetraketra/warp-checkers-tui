#include <ncursesw/curses.h>
#include <locale.h>
#include <stdlib.h>

#include "modules/error/error.h"
#include "modules/frames/board.h"
#include "modules/game/game.h"
#include "modules/game/colors.h"

bool draw(bool drawn, coord_t offset) {
    if (!drawn) {
        drawn = frame_draw(&board_singleton.frame, offset);
    }

    if (drawn) {
        game_draw(offset);
    }

    refresh();

    return drawn;
}

coord_t calc_offset(void) {
    coord_t offset = {0};

    int x;
    int y;
    getmaxyx(stdscr, y, x);
    offset = (coord_t){ .y = MAX(y/2 - (int)board_singleton.frame.h/2, 0), .x = MAX(x/2 - (int)board_singleton.frame.w/2, 0)};
    
    return offset;
}

int main(void) {
    atexit(errmsg_print); // print the currently-set error message for feedback
    atexit((void(*)(void))endwin); // ncurses teardown
    
    /* ncurses setup */
    setlocale(LC_ALL, "");
    initscr();
    errmsg_wrap(start_color(), ERR, errmsg_loc"Failed to start color.");
    errmsg_wrap(raw(), ERR, errmsg_loc"Failed to switch to raw input mode.");
    errmsg_wrap(noecho(), ERR, errmsg_loc"Failed to disable echo.");
    errmsg_wrap(cbreak(), ERR, errmsg_loc"Failed to set halfdelay."); // block for 1/10th of a second on getwchar
    errmsg_wrap(curs_set(0), ERR, errmsg_loc"Failed to make the cursor invisible.");
    errmsg_wrap(keypad(stdscr, true), ERR, errmsg_loc"Failed to enable keypad support.");
    errmsg_wrap(refresh(), ERR, errmsg_loc"Failed to refresh the screen.");
    errmsg_wrap(init_color(COLOR_GREY, 300, 300, 300), ERR, errmsg_loc"Failed to initialize color " _em_stringify(COLOR_GREY) ". You might be using a terminal with only 0-8 colors.");
    errmsg_wrap(init_pair(P2_WHITE_PAIR, COLOR_RED, COLOR_WHITE), ERR, errmsg_loc"Failed to add color pair " _em_stringify(P2_WHITE_PAIR) ".");
    errmsg_wrap(init_pair(P2_BLACK_PAIR, COLOR_RED, COLOR_GREY), ERR, errmsg_loc"Failed to add color pair " _em_stringify(P2_BLACK_PAIR) ".");
    errmsg_wrap(init_pair(P1_WHITE_PAIR, COLOR_BLUE, COLOR_WHITE), ERR, errmsg_loc"Failed to add color pair " _em_stringify(P1_WHITE_PAIR) ".");
    errmsg_wrap(init_pair(P1_BLACK_PAIR, COLOR_BLUE, COLOR_GREY), ERR, errmsg_loc"Failed to add color pair " _em_stringify(P1_BLACK_PAIR) ".");
    
    errmsg_wrap(refresh(), ERR, errmsg_loc"Failed to refresh the screen.");

    /* loop */
    coord_t offset = calc_offset();
    int     input_char;
    bool    input;
    bool    drawn = draw(drawn, offset);

    while (true) {
        input_char = getch();
        input = (input_char != ERR);

        if (input) { // the switch is the only thing in this if block
        switch (input_char) {
            case KEY_RESIZE: {
                offset = calc_offset();
                drawn = false;
                clear();
                break;
            }

            case KEY_DC:
            case KEY_DL:
            case KEY_SDC:
            case KEY_SDL:
            case KEY_BACKSPACE:
            case 127:
            case '\b': {
                if (game_singleton.command_buffer_len > 0) {
                    game_singleton.command_buffer[--game_singleton.command_buffer_len] = '\0';
                }
                
                break;
            }

            case ' ':
            case '0'...'9':
            case 'a'...'z':
            case 'A'...'Z': {
                if (game_singleton.command_buffer_len < COMMAND_BUFFER_LEN_MAX - 1) {
                    game_singleton.command_buffer[game_singleton.command_buffer_len++] = (char)input_char;
                }
                
                break;
            }

            case '\n':
            case '\r':
            case KEY_ENTER: {

                /* Parse. */
                if (game_singleton.command_buffer[0] == 'q') {
                    exit(EXIT_SUCCESS);
                } else if (game_singleton.command_buffer[0] == 'r') {
                    // TODO
                } else {
                    if (!game_parse_command()) break;
                }

                /* Reset. */
                memset(game_singleton.command_buffer, '\0', COMMAND_BUFFER_LEN_MAX);
                game_singleton.command_buffer_len = 0;
                break;
            }

            default:
                break;
        }
        }

        drawn = draw(drawn, offset);
    }

    exit(EXIT_SUCCESS);
}