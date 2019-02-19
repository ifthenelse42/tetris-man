#include "SDL2/SDL.h"

int handleInitError()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Erreur détecté lors de l'initialisation de SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int handleWindowError(SDL_Window* window)
{
  if (window == NULL) {
    printf("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int handleRendererError(SDL_Renderer* renderer)
{
  if (renderer == NULL) {
    printf("Erreur lors de la création du moteur de rendu : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}
