#include "frame.h"

/*
    Draw a frame, or a too-small message if not enough space.

    @param frame The frame to draw.
    @param offset The offset to draw the frame at.

    @returns Whether the frame drew correctly.

    @note Does not clear the screen before drawing!
    @note Spaces are still printed, which can overwrite 
    background material.
*/
bool frame_draw(frame_t* frame, coord_t offset) {
    /* Shortcut if not enough space to print the frame. */
    size_t x;
    size_t y;
    getmaxyx(stdscr, y, x);

    if (frame->h + offset.y > y || frame->w + offset.x > x) {
        mvprintw(0, 0, "Window too small!\nWidth = %ld\nHeight = %ld", x, y);
        return false; /* Done */ /* Too small! */
    }

    /* Print the frame. */
    size_t frame_arr_offset = 0;
    char buffer[FRAME_T_MAX_ARRAY_LEN];

    for (size_t line = 0; line < frame->h; line++) { // for line 
        memset(buffer, '\0', FRAME_T_MAX_ARRAY_LEN);
        
        for (size_t i = 0; i < FRAME_T_MAX_ARRAY_LEN; i++) { // fetch line 
            buffer[i] = frame->frame[frame_arr_offset + i];
            if (buffer[i] == '\n') {
                mvprintw(offset.y + line, offset.x, (const char*)buffer);
                frame_arr_offset += (++i);
                
                break; // to next line
            }
        }
    }

    return true;
}