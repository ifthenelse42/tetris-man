#include "tetromino.h"
#include "SDL2/SDL.h"
#include "textureManager.h"
#include <iostream>

const int BLOC_WIDTH = 50;
const int BLOC_HEIGHT = 50;

// TODO: Assembler plusieurs blocs dans un tableau
SDL_Texture* mkBloc(SDL_Renderer* renderer)
{
  SDL_Texture* bloc = mkTetromino(renderer, BLOC_WIDTH, BLOC_HEIGHT);

  // On transforme la texture en un rendu
  SDL_SetRenderTarget(renderer, bloc);

  // Puis on change sa couleur
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderClear(renderer);

  // Enfin, on rend le rendu au renderer initial
  SDL_SetRenderTarget(renderer, NULL);
  return bloc;
}

int lastTetrominosIndex(blocs* tetrominos)
{
  int last = 0;

  for (int i = 0; i < MAX_TETROMINOS; i++) {
    if (tetrominos[i].bloc[0] == NULL) {
      last = i;
      break;
    }
  }

  return last;
}

int lastBlocIndex(blocs* tetrominos, int tetrominosIndex)
{
  int last = 0;

  for (int i = 0; i < MAX_TETROMINOS_BLOC; i++) {
    if (tetrominos[tetrominosIndex].bloc[i] == NULL) {
      last = i;
      break;
    }
  }

  return last;
}

// Premier type de tetromino - on assemble plusieurs blocs
void tetromino1(SDL_Renderer* renderer, blocs* tetrominos)
{
  // On forme un bloc
  SDL_Texture* bloc = mkBloc(renderer);

  // On obtiens le dernier index des tetrominos
  int last = lastTetrominosIndex(tetrominos);

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].bloc[0] = bloc;
  tetrominos[last].position[0] = { 100, 50, BLOC_WIDTH, BLOC_HEIGHT };
  tetrominos[last].bloc[1] = bloc;
  tetrominos[last].position[1] = { tetrominos[last].position[0].x, tetrominos[last].position[0].y + 50, BLOC_WIDTH, BLOC_HEIGHT };
  tetrominos[last].bloc[2] = bloc;
  tetrominos[last].position[2] = { tetrominos[last].position[0].x + 50, tetrominos[last].position[1].y, BLOC_WIDTH, BLOC_HEIGHT };
}
