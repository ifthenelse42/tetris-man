#pragma once
#include "SDL2/SDL.h"

const int MAX_TETROMINOS_BLOC = 6;
const int MAX_TETROMINOS = 1000;

struct blocs {
  SDL_Texture* bloc[MAX_TETROMINOS_BLOC] = { NULL };
  SDL_Rect position[MAX_TETROMINOS_BLOC];
};

SDL_Texture* mkBloc(SDL_Renderer* renderer);
int lastTetrominosIndex(struct blocs *tetrominos);
int lastBlocIndex(struct blocs *tetrominos, int tetrominosIndex);
void tetromino1(SDL_Renderer* renderer, struct blocs *tetrominos);
