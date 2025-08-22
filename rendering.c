#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include "game.h"
#include "rendering.h"

void RenderRunningState(const game_t *game);
void RenderGameOverState(const game_t *game);
void RenderPauseState(const game_t *game);

int winChange;
bool winChangeInit = false;

void RenderGame(const game_t *game){
	switch(game->state){
		case RUNNING_STATE:
			RenderRunningState(game);
			break;
		case PAUSE_STATE:
			RenderPauseState(game);
			break;
		case GAME_OVER_STATE:
			RenderGameOverState(game);
			break;
	}
}

void RenderRunningState(const game_t *game){
	int max_col = getmaxx(stdscr);
	if(winChangeInit == true){
		if(winChange != max_col){
			clear();
		}
	}
	winChangeInit = true;
	winChange = max_col;
	//int max_row = getmax(stdscr)y;
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - COLUMNS;
	int center_row = max_row/2 - ROWS/2;

	// renders the board
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLUMNS; col++){
			if(game->play_area[row][col] != EMPTY){
				attron(COLOR_PAIR(game->play_area[row][col] + 1));
				mvaddch(row + center_row, 2*col + center_col, '[');
				mvaddch(row + center_row, 2*col+1 + center_col, ']');
				attroff(COLOR_PAIR(game->play_area[row][col] + 1));
			}
			else {
				mvaddch(row + center_row, 2*col + center_col, ' ');
				mvaddch(row + center_row, 2*col+1 + center_col, '.');
			}
		}
	}

	//renders shadow piece
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->lowest_piece_row >= 0 && game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
				mvaddch(row + game->lowest_piece_row + center_row, 2*(col + game->piece_col) + center_col, '(');
				mvaddch(row + game->lowest_piece_row + center_row, 2*(col + game->piece_col) + 1 + center_col, ')');
			}
		}
	}
	//renders the falling piece
	attron(COLOR_PAIR(game->piece_index + 1));
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
				mvaddch(row + game->piece_row + center_row, 2*(col + game->piece_col) + center_col, '[');
				mvaddch(row + game->piece_row + center_row, 2*(col + game->piece_col) + 1 + center_col, ']');
			}
		}
	}
	attroff(COLOR_PAIR(game->piece_index + 1));

	// render RenderMiniPieces
	if(game->alt_init == true){ //Cached Piece
		for(int col = 0; col < 4; col++){
			for(int row = 0; row < 4; row++){
				if(game->tetrominos[game->alt_index][0][row][col] == FALLING_SQUARE){
					attron(COLOR_PAIR(game->alt_index + 1));
					mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 2 + center_col, '[');
					mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ']');
					attroff(COLOR_PAIR(game->alt_index + 1));
				}
				else{
					mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 2 + center_col, ' ');
					mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ' ');
				}
			}
		}
	}

	for (int i = 0; i < 8; i++){
		mvaddch(6 + center_row, 2*COLUMNS + 2 + i + center_col, '*');
	}

	for(int col = 0; col < 4; col++){ //Up Next
		for(int row = 0; row < 4; row++){
			if(game->tetrominos[game->cached_index[0]][0][row][col] == FALLING_SQUARE){
				attron(COLOR_PAIR(game->cached_index[0] + 1));
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 2 + center_col, '[');
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ']');
				attroff(COLOR_PAIR(game->cached_index[0] + 1));
			}
			else{
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 2 + center_col, ' ');
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ' ');
			}
		}
	}

	mvprintw(center_row, center_col + 2*COLUMNS+2,"%s",game->score_c);
	mvprintw(center_row + 1, center_col + 2*COLUMNS+2,"%s",game->lines_c);
}

void RenderPauseState(const game_t *game){
	clear();
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - 3;
	int center_row = max_row/2 - 1;
	mvprintw(center_row,center_col,"PAUSED");
	refresh();
}
void RenderGameOverState(const game_t *game){
	clear();
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - 5;
	int center_row = max_row/2 - 1;
	mvprintw(center_row,center_col,"GAME OVER");
	mvprintw(center_row+1,center_col,"%s", game->score_c);
	mvprintw(center_row+2,center_col,"%s", game->lines_c);
	refresh();
}
