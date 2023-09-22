#include "game.h"

void WriteGameToBuffer(game_t *game, char *buffer){
    // renders the board
    char temp_buffer[ROWS][COLUMNS*3];
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            if(game->play_area[row][col] != EMPTY){
                temp_buffer[row][2*col] = '[';
                temp_buffer[row][2*col + 1] = ']';
            }
            else {
                temp_buffer[row][2*col] = ' ';
                temp_buffer[row][2*col + 1] = '.';
            }
        }
    }
    if (game->state == RUNNING_STATE){
        //renders shadow piece
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                    temp_buffer[row + game->lowest_piece_row][2*(col + game->piece_col)] = '(';
                    temp_buffer[row + game->lowest_piece_row][2*(col + game->piece_col) + 1] = ')';
                }
            }
        }
        //renders the falling piece
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                    temp_buffer[row + game->piece_row][2*(col + game->piece_col)] = '[';
                    temp_buffer[row + game->piece_row][2*(col + game->piece_col) + 1] = ']';
                }
            }
        }
    }
    for (int i = 0; i < ROWS*COLUMNS*3 ; i++) {
        *(buffer + i) = temp_buffer[i/ROWS][i%(3*COLUMNS)];
    }

}
