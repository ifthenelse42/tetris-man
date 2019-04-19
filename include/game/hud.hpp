#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <vector>
#include "game/menu.hpp"

class Game::HUD {
  public:
    int score(SDL_Renderer* renderer, TTF_Font* scoreFont, TTF_Font* timeFont, float* time, int* speed, bool* dead);
    void gameover(SDL_Renderer* renderer, float* deathTime, TTF_Font* deadFont, TTF_Font* bigFont, Game::Menu::button* replay, Game::Menu::button* quit2, int* speed);
};
