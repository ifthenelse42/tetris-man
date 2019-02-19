#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "handleError.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO: Récupérer les positions d'une texture (genre pour modifier un texte à la même position)
 */

void clearRender(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void displayTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x1, int y1, int x2, int y2)
{
  SDL_Rect position = { x1, y1, x2, y2 };
  SDL_RenderCopy(renderer, texture, NULL, &position);
  SDL_RenderPresent(renderer);
}

void drawLine(SDL_Renderer* renderer, int r, int g, int b, int a, int x1, int y1, int x2, int y2)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  SDL_RenderPresent(renderer);
}

SDL_Texture* mkText(SDL_Renderer* renderer, SDL_Color color, TTF_Font* font, const char* text)
{
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  //SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

  // La surface ne sert plus à rien une fois créée
  SDL_FreeSurface(surface);

  // Par contre la texture sert toujours, donc on renvoie son pointeur
  return texture;
}

SDL_Texture* mkTexture(SDL_Renderer* renderer, SDL_Surface* imgBuffer)
{
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgBuffer);
  SDL_FreeSurface(imgBuffer);

  return texture;
}

void close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

// On défini les tailles de base de la fenêtre, qui sont accessible globalement dans le programme
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) // Ces arguments sont là pour la compatibilité avec les autres OS
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool launch = true;

  handleInitError();

  window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
  handleWindowError(window);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  handleRendererError(renderer);

  SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  if (TTF_Init() < 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  TTF_Font* verdana = TTF_OpenFont("fonts/verdana.ttf", 14);

  if (!verdana) {
    printf("TTF Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // On génère une image
  SDL_Texture* image = mkTexture(renderer, IMG_Load("test.png"));

  if (!image) {
    printf("Error while loading the image. Error : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  // On génère un texte
  const char* texte1Contenu = "issou";
  SDL_Texture* texte1 = mkText(renderer, { 255, 255, 255 }, verdana, texte1Contenu);

  // On affiche les textures
  displayTexture(renderer, image, 100, 100, 100, 100);
  displayTexture(renderer, texte1, 100, 100, 100, 100);

  SDL_Delay(2000);

  // Boucle d'exécution du jeu
  //while(launch) {
  //}

  SDL_Delay(2000);

  close(renderer, window, verdana);

  return 0; // Et enfin on retourne 0 pour arrêter le programme
}
