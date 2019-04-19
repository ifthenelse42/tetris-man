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
  error.initMixer();
  error.initIMG();
  TTF_Font* scoreFont = TTF_OpenFont("assets/fonts/verdana.ttf", 16);
  TTF_Font* deadFont = TTF_OpenFont("assets/fonts/verdana.ttf", 56);
  TTF_Font* bigFont = TTF_OpenFont("assets/fonts/verdana.ttf", 64);
  TTF_Font* timeFont = TTF_OpenFont("assets/fonts/verdana.ttf", 10);
  TTF_Font* menuFont = TTF_OpenFont("assets/fonts/verdana.ttf", 36);
  error.fontLoad(menuFont);
  error.fontLoad(scoreFont);
  error.fontLoad(timeFont);

  run.loop(renderer, window, menuFont, deadFont, bigFont, scoreFont, timeFont);

  run.close(renderer, window, menuFont, deadFont, bigFont, scoreFont, timeFont);

  return 0; // On retourne 0 - pour arrêter le programme
}
