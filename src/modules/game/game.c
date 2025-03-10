#include "game.h"

game_t game_singleton = {
    .cells = {
        {CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2)},
        {CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2)},
        {CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2), CS_PLAYER_AS(CS_CHECKER, P2)},
        {CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY},
        {CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY,CS_EMPTY},
        {CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1)},
        {CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1)},
        {CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1), CS_PLAYER_AS(CS_CHECKER, P1)}
    },

    .player_move = P1,
    .player_moves = 1,

    .message_buffer = "-Warp Checkers-"
};

size_t col_to_index(char col) {
    return (size_t)(col - 'a');
}

size_t row_to_index(char row) {
    return (size_t)(row - '1');
}

/*
    Parse the current command buffer to either do nothing 
    (returns `false`) or execute the move (returns `true`).

    @returns `true` if the move happened, `false` otherwise.

    @note Does not clear the command buffer!
*/
bool game_parse_command() {
    if (// simple move or capture
        // "e1 a4" style 
        isalpha(game_singleton.command_buffer[0]) && // e
        isalpha(game_singleton.command_buffer[3]) && // a
        isdigit(game_singleton.command_buffer[1]) && // 1
        isdigit(game_singleton.command_buffer[4])    // 4
    ) { 
        coord_t from = {
            .y = row_to_index(game_singleton.command_buffer[1]), 
            .x = col_to_index(game_singleton.command_buffer[0])
        };

        coord_t to = {
            .y = row_to_index(game_singleton.command_buffer[4]), 
            .x = col_to_index(game_singleton.command_buffer[3])
        };

        /* Must move onto the board. */
        if (from.y > 7 || from.x > 7 || to.y > 7 || to.x > 7 || from.x < 0 || from.y < 0 || to.x < 0 || to.y < 0) {
            memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
            strncpy(game_singleton.message_buffer, "ER: [a-h][1-8]!", MESSAGE_BUFFER_LEN_MAX-1);
            return false;
        }

        int  dist_x = ABS(from.x - to.x);
        int  dist_y = ABS(from.y - to.y);

        cell_state_t from_cell       = CS_NORMALIZE(game_singleton.cells[from.y][from.x]);
        player_t     from_cell_owner = CS_PLAYER_FROM(game_singleton.cells[from.y][from.x]);
        cell_state_t to_cell         = CS_NORMALIZE(game_singleton.cells[to.y][to.x]);

        /* Must move a non-empty. */
        if (from_cell == CS_EMPTY) {
            memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
            strncpy(game_singleton.message_buffer, "ER: Empty move.", MESSAGE_BUFFER_LEN_MAX-1);
            return false;
        }

        /* Must move to an empty. */
        if (from_cell == CS_EMPTY || to_cell != CS_EMPTY) {
            memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
            strncpy(game_singleton.message_buffer, "ER: Occupied.", MESSAGE_BUFFER_LEN_MAX-1);
            return false;
        }

        /* Must own the piece. */
        if (from_cell_owner != game_singleton.player_move) {
            memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
            strncpy(game_singleton.message_buffer, "ER: Unowned.", MESSAGE_BUFFER_LEN_MAX-1);
            return false;
        }

        /* Valid non-warping move or jump. */
        if ((dist_x == 1 || dist_x == 2) && dist_x == dist_y) {
            int dir_y = (to.y > from.y) ? +1 : -1;
            int dir_x = (to.x > from.x) ? +1 : -1;

            /* P1 cannot move checkers/fragments down. P2 cannot move the same up. */
            if ((dir_y == +1 && game_singleton.player_move == P1 && (from_cell == CS_CHECKER || from_cell == CS_FRAGMENT)) || (dir_y == -1 && game_singleton.player_move == P2 && (from_cell == CS_CHECKER || from_cell == CS_FRAGMENT))) {
                memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
                strncpy(game_singleton.message_buffer, "ER: Direction.", MESSAGE_BUFFER_LEN_MAX-1);
                return false;
            }

            /* Move succeeded. */
            if (dist_x == 1) {
                game_singleton.cells[to.y][to.x] = game_singleton.cells[from.y][from.x];
                game_singleton.cells[from.y][from.x] = CS_EMPTY;
                goto move_succeeded;
            }

            /* Valid jump must jump over an enemy piece. */
            if (dist_x == 2) {
                coord_t jump_over = { 
                    .y = from.y + dir_y,
                    .x = from.x + dir_x
                };

                cell_state_t jump_over_cs    = CS_NORMALIZE(game_singleton.cells[jump_over.y][jump_over.x]);
                player_t     jump_over_owner = CS_PLAYER_FROM(game_singleton.cells[jump_over.y][jump_over.x]);

                /* Cannot jump over an empty cell. */
                if (jump_over_cs == CS_EMPTY) {
                    memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
                    strncpy(game_singleton.message_buffer, "ER: Too far.", MESSAGE_BUFFER_LEN_MAX-1);
                    return false;
                }

                /* Cannot jump over a friendly piece. */
                if (jump_over_owner == from_cell_owner) {
                    memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
                    strncpy(game_singleton.message_buffer, "ER: Friend jmp.", MESSAGE_BUFFER_LEN_MAX-1);
                    return false;
                }

                /* Jump succeeded. */
                game_singleton.cells[to.y][to.x] = game_singleton.cells[from.y][from.x];
                game_singleton.cells[jump_over.y][jump_over.x] = CS_EMPTY;
                goto move_succeeded;
            }
        }

        /* Valid warping move or jump. */
        // TODO WARPING MOVES
        // HERE
    } 

    return false;

move_succeeded:

    memset(game_singleton.message_buffer, '\0', MESSAGE_BUFFER_LEN_MAX);
    strncpy(game_singleton.message_buffer, "-Warp Checkers-", MESSAGE_BUFFER_LEN_MAX-1);
    
    game_singleton.player_moves--; // NOTE splitting will fuck with this

    if (game_singleton.player_moves == 0) {
        game_singleton.player_moves = 1; // NOTE splitting will fuck with this
        game_singleton.player_move = (game_singleton.player_move == P1) ? P2 : P1;
    }

    return true;
}

