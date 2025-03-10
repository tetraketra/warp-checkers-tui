#ifndef WC_BOARD_H
#define WC_BOARD_H

#include "frame.h"

typedef struct board_t {
    frame_t frame;
    coord_t zone_board_offset;
    coord_t zone_command_offset;
    coord_t zone_move_offset;
    coord_t zone_advantage_offset;
    coord_t zone_message_offset;
} board_t;

extern board_t board_singleton;

#endif