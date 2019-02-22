#pragma once
#include "SDL2/SDL.h"

SDL_Window* mkWindow();
SDL_Renderer* mkRenderer(SDL_Window* window);
void clearRender(SDL_Renderer* renderer);
