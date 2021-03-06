#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
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
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

  // On améliore aussi la qualité des pixels
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  return 0;
}

/**
 * Fonction: Engine::Error::initMixer
 * -------------------
 * Vérifie si il y a une erreur dans l'initialisation du mixer de SDL2.
 *
 * @return -1 s'il y a une erreur dans l'initialisation du module TTF de SDL, sinon 1
 *
 * @see main.cpp
 */
int Engine::Error::initMixer()
{
	if (Mix_Init(MIX_INIT_MP3) == 0) {
    printf("Erreur détecté lors de l'initialisation de Mix: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }
  
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("Erreur détecté lors de l'initialisation de SDL_mixer: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }

  return 0;
}

/**
 * Fonction: Engine::Error::initIMG
 * -------------------
 * Vérifie si il y a une erreur dans l'initialisation du module IMG de SDL, et renvoie 0 ou 1 en fonction.
 *
 * @return -1 s'il y a une erreur dans l'initialisation du module IMG de SDL, sinon 1
 *
 * @see main.cpp
 */
int Engine::Error::initIMG()
{
  if (TTF_Init() < 0) {
    printf("IMG_Init: %s\n", TTF_GetError());
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
int Engine::Error::musicLoad(Mix_Music* music)
{
  if (music == NULL) {
    printf("Erreur détecté lors du chargement de la musique: %s\n", Mix_GetError());
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
