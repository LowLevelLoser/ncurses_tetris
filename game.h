#ifndef GAME_H
#define GAME_H

#include <curses.h>
#include <ncurses.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>    // for nanosleep

#define KEY_DELAY 72
#define ROWS 20
#define COLUMNS 10
#define TIME_TO_FALL 400
#define MIN_TIME_TO_FALL 20
#define TIME_PER_FRAME_IN_MS 18

#define RUNNING_STATE 0
#define GAME_OVER_STATE 1
#define PAUSE_STATE 2
#define QUIT_STATE 3

#define EMPTY -1
#define FALLING_SQUARE 1

#define e EMPTY
#define F FALLING_SQUARE

#define CLEAR_BAR COLUMNS

#define FULL_BLOCK_COLOR true

#define ATTRON(cond, idx) \
	do { if (cond) attron(COLOR_PAIR((idx) + 1)); } while (0)

#define ATTROFF(cond, idx) \
	do { if (cond) attroff(COLOR_PAIR((idx) + 1)); } while (0)

enum piece_t{
	I_PIECE,
	LEFT_L_PIECE,
	RIGHT_L_PIECE,
	SQUARE_PIECE,
	S_PIECE,
	T_PIECE,
	Z_PIECE,
};

typedef struct {
	int play_area[ROWS][COLUMNS];
	char play_area_buffer[ROWS][COLUMNS*2];
	const int tetrominos[7][4][4][4];
	enum piece_t cached_index[2];
	enum piece_t piece_index;
	int piece_col;
	int piece_row;
	int lowest_piece_row;
	int tet_rotation;
	enum piece_t alt_index;

	int score;
	int line_cleared;
	int state;

	char score_c[50];
	char lines_c[50];

	bool alt_init;
	bool game_init;
} game_t;
#endif
