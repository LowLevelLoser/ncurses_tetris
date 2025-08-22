#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"
#include "util.h"

int main(){
	InitCurses();
	game_t game = InitGameData();

	while(game.state != QUIT_STATE){
		doupdate();
		napms(TIME_PER_FRAME_IN_MS);
		RenderGame(&game);
		RunGame(&game, getch());
	}

	endwin();

	return 0;
}
