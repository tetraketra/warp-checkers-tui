#ifndef WC_FRAME_H
#define WC_FRAME_H

#include <stddef.h>
#include <string.h>
#include <ncursesw/curses.h>
#include "coord.h"

#define FRAME_T_MAX_ARRAY_LEN 2048

typedef struct frame_t {
    size_t w; /* Visual width.  */
    size_t h; /* Visual height. */
    char frame[FRAME_T_MAX_ARRAY_LEN]; /* Flat. Rows are '\n'-terminated. Array is '\0'-terminated. */
} frame_t;

extern bool frame_draw(frame_t* frame, coord_t offset);

#endif 