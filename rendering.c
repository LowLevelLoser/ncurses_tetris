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
//wresize
//resizeterm

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

#if 1
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

	//for (int i = 0; i < ROWS; i++){
	//    mvaddch(i + center_row, 2*COLUMNS + 1 + center_col, '|');
	//}
	// render RenderMiniPieces
	if(game->alt_init == true){
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

	for(int col = 0; col < 4; col++){
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
	//render score and lines;
	//DrawText(game->score_c, GAME_SCREEN_WIDTH+10, 780, 30, BLACK);
	//DrawText(game->lines_c, GAME_SCREEN_WIDTH+10, 810, 30, BLACK);

#else
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLUMNS * 2; col++){
			mvaddch(row, col, game->play_area_buffer[row][col]);
		}
	}

#endif
}

void RenderPauseState(const game_t *game){
	//DrawText("PAUSED", SCREEN_WIDTH/3-60, SCREEN_HEIGHT/3+20, 100, GRAY);
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - COLUMNS;
	int center_row = max_row/2 - ROWS/2;
	mvprintw(center_row,center_col,"PAUSED");
}
void RenderGameOverState(const game_t *game){
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - COLUMNS;
	int center_row = max_row/2 - ROWS/2;
	mvprintw(center_row,center_col,"GAME OVER");
	mvprintw(center_row+1,center_col,"%s", game->score_c);
	mvprintw(center_row+2,center_col,"%s", game->lines_c);
	//DrawText("GAME OVER", SCREEN_WIDTH/3-160, SCREEN_HEIGHT/3+20, 100, BLACK);
	//DrawText(game->score_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+20, 40, BLACK);
	//DrawText(game->lines_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+60,40, BLACK);
}
