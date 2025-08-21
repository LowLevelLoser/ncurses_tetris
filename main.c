#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"
#include "util.h"

int main(){
	// NCURSES initialization:
	initscr();             // initialize curses
	if(has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	//if (can_change_color()){
	//	init_color(COLOR_WHITE, 1000, 647, 0); //replace white with orange
	//}
	InitColorPairs();
	//start_color();

	cbreak();              // pass key presses to program, but not signals
	noecho();              // don't echo key presses to screen
	keypad(stdscr, TRUE);  // allow arrow keys
	timeout(0);            // no blocking on getch()
	curs_set(0);           // set the cursor to invisible
	nodelay(stdscr, TRUE); // Set non-blocking input

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
		//.piece_color_index = {CYAN, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED},
		.tetrominos =
#include "TetrominoDef.txt"
	};

	memset(game.play_area, EMPTY, sizeof(game.play_area));
	//char buffer1[ROWS][COLUMNS*2];
	//char buffer2[ROWS][COLUMNS*2];
	//char (*current_buffer)[20] = buffer1;
	//char (*next_buffer)[20] = buffer2;
	while(game.state != QUIT_STATE){
		//SleepInMilliseconds(TIME_PER_FRAME_IN_MS);
		//signal(SIGWINCH, Clear);
		doupdate();
		napms(TIME_PER_FRAME_IN_MS);
		RenderGame(&game);
		RunGame(&game, getch());
		//WriteGameToBuffer(&game, game.play_area_buffer[0]);
		//clear();
	}
	//StopMusicStream(music);

	//CloseAudioDevice();

	endwin();

	return 0;
}
