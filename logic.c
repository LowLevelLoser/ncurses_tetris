#include <curses.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "logic.h"

void MoveLeft(game_t *game);
void MoveRight(game_t *game);
void MoveDown(game_t *game);
void Spin(game_t *game);
void Drop(game_t *game);
void Pause(game_t *game);
void SwitchPiece(game_t *game);

bool Overlaps(game_t *game);
bool Collides(game_t *game);
int FindFullLine(game_t *game);
int GetLowestPossibleRow(game_t *game);
void SettlePiece(game_t *game);
void ResetGame(game_t *game);
void NextPiece(game_t *game);
void ResetPlacement(game_t *game);
void ClearFullLines(game_t *game);
bool EdgeCorrect(game_t *game);

bool settled = false;
bool canSwitch = true;
float fallTime = 0;
float timeToFall = TIME_TO_FALL;

void RunGame(game_t *game, int key_pressed){
    static float left_time = KEY_DELAY;
    static float right_time = KEY_DELAY;
    static float down_time = KEY_DELAY;
    if(game->state == RUNNING_STATE){
        fallTime += TIME_PER_FRAME_IN_MS;
        if(game->game_init == false){
            for(int i = 0; i < 1; i++){
                srand(rand());
                game->cached_index[i] = rand()%7;
            }
            game->game_init = true;
        }

        if(fallTime >= timeToFall){
            MoveDown(game);
        }

        if(key_pressed == (KEY_LEFT)){
            if(left_time >= KEY_DELAY){
                MoveLeft(game);
                left_time = 0;
            }
        }
        else {
            left_time = KEY_DELAY;
        }
        if(key_pressed == (KEY_RIGHT)){
            if(right_time >= KEY_DELAY){
                MoveRight(game);
                right_time = 0;
            }
        }
        else {
            right_time = KEY_DELAY;
        }
        if(key_pressed == (KEY_DOWN)){
            if(down_time >= KEY_DELAY){
                MoveDown(game);
                down_time = 0;
            }
        }
        else {
            down_time = KEY_DELAY;
        }

        switch(key_pressed){
            case 'q':
                game->state = QUIT_STATE;
                break;
             case KEY_UP:
                Spin(game);
                break;
            case ' ':
                Drop(game);
                break;
            case 'c':
                SwitchPiece(game);
                break;
            case 'r':
                ResetGame(game);
                break;
            case 27/*escape*/:
                game->state = PAUSE_STATE;
                clear();
                break;
        }
        left_time += TIME_PER_FRAME_IN_MS;
        right_time += TIME_PER_FRAME_IN_MS;
        down_time += TIME_PER_FRAME_IN_MS;

        game->lowest_piece_row = GetLowestPossibleRow(game);
    }

    else{
        switch(key_pressed){
            case 'q':
                game->state = QUIT_STATE;
                break;
            case 'r':
                ResetGame(game);
                break;
            case 27/*escape*/:
                if(game->state  == PAUSE_STATE){
                    game->state = RUNNING_STATE;
                }
                break;
        }
    }
}

int GetLowestPossibleRow(game_t *game){
    int row_c = game->piece_row;
    int lowest_row = row_c;
    while(Collides(game) == false){
        game->piece_row++;
    }
    lowest_row = game->piece_row - 1;
    game->piece_row = row_c;
    return lowest_row;
}

void MoveLeft(game_t *game){
    game->piece_col--;
    if(Collides(game) == true){
        game->piece_col++;
    }
}

void MoveRight(game_t *game){
    game->piece_col++;
    if(Collides(game) == true){
        game->piece_col--;
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= KEY_DELAY - 1;
    }
    game->piece_row--;
}

void MoveDown(game_t *game){
    fallTime = 0;
    game->piece_row++;
    if(Collides(game) == true){
        game->piece_row--;
        SettlePiece(game);
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= KEY_DELAY - 1;
    }
    game->piece_row--;
}

void Spin(game_t *game) {
    if(game->piece_row == -1){
        game->piece_row++;
    }
    int b = game->tet_rotation;
    game->tet_rotation = (game->tet_rotation+1)%4;
    if(Collides(game) == true){
        if (EdgeCorrect(game) == true){
            game->tet_rotation = b;
        }
    }
    game->piece_row++;
    if(Collides(game) == true){
        fallTime -= 10;
    }
    game->piece_row--;
}


void Drop(game_t *game){
    settled = false;
    while(settled == false){
        MoveDown(game);
        game->score += 1;
    }
    snprintf(game->score_c, 50, "score: %d", game->score);
}

