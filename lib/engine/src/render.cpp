#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/render.hpp"

/**
 * Fonction: Engine::Render::create
 * -------------------
 * Créé un rendu SDL
 * C'est là où sera affecté tout ce que l'on veut afficher à l'utilisateur
 *
 * @param window Fenêtre où affecté le rendu
 * @return Pointeur vers le rendu créé
 */
SDL_Renderer* Engine::Render::create(SDL_Window* window)
{
  return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
}

/**
 * Fonction: Engine::Render::createWindow
 * -------------------
 * Créé une fenêtre, où sera affecté le rendu
 *
 * @return Pointeur vers la fenêtre créée
 */
SDL_Window* Engine::Render::createWindow()
{
  return SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Render::width, Render::height, SDL_WINDOW_RESIZABLE);
}

/**
 * Fonction: Engine::Render::clear
 * -------------------
 * Rafraichis l'affichage
 * Ca permet d'afficher les éléments nouvellements ajoutés / modifiés dans le rendu
 *
 * @see Game::Tetromino::display
 */
void Engine::Render::clear(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
  SDL_RenderClear(renderer);
}