/*
    Prints the current state of the game.

    @param offset Offset to print at (should be same as the baord offset).
*/
void game_draw(coord_t offset) {
    
    /* Move, Adv, & Command */
    mvprintw(
        board_singleton.zone_advantage_offset.y + offset.y,
        board_singleton.zone_advantage_offset.x + offset.x,
        "%+03d",
        game_singleton.player_adv
    );

    mvprintw(
        board_singleton.zone_move_offset.y + offset.y,
        board_singleton.zone_move_offset.x + offset.x,
        "%1d",
        game_singleton.player_move + 1
    );

    mvprintw(
        board_singleton.zone_command_offset.y + offset.y,
        board_singleton.zone_command_offset.x + offset.x,
        "%-10s",
        game_singleton.command_buffer + MAX((int)game_singleton.command_buffer_len - 10, 0)
    );

    /* The board. */
    for (size_t row = 0; row < 8; row++) {
        for (size_t col = 0; col < 8; col++) {
            char signifier[5];

            switch (CS_NORMALIZE(game_singleton.cells[row][col])) {
                case CS_CHECKER:
                    strcpy(signifier, "● ");
                    break;
                case CS_FRAGMENT:
                    strcpy(signifier, "●F");
                    break;
                case CS_KING:
                    strcpy(signifier, "●K");
                    break;
                case CS_QUEEN:
                    strcpy(signifier, "●Q");
                    break;
                case CS_STACKED_KINGS:
                    strcpy(signifier, "2K");
                    break;
                case CS_STACKED_FRAGMENTS:
                    strcpy(signifier, "2Q");
                    break;

                case CS_EMPTY:
                default:
                    strcpy(signifier, "  ");
                    break;
            }

            int colorpair = ((row + col) % 2) + 1 + 2*CS_PLAYER_FROM(game_singleton.cells[row][col]);
            attron(COLOR_PAIR((colorpair))); 

            mvprintw(
                board_singleton.zone_board_offset.y + offset.y + row,
                board_singleton.zone_board_offset.x + offset.x + col*2,
                "%s", 
                signifier
            );

            attroff(COLOR_PAIR((colorpair)));
            
        }
    }

    /* Default message. */
    mvprintw(
        board_singleton.zone_message_offset.y + offset.y, 
        board_singleton.zone_message_offset.x + offset.x,
        "%-16.16s",
        game_singleton.message_buffer
    );
}