#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Engine::Texture {
  public:
  SDL_Texture* create(SDL_Renderer* renderer, int width, int height);
  SDL_Texture* createFromImage(SDL_Renderer* renderer, SDL_Surface* imgBuffer);
  SDL_Texture* createBloc(SDL_Renderer* renderer, SDL_Color color);
  SDL_Texture* createTetromino(SDL_Renderer* renderer, int x, int y);
  SDL_Texture* createText(SDL_Renderer* renderer, SDL_Color color, TTF_Font* font, const char* text);
  void createLine(SDL_Renderer* renderer, int r, int g, int b, int a, int x1, int y1, int x2, int y2);
  void display(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height);
  void displayImage(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);
};
