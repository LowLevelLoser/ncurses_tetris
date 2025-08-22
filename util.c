#include "game.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

void InitColorPairs(){
	start_color();
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

void InitCurses(){
	initscr();
	if (has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

	InitColorPairs();

	cbreak();              // pass key presses to program, but not signals
	noecho();              // don't echo key presses to screen
	keypad(stdscr, TRUE);  // allow arrow keys
	timeout(0);            // no blocking on getch()
	curs_set(0);           // set the cursor to invisible
	nodelay(stdscr, TRUE); // Set non-blocking input
}

game_t InitGameData(){
	srand(time(NULL));
	int r = rand() % 7;
	game_t game = {
		.tet_rotation = 0,
		.piece_index = r,
		.score = 0,
		.cached_index = {0},
		.alt_index = 0,
		.alt_init = false,
		.game_init = false,
		.line_cleared = 0,
		.piece_col = 3,
		.piece_row = 0,
		.lowest_piece_row = 0,
		.state = RUNNING_STATE,
		.score_c = "score: 0",
		.lines_c = "lines: 0",
		.tetrominos =
		#include "TetrominoDef.h"
	};

	memset(game.play_area, EMPTY, sizeof(game.play_area));
	return game;
}
