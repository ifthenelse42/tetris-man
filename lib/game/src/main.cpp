#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "engine/engine.hpp"
#include "engine/run.hpp"
#include "engine/render.hpp"
#include "engine/error.hpp"
#include "game/game.hpp"
#include <stdio.h>
#include <stdlib.h>

/**
 * Fonction: main
 * ------------------------
 * Fonction principale venant puiser dans toute les classes du jeu pour le faire fonctionner.
 *
 * @param argc Aucune info dessus - mais c'est beau
 * @param argv Tableau dynamique contenant certaines instructions fournis par le système d'exploitation
 *
 * @see Game::Tetromino
 */
int main(int argc, char **argv)
{
  Engine::Run run; 
  Engine::Render render;
  Engine::Error error;
  run.SDL();

  SDL_Window* window = render.createWindow();
  SDL_Renderer* renderer = render.create(window);
  error.rendererAndWindow(renderer, window);

  render.clear(renderer);

  error.initTTF();
  TTF_Font* verdana = TTF_OpenFont("assets/fonts/verdana.ttf", 14);
  error.fontLoad(verdana);

  run.loop(renderer, window, verdana);

  run.close(renderer, window, verdana);

  return 0; // On retourne 0 - pour arrêter le programme
}
