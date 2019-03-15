#include "tetromino.h"
#include "SDL2/SDL.h"
#include "renderer.h"
#include "textureManager.h"
#include <iostream>
#include <math.h>

SDL_Texture* mkBloc(SDL_Renderer* renderer)
{
  SDL_Texture* bloc = mkTetromino(renderer, BLOC_WIDTH, BLOC_HEIGHT);

  // On transforme la texture en un rendu
  SDL_SetRenderTarget(renderer, bloc);

  // Puis on change sa couleur
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderClear(renderer);

  // On ajoute des lignes pour faire jolie
  mkLine(renderer, 0, 0, 0, 255, 0, 0, BLOC_WIDTH, 0);
  mkLine(renderer, 0, 0, 0, 255, 0, BLOC_HEIGHT - 1, BLOC_WIDTH, BLOC_HEIGHT - 1);
  mkLine(renderer, 0, 0, 0, 255, 0, 0, 0, BLOC_HEIGHT - 1);
  mkLine(renderer, 0, 0, 0, 255, BLOC_WIDTH - 1, 0, BLOC_WIDTH - 1, BLOC_HEIGHT - 1);
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
void fall(blocs2* blocs)
{
  for (int k = 0; k < 2; k++) {
    blocs[k].coordinate[1][0] += 1;
    blocs[k].coordinate[1][1] += 1;
  }
}

// Premier type de tetromino - on assemble plusieurs blocs
void tetromino1(SDL_Renderer* renderer, blocs* tetrominos, SDL_Rect position, int rotation)
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

  // On applique une rotation à chaque bloc
  for (int i = 0; i < lastBlocIndex(tetrominos, last); i++) {
    int x = tetrominos[last].position[i].x;
    int y = tetrominos[last].position[i].y;
    std::cout << x << std::endl;

    /*
    // TODO rotation ici - formule de Mme Baert
    tetrominos[last].position[i].x = x + x * cos(M_PI / 2) - y * sin(M_PI / 2);
    tetrominos[last].position[i].y = y + x * sin(M_PI / 2) + y * cos(M_PI / 2);
    */
  }
}

void tetromino2(SDL_Renderer* renderer, blocs* tetrominos, SDL_Rect position, int rotation)
{
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

blocs2 transpose(blocs2 tetromino, int rotation)
{
  blocs2 transposed;

  // On fait une itération par rotation (si 0, la boucle n'est donc pas exécutée et il n'y a pas rotation)
  for (int n = 0; n < rotation; n++) {
    for (int x = 0; x < MAX_TETROMINOS_SIZE; x++) {
      for (int y = 0; y < MAX_TETROMINOS_SIZE; y++) {
        // On fait la transposition en faisant un mirroir au niveau des index y
        transposed.coordinate[y][x] = tetromino.coordinate[x][MAX_TETROMINOS_SIZE - y - 1];
      }
    }
    // On remplace le contenu de tetromino par la transposition - transposed sera ensuite ré-écris avec les nouvelles positions du tetromino
    tetromino = transposed;
  }

  return tetromino;
}

// On cherche si dans la zone d'affichage, il existe des portées de pixels formant un bloc
// Si oui, y afficher le bloc
void affiche(SDL_Renderer* renderer, SDL_Texture* bloc, blocs2* blocs, SDL_Texture* active, SDL_Texture* inactive)
{
  // Pour chaque tetromino
  for (int k = 0; k < 1; k++) {
    // On fouille ses coordonnées, qui est une matrice 6x6
    for (int x = 0; x <= SCREEN_WIDTH; x += 50) {
      for (int y = 0; y <= SCREEN_HEIGHT; y += 50) {
        // Si les coordonnées actuelle correspondent au début d'un tetromino
        if (x == blocs[k].startX && y == blocs[k].startY) {
          // Alors on circule dedans pour construire le tetromino
          for (int xVec = 0; xVec < MAX_TETROMINOS_SIZE; xVec++) {
            for (int yVec = 0; yVec < MAX_TETROMINOS_SIZE; yVec++) {
              if (blocs[k].coordinate[xVec][yVec] == 1)
                displayTexture(renderer, bloc, x + (BLOC_WIDTH * xVec), y + (BLOC_HEIGHT * yVec), BLOC_WIDTH, BLOC_HEIGHT);
            }
          }
        }
      }
    }
  }
}
