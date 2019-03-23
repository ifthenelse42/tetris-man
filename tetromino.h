#pragma once
#include "SDL2/SDL.h"
#include <vector>

const int MAX_TETROMINOS_BLOC = 4;
const int MAX_TETROMINOS = 1000;
const int MAX_TETROMINOS_SIZE = 4;
const int BLOC_WIDTH = 25;
const int BLOC_HEIGHT = 25;

struct blocs {
  SDL_Texture* bloc[MAX_TETROMINOS_BLOC] = { NULL };
  SDL_Rect position[MAX_TETROMINOS_BLOC];
  bool move = true;
};
struct blocs2 {
  int startX;
  int startY;
  int type;
  bool move;
  std::vector<std::vector<int>> coordinate = std::vector<std::vector<int>>(MAX_TETROMINOS_SIZE, std::vector<int>(MAX_TETROMINOS_SIZE, 0));
};

SDL_Texture* mkBloc(SDL_Renderer* renderer);
int lastIndex(struct blocs2* tetrominos);
void fall(struct blocs2* tetrominos);
void addTetromino(struct blocs2* tetrominos, int startX, int startY, int type, int rotation);
SDL_Texture* bloc(SDL_Renderer* renderer);
bool rotationAmount(int type, int rotation);
void tetrominoShift(struct blocs2* bloc, int rotation);
void transpose(struct blocs2* tetromino, int rotation);
void affiche(SDL_Renderer* renderer, SDL_Texture* bloc, struct blocs2* blocs, SDL_Texture* active, SDL_Texture* inactive);
