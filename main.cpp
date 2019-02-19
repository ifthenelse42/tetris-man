#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include "handleError.h"

void drawLine(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawLine(renderer, 100, 100, 400, 150);
  SDL_RenderPresent(renderer);
}

// On défini les tailles de base de la fenêtre, qui sont accessible globalement dans le programme
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) // Ces arguments sont là pour la compatibilité avec les autres OS
{
  SDL_Window* window;
  SDL_Renderer* renderer;

  handleInitError();

  window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
  handleWindowError(window);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  handleRendererError(renderer);

  SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  // On dessine une ligne pour test
  drawLine(renderer, 255, 255, 255, 255);

  SDL_Delay(2000);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0; // Et enfin on retourne 0 pour arrêter le programme
}
