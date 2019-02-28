#include "tetromino.h"
#include "SDL2/SDL.h"
#include "textureManager.h"
#include <iostream>

SDL_Texture* mkBloc(SDL_Renderer* renderer)
{
  SDL_Texture* bloc = mkTetromino(renderer, BLOC_WIDTH, BLOC_HEIGHT);

  // On transforme la texture en un rendu
  SDL_SetRenderTarget(renderer, bloc);

  // Puis on change sa couleur
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderClear(renderer);

  // On ajoute des lignes pour faire jolie
  drawLine(renderer, 0, 0, 0, 255, 0, 0, BLOC_WIDTH, 0);
  drawLine(renderer, 0, 0, 0, 255, 0, BLOC_HEIGHT - 1, BLOC_WIDTH, BLOC_HEIGHT - 1);
  drawLine(renderer, 0, 0, 0, 255, 0, 0, 0, BLOC_HEIGHT - 1);
  drawLine(renderer, 0, 0, 0, 255, BLOC_WIDTH - 1, 0, BLOC_WIDTH - 1, BLOC_HEIGHT - 1);
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

// On fait tomber tous les tetrominos qui sont en état de mouvement
void fall(blocs* tetrominos)
{
  for (int i = 0; i < MAX_TETROMINOS; i++) {
    for (int j = 0; j < MAX_TETROMINOS_BLOC; j++) {
      if (tetrominos[i].move)
        tetrominos[i].position[j].y += 2;
    }
  }
}

// Premier type de tetromino - on assemble plusieurs blocs
void tetromino1(SDL_Renderer* renderer, blocs* tetrominos, SDL_Rect position)
{
  std::cout << "test" << std::endl;
  // On forme un bloc
  SDL_Texture* bloc = mkBloc(renderer);

  // On obtiens le dernier index des tetrominos
  int last = lastTetrominosIndex(tetrominos);

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].bloc[0] = bloc;
  tetrominos[last].position[0] = position;
  tetrominos[last].bloc[1] = bloc;
  tetrominos[last].position[1] = { tetrominos[last].position[0].x, tetrominos[last].position[0].y + BLOC_HEIGHT, BLOC_WIDTH, BLOC_HEIGHT };
  tetrominos[last].bloc[2] = bloc;
  tetrominos[last].position[2] = { tetrominos[last].position[0].x + BLOC_WIDTH, tetrominos[last].position[1].y, BLOC_WIDTH, BLOC_HEIGHT };
}

void tetromino2(SDL_Renderer* renderer, blocs* tetrominos, SDL_Rect position)
{
  std::cout << "test" << std::endl;
  // On forme un bloc
  SDL_Texture* bloc = mkBloc(renderer);

  // On obtiens le dernier index des tetrominos
  int last = lastTetrominosIndex(tetrominos);

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].bloc[0] = bloc;
  tetrominos[last].position[0] = position;
  tetrominos[last].bloc[1] = bloc;
  tetrominos[last].position[1] = { tetrominos[last].position[0].x, tetrominos[last].position[0].y + BLOC_HEIGHT, BLOC_WIDTH, BLOC_HEIGHT };
  tetrominos[last].bloc[2] = bloc;
  tetrominos[last].position[2] = { tetrominos[last].position[0].x, tetrominos[last].position[1].y + BLOC_HEIGHT, BLOC_WIDTH, BLOC_HEIGHT };
}
