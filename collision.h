#pragma once
#include "SDL2/SDL.h"
#include "tetromino.h"

bool xCollide(int x1Actual, int x2Actual, int x1All, int x2All);
bool yCollide(int y1Actual, int y2Actual, int y1All, int y2All);
bool collide(SDL_Renderer* renderer, struct blocs *tetrominos, int tetrominoIndex);
