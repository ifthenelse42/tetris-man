#pragma once
#include "SDL2/SDL.h"

const int SCREEN_WIDTH = 860;
const int SCREEN_HEIGHT = 480;

SDL_Window* mkWindow();
SDL_Renderer* mkRenderer(SDL_Window* window);
void clearRender(SDL_Renderer* renderer);
