#pragma once
#include "SDL2/SDL.h"
#include "game.hpp"
#include <vector>

class Game::Character {
  public:
    struct position {
      int x1 = 0;
      int x2 = 0;
      int y1 = 0;
      int y2 = 0;
    };

    void move(SDL_Texture* personnage);
    void display(SDL_Render* renderer, SDL_Texture* personnage);
};
