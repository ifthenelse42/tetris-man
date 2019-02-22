#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "errorManager.h"
#include "game.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

/*
 * TODO
 */

int main(int argc, char* argv[]) // Ces arguments sont là pour la compatibilité avec les autres OS
{
  initSDL();

  SDL_Window* window = mkWindow();
  SDL_Renderer* renderer = mkRenderer(window);
  checkRendererAndWindow(renderer, window);

  clearRender(renderer);

  initTTFError();
  TTF_Font* verdana = TTF_OpenFont("fonts/verdana.ttf", 14);
  fontLoadError(verdana);

  loop(renderer, window, verdana);

  close(renderer, window, verdana);

  return 0; // Et enfin on retourne 0 pour arrêter le programme
}
