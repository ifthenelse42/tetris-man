#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Game::Menu {
  public:
    void index(SDL_Renderer* renderer, TTF_Font* menuFont);
};
