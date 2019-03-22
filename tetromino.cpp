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

int lastTetrominosIndex(blocs2* tetrominos)
{
  int last = 0;

  for (int i = 0; i < MAX_TETROMINOS; i++) {
    // Si le type est défini à 0, ca veut dire que ca ne correspond à aucun tetromino
    if (tetrominos[i].type == 0) {
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
  for (int k = 0; k < lastTetrominosIndex(blocs); k++) {
    if (blocs[k].move) {
      blocs[k].startY += 2;
    }
  }
}

// Tetromino - type L
void addTetromino(blocs2* tetrominos, int startX, int startY, int type, int rotation)
{
  // On obtiens le dernier index des tetrominos
  int last = lastTetrominosIndex(tetrominos);

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].startX = startX;
  tetrominos[last].startY = startY;
  tetrominos[last].type = type;
  tetrominos[last].move = true;

  // La matrice bi-dimentionnelle représentant le tetromino dépend de son type
  switch (type) {
    // tetromino L
  case 1:
    tetrominos[last].coordinate[0][0] = 1;
    tetrominos[last].coordinate[0][1] = 1;
    tetrominos[last].coordinate[0][2] = 1;
    tetrominos[last].coordinate[1][2] = 1;
    break;
    // tetromino I
  case 2:
    tetrominos[last].coordinate[0][0] = 1;
    tetrominos[last].coordinate[0][1] = 1;
    tetrominos[last].coordinate[0][2] = 1;
    tetrominos[last].coordinate[0][3] = 1;
    break;
    // tetromino O
  case 3:
    tetrominos[last].coordinate[0][0] = 1;
    tetrominos[last].coordinate[0][1] = 1;
    tetrominos[last].coordinate[1][0] = 1;
    tetrominos[last].coordinate[1][1] = 1;
    break;
  // tetromino s
  case 4:
    tetrominos[last].coordinate[0][1] = 1;
    tetrominos[last].coordinate[1][0] = 1;
    tetrominos[last].coordinate[1][1] = 1;
    tetrominos[last].coordinate[2][0] = 1;
    break;
  // tetromino z
  case 5:
    tetrominos[last].coordinate[0][0] = 1;
    tetrominos[last].coordinate[1][0] = 1;
    tetrominos[last].coordinate[1][1] = 1;
    tetrominos[last].coordinate[2][1] = 1;
    break;
  // tetromino T
  case 6:
    tetrominos[last].coordinate[0][0] = 1;
    tetrominos[last].coordinate[1][0] = 1;
    tetrominos[last].coordinate[1][1] = 1;
    tetrominos[last].coordinate[2][0] = 1;
    break;
  }

  // On applique la transposition (sera faite si nécessaire seulement)
  transpose(&tetrominos[last], rotation);
}

// Cette fonction sert à retourner une condition sur le nombre de rotation selon le type du tetromino
bool rotationAmount(int type, int rotation)
{
  switch (type) {
  case 1:
    return (rotation < 4 && rotation > 0);
    break;
  case 2:
    return (rotation < 2 && rotation > 0);
  case 3:
    return 0;
  case 4:
    return (rotation < 2 && rotation > 0);
  case 5:
    return (rotation < 2 && rotation > 0);
  case 6:
    return (rotation < 4 && rotation > 0);
  }

  return 0;
}

// Cette fonction va décaler la position initiale du tetromino selon son type et la rotation choisie
void tetrominoShift(blocs2* bloc, int rotation)
{
  switch (bloc->type) {
  // Tetromino L
  case 1:
    if (rotation == 1) {
      bloc->startX -= BLOC_WIDTH * 2;
      bloc->startY += BLOC_HEIGHT;
    } else if (rotation == 2) {
      bloc->startX -= BLOC_WIDTH * 3;
      bloc->startY -= BLOC_HEIGHT;
    } else if (rotation == 3) {
      bloc->startX -= BLOC_WIDTH;
      bloc->startY -= BLOC_HEIGHT * 2;
    }
    break;
  // Tetromino I
  case 2:
    if (rotation == 1) {
      bloc->startX -= 25;
      bloc->startY += 25;
    }

    break;
  case 3:
    // Aucune rotation pour lui
    break;
  case 4:
    if (rotation == 1) {
      bloc->startX -= BLOC_WIDTH * 2;
      bloc->startY -= BLOC_WIDTH;
    } else if (rotation == 2) {
      bloc->startX -= BLOC_WIDTH;
      bloc->startY -= BLOC_WIDTH * 2;
    }
    break;
  case 5:
    if (rotation == 1) {
      bloc->startX -= BLOC_WIDTH;
      bloc->startY -= BLOC_WIDTH;
    } else if (rotation == 2) {
      bloc->startX -= BLOC_WIDTH;
      bloc->startY -= BLOC_WIDTH * 2;
    }
    break;
  case 6:
    if (rotation == 1) {
      bloc->startX -= BLOC_WIDTH * 2;
      bloc->startY -= BLOC_WIDTH;
    } else if (rotation == 2) {
      bloc->startX -= BLOC_WIDTH;
      bloc->startY -= BLOC_WIDTH * 3;
    } else if (rotation == 3) {
      bloc->startX += BLOC_WIDTH;
      bloc->startY -= BLOC_WIDTH * 2;
    }

    break;
  }
}

void transpose(blocs2* tetromino, int rotation)
{
  /*
   * Différents types de tetrominos
   * 1: L -> 3 rotations
   * 2: I -> 1 rotation
   * 3:
   * 4:
   * */
  blocs2 transposed;

  // On empêche la rotation si il y a plus de 4 rotation choisie, ce qui ne sert à rien
  if (rotationAmount(tetromino->type, rotation)) {
    // On fait une itération par rotation (si 0, la boucle n'est donc pas exécutée et il n'y a pas rotation)
    for (int n = 0; n < rotation; n++) {
      for (int x = 0; x < MAX_TETROMINOS_SIZE; x++) {
        for (int y = 0; y < MAX_TETROMINOS_SIZE; y++) {
          // On fait la transposition en faisant un mirroir au niveau des index y
          transposed.coordinate[y][x] = tetromino->coordinate[x][MAX_TETROMINOS_SIZE - y - 1];
        }
      }
      // On remplace le contenu de tetromino par la transposition - transposed sera ensuite ré-écris avec les nouvelles positions du tetromino
      tetromino->coordinate = transposed.coordinate;
    }

    // Et enfin on décale le tetromino pour que le déplacement du à la transposition soit transparente
    tetrominoShift(tetromino, rotation);
  }
}

// On cherche si dans la zone d'affichage, il existe des portées de pixels formant un bloc
// Si oui, y afficher le bloc
void affiche(SDL_Renderer* renderer, SDL_Texture* bloc, blocs2* blocs, SDL_Texture* active, SDL_Texture* inactive)
{
  // Pour chaque tetromino
  for (int k = 0; k < lastTetrominosIndex(blocs); k++) {
    // On fouille ses coordonnées, qui est une matrice 6x6
    for (int x = 0; x <= SCREEN_WIDTH; x++) {
      for (int y = 0; y <= SCREEN_HEIGHT; y++) {
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
