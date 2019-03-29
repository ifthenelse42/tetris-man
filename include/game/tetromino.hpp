#pragma once
#include "SDL2/SDL.h"
#include "game.hpp"
#include <vector>

class Game::Tetromino {
  public:
  struct blocs {
    int startX;
    int startY;
    int type = { 0 };
    int rotation = { 0 };
    bool move;
    bool zombie;
    std::vector<std::vector<int>> coordinate = std::vector<std::vector<int>>(maxSize, std::vector<int>(maxSize, 0));
  };
  struct compatible {
    int shiftX;
    int type = { 0 };
    int rotation = { 0 };
  };
  struct spawn {
    int x1;
    int x2;
    bool goesLeft = true;
  };

  static const int max = 100;
  static const int maxInterlock = 6;
  static const int interlockStartY = -400;
  static const int maxBloc = 4;
  static const int maxSize = 4;
  static const int blocWidth = 25;
  static const int blocHeight = 25;

  SDL_Texture* mkBloc(SDL_Renderer* renderer);
  int lastIndex(blocs* tetrominos);
  void fall(blocs* tetrominos);
  void add(blocs* tetrominos, int startX, int startY, int type, int rotation);
  void addRandom(blocs* tetrominos, int tetrominoActual, compatible* interlocks);
  void generateRandom(blocs* tetrominos, compatible* interlocks);
  unsigned int fallingTetrominos(blocs* tetrominos);
  SDL_Texture* bloc(SDL_Renderer* renderer);
  bool rotationAmount(int type, int rotation);
  void shift(blocs* bloc, int rotation);
  void transpose(blocs* tetromino, int rotation);
  void interlock(blocs tetrominos, compatible* interlocks, int index);
  void handleSpawn(blocs tetrominos, spawn* larry);
  void display(SDL_Renderer* renderer, SDL_Texture* bloc, struct blocs* blocs, SDL_Texture* active, SDL_Texture* inactive);
};
