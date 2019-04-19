#pragma once
#include "SDL2/SDL.h"
#include "game/character.hpp"
#include "game/menu.hpp"

class Game::Input {
  public:
    void mouseHover(int* x, int* y, Game::Menu::button* button);
    void mouseTrack(Game::Menu::button* button);
    void keyDown(SDL_Event* event, bool* left, bool* right, bool* jump);
    void keyUp(SDL_Event* event, bool* left, bool* right, bool* jump);
    void quit(SDL_Event* event, bool* run);
    void mouseClick(Game::Menu::button* start, Game::Menu::button* quit, Game::Menu::button* replay, Game::Menu::button* quit2, SDL_Event* event, bool* launch, bool* game, bool* dead, bool* run);
    void handle(Game::Tetromino::blocs* tetrominos, int* max, SDL_Event* event, Game::Character::position* position, bool* left, bool* right, bool* jump, bool* launch, bool* game, bool* dead, bool* run, Game::Menu::button* start, Game::Menu::button* quit, Game::Menu::button* replay, Game::Menu::button* quit2);
};
