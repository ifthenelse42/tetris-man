#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

SDL_Texture* mkTexture(SDL_Renderer* renderer, SDL_Surface* imgBuffer);

SDL_Texture* mkText(SDL_Renderer* renderer, SDL_Color color, TTF_Font* font, const char* text);

void displayTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height);

void displayText(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

void drawLine(SDL_Renderer* renderer, int r, int g, int b, int a, int x1, int y1, int x2, int y2);
