#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "errorManager.h"
#include "renderer.h"
#include "tetromino.h"
#include "textureManager.h"

void initSDL()
{
  initSDLError();
}

void loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  bool run = true;
  int issouX = 0;
  int issouY = 0;

  // On génère un texte
  const char* texte1Contenu = "test";
  SDL_Texture* texte1 = mkText(renderer, { 0, 0, 0 }, font, texte1Contenu);

  // On génère un premier tetromino
  SDL_Texture* bloc = bloc1(renderer, 50, 50);

  SDL_Event e;

  int blocX = 0;
  int blocY = 0;
  while (run) {
    if (blocY < 750)
      blocY += 2;
    clearRender(renderer);
    displayText(renderer, texte1, 50, 50);
    //displayTexture(renderer, image, issouX, issouY, 50, 50);
    displayTexture(renderer, bloc, blocX, blocY, 50, 50);
    SDL_RenderPresent(renderer);

    while (SDL_PollEvent(&e) != 0) {
      // Si l'utilisateur demande à fermer la fenêtre du jeu
      if (e.type == SDL_QUIT) {
        run = false;
      }
      // Si l'utilisateur appuis sur une touche
      else if (e.type == SDL_KEYDOWN) {
        printf("y: %d\n", issouY);
        switch (e.key.keysym.sym) {
        case SDLK_UP:
          issouY -= 50;
          break;
        case SDLK_DOWN:
          issouY += 50;
          break;
        case SDLK_LEFT:
          issouX -= 50;
          break;
        case SDLK_RIGHT:
          issouX += 50;
          break;
        }
      }
    }
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
