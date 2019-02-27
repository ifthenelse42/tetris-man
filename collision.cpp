#include "tetromino.h"
#include <iostream>

bool xCollide(int x1Actual, int x2Actual, int x1All, int x2All)
{
  return x2Actual >= x1All && x1Actual <= x2All;
}

bool yCollide(int y1Actual, int y2Actual, int y1All, int y2All)
{
  return y1Actual <= y2All && y2Actual >= y1All;
}

bool borderTopRightCollide(int x2Actual, int x1All, int y1Actual, int y2All)
{
  return (y1Actual == y2All) && (x2Actual == x1All);
}
bool borderTopLeftCollide(int x1Actual, int x2All, int y1Actual, int y2All)
{
  return (y1Actual == y2All) && (x1Actual == x2All);
}
bool borderBottomRightCollide(int x2Actual, int x1All, int y2Actual, int y1All)
{
  return (y2Actual == y1All) && (x2Actual == x1All);
}
bool borderBottomLeftCollide(int x1Actual, int x2All, int y2Actual, int y1All)
{
  return (y2Actual == y1All) && (x1Actual == x2All);
}

// On renvoi si la position donnée touche un autre bloc dans la mémoire
void collide(blocs* tetrominos, int tetrominoIndex)
{
  int lastIndex = lastTetrominosIndex(tetrominos);
  int lastBloc = lastBlocIndex(tetrominos, lastIndex - 1);
  // On fouille tous les autres blocs existant en mémoire
  for (int i = 0; i < lastIndex; i++) {
    std::cout << "Tetromino [" << i << "] :" << std::endl;
    // On ne vérifie pas la collision des blocs d'un même tetromino
    if (i != tetrominoIndex) {
      // Il faut faire une boucle pour chaque bloc du tetrominos actuel
      for (int k = 0; k < lastBlocIndex(tetrominos, i); k++) {
        int x1Actual = tetrominos[tetrominoIndex].position[k].x;
        int y1Actual = tetrominos[tetrominoIndex].position[k].y;

        int x2Actual = tetrominos[tetrominoIndex].position[k].x + BLOC_WIDTH;
        int y2Actual = tetrominos[tetrominoIndex].position[k].y + BLOC_HEIGHT;

        for (int j = 0; j < lastBloc; j++) {
          int x1All = tetrominos[i].position[j].x;
          int y1All = tetrominos[i].position[j].y;
          int x2All = tetrominos[i].position[j].x + BLOC_WIDTH;
          int y2All = tetrominos[i].position[j].y + BLOC_HEIGHT;

          std::cout << "-> x1Actual :" << x1Actual << std::endl;
          std::cout << "-> y1Actual :" << y1Actual << std::endl;
          std::cout << "-> x2Actual :" << x2Actual << std::endl;
          std::cout << "-> y2Actual :" << y2Actual << std::endl;

          std::cout << "-> x1All :" << x1All << std::endl;
          std::cout << "-> y1All :" << y1All << std::endl;
          std::cout << "-> x2All :" << x2All << std::endl;
          std::cout << "-> y2All :" << y2All << std::endl;
          std::cout << "-> canMove :" << tetrominos[tetrominoIndex].move << std::endl;

          // Si les x et y se touchent
          if ((xCollide(x1Actual, x2Actual, x1All, x2All) && yCollide(y1Actual, y2Actual, y1All, y2All))
              && !borderTopRightCollide(x2Actual, x1All, y1Actual, y2All)
              && !borderTopLeftCollide(x1Actual, x2All, y1Actual, y2All)
              && !borderBottomRightCollide(x2Actual, x1All, y2Actual, y1All)
              && !borderBottomLeftCollide(x1Actual, x2All, y2Actual, y1All)) {
            std::cout << "Touch!" << std::endl;

            // Il y a collision, donc on modifie l'état du tetromino pour l'immobiliser
            tetrominos[i].move = false;
          }
        }
      }
    }
  }
}