void SwitchPiece(game_t *game){
    if (canSwitch == true){
        enum piece_t b = game->piece_index;

        if(game->alt_init == true){
            game->piece_index = game->alt_index;
            game->alt_index = b;
        }
        else{
            game->alt_index = b;
            game->alt_init = true;
            game->piece_index = game->cached_index[0];
            NextPiece(game);
        }
        ResetPlacement(game);
        canSwitch = false;
    }
}

bool Collides(game_t *game){
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE){
                if(game->piece_col+col >= COLUMNS || game->piece_col+col < 0){
                    return true;
                }
                else if(game->piece_row+row >= ROWS){
                    return true;
                }
            }
        }
    }
    return Overlaps(game);
}

bool EdgeCorrect(game_t *game){
    for(int i = 0; i < 2; i++){
        if(Collides(game) == true){
            game->piece_col--;
        }
    }
    if(Collides(game) == true){
        game->piece_col = game->piece_col + 2;
    }

    for(int i = 0; i < 2; i++){
        if(Collides(game) == true){
            game->piece_col++;
        }
    }
    if(Collides(game) == true){
        game->piece_col = game->piece_col - 2;
    }

    if(Collides(game) == true){
        game->piece_row--;
    }

    if(Collides(game) == true){
        game->piece_row++;
    }

    //reset_overlaps();
    return Collides(game);
}

bool Overlaps(game_t *game){
    for(int col = 0; col < 4; col++){
        for(int row = 0; row < 4; row++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE
               && game->play_area[game->piece_row+row][game->piece_col+col] !=  EMPTY){
                return true;
            }
        }
    }
    return false;
}

void SettlePiece(game_t *game) {
    if(game->piece_row <= 0){
        game->state = GAME_OVER_STATE;
        clear();
    }
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (game->tetrominos[game->piece_index][game->tet_rotation][row][col] == FALLING_SQUARE)
                game->play_area[game->piece_row + row][game->piece_col + col] = game->piece_index;
        }
    }
    settled = true;
    NextPiece(game);
    ResetPlacement(game);
    ClearFullLines(game);
    canSwitch = true;
}

void ClearFullLines(game_t *game){
    static int temp_lines = 0;
    int line = FindFullLine(game);
    if(line == -1){
        game->line_cleared += temp_lines;
        game->score += 100*temp_lines*temp_lines;
        temp_lines = 0;
        snprintf(game->score_c, 50, "score: %d", game->score);
        snprintf(game->lines_c, 50, "lines: %d", game->line_cleared);
        return;
    }

    for (int col = 0; col < COLUMNS; col++){
        game->play_area[0][col] = EMPTY;
    }

    if(line > 0){
        for(int col = 0; col < COLUMNS; col++)
            for(int row = line; row > 0; row--){
                game->play_area[row][col] = game->play_area[row-1][col];
            }
        temp_lines++;
    }

    ClearFullLines(game);
    if (timeToFall > MIN_TIME_TO_FALL){
        timeToFall = TIME_TO_FALL - 2*game->line_cleared;
    }
}

int FindFullLine(game_t *game){
    int total = 0;
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            total += (game->play_area[row][col] != EMPTY);
        }
        if(total == CLEAR_BAR){
            return row;
        }
        total = 0;
    }
    total = 0;
    return -1;
}

void NextPiece(game_t *game){
    srand(rand());
    int r = rand();
    game->piece_index = game->cached_index[0];
    if(r == game->cached_index[1]){
        r = (r+1)%7;
    }
    game->cached_index[1] = r % 7;
    game->cached_index[0] = game->cached_index[1];
}

void ResetPlacement(game_t *game){
    switch(game->piece_index){
        case I_PIECE:
            game->piece_row = -1;
            break;
        default:
            game->piece_row = 0;
    }
    game->tet_rotation = 0;
    game->piece_col = 3;
    fallTime = 0;
}

void ResetGame(game_t *game){
    for(int i = 0; i < 1; i++){
        srand(rand());
        game->cached_index[i] = rand()%7;
    }
    memset(game->play_area, EMPTY, sizeof(int)*ROWS*COLUMNS);
    game->state = RUNNING_STATE;
    game->score = 0;
    game->line_cleared = 0;
    strcpy(game->score_c, "score: 0");
    strcpy(game->lines_c, "lines: 0");
    game->alt_init = false;
    canSwitch = true;
    timeToFall = TIME_TO_FALL;
    ResetPlacement(game);
    clear();
}
