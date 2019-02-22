#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

int initSDLError();
int initTTFError();
int checkRendererAndWindow(SDL_Renderer* renderer, SDL_Window* window);
int textureLoadError(SDL_Texture* texture);
int fontLoadError(TTF_Font* font);
