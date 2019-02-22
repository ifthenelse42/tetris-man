#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

int initSDLError()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Erreur détecté lors de l'initialisation de SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int initTTFError()
{
  if (TTF_Init() < 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int checkRendererAndWindow(SDL_Renderer* renderer, SDL_Window* window)
{
  if (renderer == NULL) {
    printf("Erreur lors de la création du moteur de rendu : %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  if (window == NULL) {
    printf("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int textureLoadError(SDL_Texture* texture)
{
  if (!texture) {
    printf("Error while loading texture. Error : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int surfaceGenerationError(SDL_Surface* surface)
{
  if (!surface) {
    printf("Error while generating surface. Error : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

int fontLoadError(TTF_Font* font)
{
  if (!font) {
    printf("TTF Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}
