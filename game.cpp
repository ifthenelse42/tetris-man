#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "collision.h"
#include "errorManager.h"
#include "renderer.h"
#include "tetromino.h"
#include "textureManager.h"
#include <iostream>
#include <vector>

void initSDL()
{
  initSDLError();
}

void loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  bool run = true;

  // On génère un texte
  const char* activeText = "1";
  const char* inactiveText = "0";
  SDL_Texture* active = mkText(renderer, { 0, 0, 0 }, font, activeText);
  SDL_Texture* inactive = mkText(renderer, { 0, 0, 0 }, font, inactiveText);

  SDL_Event e;
  blocs tetrominos[MAX_TETROMINOS];
  blocs* allTetrominos = tetrominos;

  // Créé un premier tetromino
  //tetromino1(renderer, allTetrominos, { 200, -10, BLOC_WIDTH, BLOC_HEIGHT }, 1);
  // ---- nouvelle méthode ----
  blocs2 allBlocs[MAX_TETROMINOS];

  // On rajoute un tetromino de type 1
  addTetromino(allBlocs, 225, -100, 1, 0);
  addTetromino(allBlocs, 225, 100, 2, 0);

  SDL_Texture* bloc = mkBloc(renderer);
  while (run) {
    clearRender(renderer);
    int last = lastIndex(allBlocs);
    for (int i = 0; i < last; i++) {
      collide(allBlocs, i);
    }
    while (SDL_PollEvent(&e) != 0) {
      // Si l'utilisateur demande à fermer la fenêtre du jeu
      if (e.type == SDL_QUIT) {
        run = false;
      }
      // Si l'utilisateur appuis sur une touche
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_DOWN:
          break;
        case SDLK_UP:
          break;
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        }
      }
    }

    affiche(renderer, bloc, allBlocs, active, inactive);
    fall(allBlocs);
    SDL_RenderPresent(renderer);
  }
}

void close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}
