#include "SDL2/SDL.h"
#include <stdio.h>

// On défini les tailles de base de la fenêtre, qui sont accessible globalement dans le programme
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) // Ces arguments sont là pour la compatibilité avec les autres OS
{
  SDL_Window *window = NULL; // la fenêtre de base est égale à rien
  SDL_Surface *screenSurface = NULL; // idem il n'y a pas de surface si la fenêtre n'existe pas

  if (SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL_Init est égal à -1 si SDL n'a pas réussi à s'initialiser
    printf("Failed to initialized SDL. SDL_Error : %s\n", SDL_GetError()); // printf viens de stdio, c'est plus safe d'utiliser stdio que iostream
  } else {
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // Si tout va bien on créé la fenêtre, les deux premiers arguments sont les positions x et y de la fenêtre à l'apparition, et puisqu'on s'en fou on envoi des constantes faites pour

    if (window == NULL) { // Si la fenêtre n'existe toujours pas, il y a eu une erreur
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()); // SDL_GetError() permet d'obtenir l'erreur de SDL
    } else {
      screenSurface = SDL_GetWindowSurface(window); // Si la fenêtre existe, on récupère sa surface
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF)); // On dessine un rectangle qui recouvre toute la surface de la fenêtre
      SDL_UpdateWindowSurface(window); // On met à jour la surface de la fenêtre avec le contenu, cad le rectangle
      SDL_Delay(2000); // On attend 2 secondes avant de passer à la suite, pour voir que la fenêtre est bien lancée
    }
  }

  SDL_DestroyWindow(window); // On détruit la fenêtre
  SDL_Quit(); // On quitte la librairie SDL

  return 0; // Et enfin on retourne 0 pour arrêter le programme
}
