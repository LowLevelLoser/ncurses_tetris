#include <curses.h>
#include <ncurses.h>
#include "game.h"
#include "rendering.h"
#include "util.h"

void RenderRunningState(const game_t *game);
void RenderGameOverState(const game_t *game);
void RenderPauseState(const game_t *game);

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
#if 1
    // renders the board
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            if(game->play_area[row][col] != EMPTY){
                mvaddch(row, 2*col, '[');
                mvaddch(row, 2*col+1, ']');
            }
            else {
                mvaddch(row, 2*col, ' ');
                mvaddch(row, 2*col+1, '.');
            }
        }
    }
    
    //renders shadow piece
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                mvaddch(row + game->lowest_piece_row, 2*(col + game->piece_col), '(');
                mvaddch(row + game->lowest_piece_row, 2*(col + game->piece_col) + 1, ')');
            }
        }
    }
    //renders the falling piece
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                mvaddch(row + game->piece_row, 2*(col + game->piece_col), '[');
                mvaddch(row + game->piece_row, 2*(col + game->piece_col) + 1, ']');
            }
        }
    }

    for (int i = 0; i < ROWS; i++){
        mvaddch(i, 2*COLUMNS + 1, '|');
    }
    // render RenderMiniPieces
    if(game->alt_init == true){
        for(int col = 0; col < 4; col++){
            for(int row = 0; row < 4; row++){
                if(game->tetrominos[game->alt_index][0][row][col] == FALLING_SQUARE){
                    mvaddch(row + 2, 2*col + 2*COLUMNS + 3, '[');
                    mvaddch(row + 2, 2*col + 2*COLUMNS + 4, ']');
                }
                else{
                    mvaddch(row + 2, 2*col + 2*COLUMNS + 3, ' ');
                    mvaddch(row + 2, 2*col + 2*COLUMNS + 4, ' ');
                }
            }
        }
    }

    for (int i = 0; i < 8; i++){
        mvaddch(6, 2*COLUMNS + 2 + i, '*');
    }

    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->cached_index[0]][0][row][col] == FALLING_SQUARE){
                mvaddch(row + 8, 2*col + 2*COLUMNS + 3, '[');
                mvaddch(row + 8, 2*col + 2*COLUMNS + 4, ']');
            }
            else{
                mvaddch(row + 8, 2*col + 2*COLUMNS + 3, ' ');
                mvaddch(row + 8, 2*col + 2*COLUMNS + 4, ' ');
            }
        }
    }

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
    mvprintw(0,0,"PAUSED");
}
void RenderGameOverState(const game_t *game){
    mvprintw(0,0,"GAME OVER \nscore: %d\nlines cleared: %s", game->score, game->lines_c);
    //DrawText("GAME OVER", SCREEN_WIDTH/3-160, SCREEN_HEIGHT/3+20, 100, BLACK);
    //DrawText(game->score_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+20, 40, BLACK);
    //DrawText(game->lines_c, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+60,40, BLACK);
}
