#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "errorManager.h"
#include "renderer.h"
#include "tetromino.h"
#include "textureManager.h"
#include <iostream>

void initSDL()
{
  initSDLError();
}

void loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  bool run = true;

  // On génère un texte
  const char* texte1Contenu = "test";
  SDL_Texture* texte1 = mkText(renderer, { 0, 0, 0 }, font, texte1Contenu);

  SDL_Event e;
  blocs tetrominos[MAX_TETROMINOS];
  blocs* allTetrominos = tetrominos;

  // Créé un premier tetromino
  tetromino1(renderer, allTetrominos);

  while (run) {
    int last = lastTetrominosIndex(tetrominos);
    int lastBloc = lastBlocIndex(tetrominos, last - 1);
    clearRender(renderer);
    // On parcours le tableau contenant les tetrominos
    //for (long unsigned int i = 0; i < sizeof(tetrominos) / sizeof(tetrominos[0]); i++) {
    //}
    for (int i = 0; i < last; i++) {
      std::cout << lastBlocIndex(tetrominos, last - 1) << std::endl;
      for (int j = 0; j < lastBloc; j++) {
        displayTexture(renderer, tetrominos[i].bloc[j], tetrominos[i].position[j].x, tetrominos[i].position[j].y, tetrominos[i].position[j].w, tetrominos[i].position[j].h);
      }
    }

    displayText(renderer, texte1, 50, 50);
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
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        }
      }
    }
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
