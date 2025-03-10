#include "board.h"

board_t board_singleton = {
    .frame = {
        .h = 17,
        .w = 23,
        .frame = \
            "╔═════════════════════╗\n" \
            "║                     ║\n" \
            "╠═════════════════════╣\n" \
            "║   a b c d e f g h   ║\n" \
            "║ 1                   ║\n" \
            "║ 2                   ║\n" \
            "║ 3                   ║\n" \
            "║ 4                   ║\n" \
            "║ 5                   ║\n" \
            "║ 6                   ║\n" \
            "║ 7                   ║\n" \
            "║ 8                   ║\n" \
            "║ Move:P     Adv:     ║\n" \
            "║ Command:            ║\n" \
            "╠═════════════════════╣\n" \
            "║ r↵:rules    q↵:quit ║\n" \
            "╚═════════════════════╝\n"
    },
    .zone_board_offset      = { .y = 4,  .x = 4  },
    .zone_command_offset    = { .y = 13, .x = 10 },
    .zone_advantage_offset  = { .y = 12, .x = 17 },
    .zone_move_offset       = { .y = 12, .x = 8  },
    .zone_message_offset    = { .y = 1,  .x = 4  }
};