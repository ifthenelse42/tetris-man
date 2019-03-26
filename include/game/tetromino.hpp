#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include "game.hpp"

class Game::Tetromino {
  public:
  struct blocs {
    int startX;
    int startY;
    int type;
    bool move;
    std::vector<std::vector<int>> coordinate = std::vector<std::vector<int>>(maxSize, std::vector<int>(maxSize, 0));
  };

  static const int max = 1000;
  static const int maxBloc = 4;
  static const int maxSize = 4;
  static const int blocWidth = 25;
  static const int blocHeight = 25;

  SDL_Texture* mkBloc(SDL_Renderer* renderer);
  int lastIndex(struct blocs* tetrominos);
  void fall(struct blocs* tetrominos);
  void add(struct blocs* tetrominos, int startX, int startY, int type, int rotation);
  SDL_Texture* bloc(SDL_Renderer* renderer);
  bool rotationAmount(int type, int rotation);
  void shift(struct blocs* bloc, int rotation);
  void transpose(struct blocs* tetromino, int rotation);
  void display(SDL_Renderer* renderer, SDL_Texture* bloc, struct blocs* blocs, SDL_Texture* active, SDL_Texture* inactive);
};
