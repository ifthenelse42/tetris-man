#pragma once
#include "SDL2/SDL.h"
#include "game/character.hpp"

class Game::Input {
  public:
    void handle(Game::Tetromino::blocs* tetrominos, int* max, SDL_Event* event, Game::Character::position* position, bool* left, bool* right, bool* run);
};
