#include "game.h"
#include <curses.h>


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


void InitColorPairs(){
	start_color();
	int FULL_BLOCK_COLOR = 1;
	if (FULL_BLOCK_COLOR){
		init_pair(1, 51, 51);     //CYAN
		init_pair(2, 21, 21);     //BLUE
		init_pair(3, 202, 202);    //ORANGE
		init_pair(4, 226, 226);    //YELLOW
		init_pair(5, 46, 46);     //GREEN
		init_pair(6, 129, 129);    //PURPLE
		init_pair(7, 196, 196);    //RED
	}
	else{
		init_pair(1, 51, COLOR_BLACK);     //CYAN
		init_pair(2, 21, COLOR_BLACK);     //BLUE
		init_pair(3, 202, COLOR_BLACK);    //ORANGE
		init_pair(4, 226, COLOR_BLACK);    //YELLOW
		init_pair(5, 46, COLOR_BLACK);     //GREEN
		init_pair(6, 129, COLOR_BLACK);    //PURPLE
		init_pair(7, 196, COLOR_BLACK);    //RED
	}
}

