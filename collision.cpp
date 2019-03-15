#include "console.h"
#include "tetromino.h"
#include <iostream>

bool xCollide(int x1Actual, int x2Actual, int x1All, int x2All)
{
  return x2Actual > x1All && x1Actual < x2All;
}

bool yCollide(int y1Actual, int y2Actual, int y1All, int y2All)
{
  return y1Actual <= y2All && y2Actual >= y1All;
}

bool bottomBorderCollide(int y2)
{
  return y2 >= 700;
}

// On renvoi si la position donnée touche un autre bloc dans la mémoire
void collide(SDL_Renderer* renderer, blocs* tetrominos, int tetrominoIndex)
{
  int lastIndex = lastTetrominosIndex(tetrominos);
  int lastBloc = lastBlocIndex(tetrominos, lastIndex - 1);
  // On fouille tous les autres blocs existant en mémoire
  for (int i = 0; i < lastIndex; i++) {
    // On ne vérifie pas la collision des blocs d'un même tetromino
    if (i != tetrominoIndex) {
      std::cout << lastIndex << std::endl;
      // Il faut faire une boucle pour chaque bloc du tetrominos actuel
      for (int k = 0; k < lastBlocIndex(tetrominos, i); k++) {
        int x1Actual = tetrominos[tetrominoIndex].position[k].x;
        int y1Actual = tetrominos[tetrominoIndex].position[k].y;

        int x2Actual = tetrominos[tetrominoIndex].position[k].x + BLOC_WIDTH;
        int y2Actual = tetrominos[tetrominoIndex].position[k].y + BLOC_HEIGHT;

        // Puis une boucle pour tous les autres blocs de tous les autres tetrominos
        for (int j = 0; j < lastBloc; j++) {
          int x1All = tetrominos[i].position[j].x;
          int y1All = tetrominos[i].position[j].y;
          int x2All = tetrominos[i].position[j].x + BLOC_WIDTH;
          int y2All = tetrominos[i].position[j].y + BLOC_HEIGHT;

          // Si les x et y se touchent - ainsi que si un bloc touche le bord de l'écran de jeu
          if (((xCollide(x1Actual, x2Actual, x1All, x2All) && yCollide(y1Actual, y2Actual, y1All, y2All))
                  || bottomBorderCollide(y2All))) {

            // Il y a collision, donc on modifie l'état du tetromino pour l'immobiliser
            tetrominos[i].move = false;
          }
        }
      }
    }
  }
}
