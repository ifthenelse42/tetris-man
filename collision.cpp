#include "console.h"
#include "renderer.h"
#include "tetromino.h"
#include <iostream>

bool xCollide(int x1BlocActual, int x2BlocActual, int x1BlocAll, int x2BlocAll)
{
  return x2BlocActual > x1BlocAll && x2BlocAll > x1BlocActual;
}

bool yCollide(int y1BlocActual, int y2BlocActual, int y1BlocAll, int y2BlocAll)
{
  return y2BlocActual >= y1BlocAll && y2BlocAll >= y1BlocActual;
}

bool bottomBorderCollide(int y2)
{
  return y2 >= 700;
}

// On renvoi si la position donnée touche un autre bloc dans la mémoire
void collide(blocs2* tetrominos, int tetrominoIndex)
{
  int lastIndex = lastTetrominosIndex(tetrominos);

  // On fouille dans tous les tetrominos existant
  for (int k = 0; k < lastIndex; k++) {
    // Sauf le tetromino actuel
    if (k != tetrominoIndex) {
      std::cout << "Bloc " << k << std::endl;
      // On voit si chaque startX/Y + les points de la matrice où un bloc est présent touche un autre
      // startX + x * BLOC_WIDTH
      // Si la coordonnée actuelle est un bloc - on le compare avec tous les autres blocs
      for (int xActual = 0; xActual < MAX_TETROMINOS_SIZE; xActual++) {
        for (int yActual = 0; yActual < MAX_TETROMINOS_SIZE; yActual++) {
          if (tetrominos[tetrominoIndex].coordinate[xActual][yActual] == 1) {
            // On récupère les coordonnées du bloc actuel
            int x1BlocActual = tetrominos[tetrominoIndex].startX + (xActual * BLOC_WIDTH);
            int y1BlocActual = tetrominos[tetrominoIndex].startY + (yActual * BLOC_HEIGHT);
            int x2BlocActual = x1BlocActual + BLOC_WIDTH;
            int y2BlocActual = y1BlocActual + BLOC_HEIGHT;

            // On fait une autre boucle pour tous les blocs à l'intérieur du tetromino actuel

            for (int xAll = 0; xAll < MAX_TETROMINOS_SIZE; xAll++) {
              for (int yAll = 0; yAll < MAX_TETROMINOS_SIZE; yAll++) {
                if (tetrominos[k].coordinate[xAll][yAll] == 1) {
                  // On récupère les coordonnées de tous les autres blocs
                  int x1BlocAll = tetrominos[k].startX + (xAll * BLOC_WIDTH);
                  int y1BlocAll = tetrominos[k].startY + (yAll * BLOC_HEIGHT);
                  int x2BlocAll = x1BlocAll + BLOC_WIDTH;
                  int y2BlocAll = y1BlocAll + BLOC_HEIGHT;

                  // Si le bloc actuel ne touche pas encore le sol, on peut vérifier ses collisions
                  if (!(y2BlocActual > SCREEN_HEIGHT)) {
                    // On compare les blocs du tetromino actuel avec tous les autres pour voir s'il y a collision
                    if (xCollide(x1BlocActual, x2BlocActual, x1BlocAll, x2BlocAll)
                        && yCollide(y1BlocActual, y2BlocActual, y1BlocAll, y2BlocAll)) {
                      std::cout << "Collides with bloc " << k << std::endl;
                      std::cout << "y1BlocActual: " << y1BlocActual << std::endl;
                      std::cout << "y2BlocActual: " << y2BlocActual << std::endl;
                      std::cout << "y1BlocAll: " << y1BlocAll << std::endl;
                      std::cout << "y2BlocAll: " << y2BlocAll << std::endl;

                      // Si le bloc actuel touche un autre bloc, on le rend lui aussi immobile
                      tetrominos[tetrominoIndex].move = false;
                    }
                  } else {
                    // Si le bloc actuel touche le sol, on le rend immobile
                    tetrominos[tetrominoIndex].move = false;
                  }
                }
              }
            }
          }
        }
      }
      std::cout << "------------------------------------" << std::endl;
    }
  }
}
