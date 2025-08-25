#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include "game.h"
#include "rendering.h"

void RenderRunningState(const game_t *game);
void RenderGameOverState(const game_t *game);
void RenderPauseState();

void DrawBoard(const game_t *game, int center_row, int center_col);
void DrawShadowPiece(const game_t *game, int center_row, int center_col);
void DrawFallingPiece(const game_t *game, int center_row, int center_col);
void DrawCachedPiece(const game_t *game, int center_row, int center_col);
void DrawNextPiece(const game_t *game, int center_row, int center_col);

bool render_colors = true;

void RenderGame(const game_t *game){
	switch(game->state){
		case RUNNING_STATE:
			RenderRunningState(game);
			break;
		case PAUSE_STATE:
			RenderPauseState();
			break;
		case GAME_OVER_STATE:
			RenderGameOverState(game);
			break;
	}
}

void RenderRunningState(const game_t *game){
	erase();
	int center_row = getmaxy(stdscr)/2 - ROWS/2;
	int center_col = getmaxx(stdscr)/2 - COLUMNS;
	DrawBoard(game, center_row, center_col);
	DrawShadowPiece(game, center_row, center_col);
	DrawFallingPiece(game, center_row, center_col);
	DrawCachedPiece(game, center_row, center_col);
	DrawNextPiece(game, center_row, center_col);

	for (int i = 0; i < 8; i++){ //this doesn't need to be its own function
		mvaddch(6 + center_row, 2*COLUMNS + 2 + i + center_col, '*');
	}
	mvprintw(center_row, center_col + 2*COLUMNS+2,"%s",game->score_c);
	mvprintw(center_row + 1, center_col + 2*COLUMNS+2,"%s",game->lines_c);
	//refresh();
}

void DrawBoard(const game_t *game, int center_row, int center_col){
	for(int row = 0; row < ROWS; row++){
		for(int col = 0; col < COLUMNS; col++){
			if(game->play_area[row][col] != EMPTY){
				ATTRON(render_colors, game->play_area[row][col]);
				mvaddch(row + center_row, 2*col + center_col, '[');
				mvaddch(row + center_row, 2*col+1 + center_col, ']');
				ATTROFF(render_colors, game->play_area[row][col]);
			}
			else {
				mvaddch(row + center_row, 2*col + center_col, ' ');
				mvaddch(row + center_row, 2*col+1 + center_col, '.');
			}
		}
	}
}

void DrawShadowPiece(const game_t *game, int center_row, int center_col){
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->lowest_piece_row >= 0 && game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
				ATTRON(render_colors, 7);
				mvaddch(row + game->lowest_piece_row + center_row, 2*(col + game->piece_col) + center_col, '(');
				mvaddch(row + game->lowest_piece_row + center_row, 2*(col + game->piece_col) + 1 + center_col, ')');
				ATTROFF(render_colors, 7);
			}
		}
	}
}

void DrawFallingPiece(const game_t *game, int center_row, int center_col){
	ATTRON(render_colors, game->piece_index);
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
				mvaddch(row + game->piece_row + center_row, 2*(col + game->piece_col) + center_col, '[');
				mvaddch(row + game->piece_row + center_row, 2*(col + game->piece_col) + 1 + center_col, ']');
			}
		}
	}
	ATTROFF(render_colors, game->piece_index);
}

void DrawCachedPiece(const game_t *game, int center_row, int center_col){
	if(game->alt_init == false){
		return;
	}
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->tetrominos[game->alt_index][0][row][col] == FALLING_SQUARE){
				ATTRON(render_colors, game->alt_index);
				mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 2 + center_col, '[');
				mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ']');
				ATTROFF(render_colors, game->alt_index);
			}
			else{
				mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 2 + center_col, ' ');
				mvaddch(row + 2 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ' ');
			}
		}
	}
}

void DrawNextPiece(const game_t *game, int center_row, int center_col){
	for(int col = 0; col < 4; col++){
		for(int row = 0; row < 4; row++){
			if(game->tetrominos[game->cached_index[0]][0][row][col] == FALLING_SQUARE){
				ATTRON(render_colors, game->cached_index[0]);
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 2 + center_col, '[');
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ']');
				ATTROFF(render_colors, game->cached_index[0]);
			}
			else{
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 2 + center_col, ' ');
				mvaddch(row + 8 + center_row, 2*col + 2*COLUMNS + 3 + center_col, ' ');
			}
		}
	}
}

void RenderPauseState(){
	erase();
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - 3;
	int center_row = max_row/2 - 1;
	mvprintw(center_row,center_col,"PAUSED");
	refresh();
}

void RenderGameOverState(const game_t *game){
	erase();
	int max_col = getmaxx(stdscr);
	int max_row = getmaxy(stdscr);
	int center_col = max_col/2 - 5;
	int center_row = max_row/2 - 1;
	mvprintw(center_row,center_col,"GAME OVER");
	mvprintw(center_row+1,center_col,"%s", game->score_c);
	mvprintw(center_row+2,center_col,"%s", game->lines_c);
	refresh();
}
