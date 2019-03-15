#pragma once
#include "SDL2/SDL.h"
#include <vector>

const int MAX_TETROMINOS_BLOC = 4;
const int MAX_TETROMINOS = 1000;
const int MAX_TETROMINOS_SIZE = 6;
const int BLOC_WIDTH = 50;
const int BLOC_HEIGHT = 50;

struct blocs {
  SDL_Texture* bloc[MAX_TETROMINOS_BLOC] = { NULL };
  SDL_Rect position[MAX_TETROMINOS_BLOC];
  bool move = true;
};
struct blocs2 {
  int startX;
  int startY;
  std::vector<std::vector<int>> coordinate = std::vector<std::vector<int>>(MAX_TETROMINOS_SIZE, std::vector<int>(MAX_TETROMINOS_SIZE, 0));
};

SDL_Texture* mkBloc(SDL_Renderer* renderer);
int lastTetrominosIndex(struct blocs *tetrominos);
int lastBlocIndex(struct blocs *tetrominos, int tetrominosIndex);
void fall(struct blocs2* tetrominos);
void tetromino1(SDL_Renderer* renderer, struct blocs *tetrominos, SDL_Rect position, int rotation);
void tetromino2(SDL_Renderer* renderer, struct blocs *tetrominos, SDL_Rect position, int rotation);
SDL_Texture* bloc(SDL_Renderer* renderer);
blocs2 transpose(struct blocs2 tetromino, int rotation);
void affiche(SDL_Renderer* renderer, SDL_Texture* bloc, struct blocs2* blocs, SDL_Texture* active, SDL_Texture* inactive);
