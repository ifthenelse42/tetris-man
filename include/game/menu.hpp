#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Game::Menu {
  public:
    struct button {
      int x;
      int y;
      const char* text;
      SDL_Texture* active;
      SDL_Texture* inactive;
      bool selected;
    };

    void init(SDL_Renderer* renderer, Game::Menu::button* main, Game::Menu::button* quit, TTF_Font* menuFont);
    void index(SDL_Renderer* renderer, Game::Menu::button* main, Game::Menu::button* quit, TTF_Font* menuFont);
    void start(bool* launch, bool* game);
};
