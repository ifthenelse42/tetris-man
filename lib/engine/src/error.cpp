#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "engine/engine.hpp"
#include "engine/error.hpp"

/**
 * Fonction: Engine::Error::initSDL
 * -------------------
 * Vérifie si il y a une erreur dans l'initialisation de SDL, et renvoie 0 ou 1 en fonction.
 *
 * @return -1 s'il y a une erreur dans l'initialisation de SDL, sinon 1
 *
 * @see main.cpp
 */
int Engine::Error::initSDL()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Erreur détecté lors de l'initialisation de SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

/**
 * Fonction: Engine::Error::initTTF
 * -------------------
 * Vérifie si il y a une erreur dans l'initialisation du module TTF de SDL, et renvoie 0 ou 1 en fonction.
 *
 * @return -1 s'il y a une erreur dans l'initialisation du module TTF de SDL, sinon 1
 *
 * @see main.cpp
 */
int Engine::Error::initTTF()
{
  if (TTF_Init() < 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

/**
 * Fonction: Engine::Error::rendererAndWindow
 * -------------------
 * Vérifie si le rendu et la fenêtre ont bien été créés.
 *
 * @return -1 s'il y a une erreur dans l'une des deux créations, 1 sinon
 *
 * @see main.cpp
 */
int Engine::Error::rendererAndWindow(SDL_Renderer* renderer, SDL_Window* window)
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

/**
 * Fonction: Engine::Error::textureLoad
 * -------------------
 * Vérifie si le chargement de la texture donnée s'est déroulée avec succès.
 *
 * @return -1 s'il y a une erreur dans le chargement de la texture, 1 sinon
 *
 * @see Engine::Texture
 */
int Engine::Error::textureLoad(SDL_Texture* texture)
{
  if (!texture) {
    printf("Error while loading texture. Error : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

/**
 * Fonction: Engine::Error::textureLoad
 * -------------------
 * Vérifie si le chargement de la texture donnée s'est déroulée avec succès.
 *
 * @return -1 s'il y a une erreur dans le chargement de la texture, 1 sinon
 *
 * @see Engine::Texture
 */
int Engine::Error::surfaceGeneration(SDL_Surface* surface)
{
  if (!surface) {
    printf("Error while generating surface. Error : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

/**
 * Fonction: Engine::Error::fontLoad
 * -------------------
 * Vérifie si le chargement de la police d'écriture donnée s'est déroulée avec succès.
 *
 * @return -1 s'il y a une erreur dans le chargement de la police d'écriture, 1 sinon
 *
 * @see main.cpp
 */
int Engine::Error::fontLoad(TTF_Font* font)
{
  if (!font) {
    printf("TTF Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}
