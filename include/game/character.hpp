#pragma once
#include "SDL2/SDL.h"
#include "game.hpp"
#include "tetromino.hpp"
#include <vector>

class Game::Character {
  public:
    struct position {
      int x = 0;
      int y = 0;
      int width = 0;
      int height = 0;
      int velocityX = 0;
      int velocityY = 1;
    };

    SDL_Texture* create(SDL_Renderer* renderer, SDL_Surface* image, position* position);
    void display(SDL_Renderer* renderer, SDL_Texture* personnage, position* position);
    void handleVelocity(Game::Tetromino::blocs* tetrominos, int* max, position* position, bool* run);
    void movement(position* position);
    void moveLeft(position* personnage);
    void moveRight(position* personnage);
    void moveUp(position* personnage);
    void allUp(position* personnage, int amount);
};
