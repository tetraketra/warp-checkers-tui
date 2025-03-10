#ifndef WC_GAME_H
#define WC_GAME_H

#include <ctype.h>

#include "coord.h"
#include "frames/board.h"

#define MAX(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })
#define ABS(a) ({ typeof(a) _a = (a); (_a < 0) ? -_a : _a; })
#define PLAYER_BIT_POS 7
#define CS_PLAYER_FROM(cs) (((cs) & (1 << PLAYER_BIT_POS)) >> PLAYER_BIT_POS)
#define CS_PLAYER_AS(cs, p) ((cs) | ((p) << PLAYER_BIT_POS)) /* Cell state encoded as a P1-or-P2-owned. */
#define CS_NORMALIZE(cs) ((cs) & 0b00001111) /* Cell state without any special info. Can now be compared to `board_state`. */

typedef enum player_t {
    P1,
    P2
} player_t;

typedef enum cell_state_t {
    CS_EMPTY,
    CS_CHECKER,
    CS_KING,
    CS_QUEEN,
    CS_FRAGMENT,
    CS_STACKED_KINGS,
    CS_STACKED_FRAGMENTS,
    _CS_MAX,
} cell_state_t;

#define MESSAGE_BUFFER_LEN_MAX 17
#define COMMAND_BUFFER_LEN_MAX 64
typedef struct game_t {
    cell_state_t cells[8][8];
    player_t player_move;
    int player_moves; /* 1 normally, but splitting can allow more. */
    int player_adv; /* Positive in P1's favor, negative in P2's. */
    char command_buffer[COMMAND_BUFFER_LEN_MAX];
    size_t command_buffer_len;
    char message_buffer[MESSAGE_BUFFER_LEN_MAX];
} game_t;

extern void game_draw(coord_t offset);
extern bool game_parse_command();
extern game_t game_singleton;


#endif 