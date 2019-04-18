#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <vector>

class Game::HUD {
  public:
    int score(SDL_Renderer* renderer, TTF_Font* scoreFont, TTF_Font* timeFont, float* time, int* speed);
};
