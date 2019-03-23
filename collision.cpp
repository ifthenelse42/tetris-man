#include "console.h"
#include "renderer.h"
#include "tetromino.h"
#include <iostream>

bool xCollide(int actualX1, int actualX2, int allX1, int allX2)
{
  return actualX2 > allX1 && allX2 > actualX1;
}

bool yCollide(int actualY1, int actualY2, int allY1, int allY2)
{
  return actualY2 >= allY1 && allY2 >= actualY1;
}

bool tetrominoCollide(int actualX1, int actualY1, int actualX2, int actualY2, int allX1, int allY1, int allX2, int allY2)
{
  // La vérification des x et y sont fait en un seul return
  return (actualX2 > allX1 && allX2 > actualX1) && (actualY2 >= allY1 && allY2 >= actualY1);
}

bool tetrominoCollideScreen(int actualY2)
{
  return (actualY2 >= SCREEN_HEIGHT);
}

bool bottomBorderCollide(int y2)
{
  return y2 >= 700;
}

// On renvoi si la position donnée touche un autre bloc dans la mémoire
void collide(blocs2* tetrominos, int tetrominoIndex)
{
  int last = lastIndex(tetrominos);

  // On fouille dans tous les tetrominos existant
  for (int k = 0; k < last; k++) {
    int tetrominoActualx1 = tetrominos[tetrominoIndex].startX;
    int tetrominoActualy1 = tetrominos[tetrominoIndex].startY;
    int tetrominoActualx2 = tetrominoActualx1 + (BLOC_WIDTH * MAX_TETROMINOS_SIZE);
    int tetrominoActualy2 = tetrominoActualy1 + (BLOC_HEIGHT * MAX_TETROMINOS_SIZE);

    int tetrominoAllx1 = tetrominos[k].startX;
    int tetrominoAlly1 = tetrominos[k].startY;
    int tetrominoAllx2 = tetrominoAllx1 + (BLOC_WIDTH * MAX_TETROMINOS_SIZE);
    int tetrominoAlly2 = tetrominoAlly1 + (BLOC_HEIGHT * MAX_TETROMINOS_SIZE);
    /**
     * On compare le tetromino actuel avec tous les autres, donc dans cette boucle on ne compte pas le bloc actuel
     * Il doit y avoir collision entre les matrices des tetrominos pour commencer à comparer les blocs entre eux, pour les collisions
     * De cette manière le calcul des collision est plus optimisé
     * Il faut aussi que l'une des deux matrices soit en train de bouger, sinon ca ne sert à rien
     */
    if ((k != tetrominoIndex)
        && (tetrominoCollide(tetrominoActualx1, tetrominoActualy1, tetrominoActualx2, tetrominoActualy2, tetrominoAllx1, tetrominoAlly1, tetrominoAllx2, tetrominoAlly2))
        && (tetrominos[tetrominoIndex].move || tetrominos[k].move)) {
      //std::cout << "Bloc " << k << " entre en collision avec le bloc " << tetrominoIndex << " - on procède donc à la vérification des collisions des blocs entre eux" << std::endl;
      // On voit si chaque startX/Y + les points de la matrice où un bloc est présent touche un autre
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

                  // On compare les blocs du tetromino actuel avec tous les autres pour voir s'il y a collision
                  if (xCollide(x1BlocActual, x2BlocActual, x1BlocAll, x2BlocAll)
                      && yCollide(y1BlocActual, y2BlocActual, y1BlocAll, y2BlocAll)) {

                    // Si le bloc actuel touche un autre bloc, on le rend lui aussi immobile
                    tetrominos[tetrominoIndex].move = false;
                  }
                }
              }
            }
          }
        }
      }
    } else {
      // On veut quand même voir si les blocs du tetromino actuel touchent le sol ou non
      for (int xActual = 0; xActual < MAX_TETROMINOS_SIZE; xActual++) {
        for (int yActual = 0; yActual < MAX_TETROMINOS_SIZE; yActual++) {
          // On récupère les coordonnées du bloc actuel, qu'on utilise pour vérifier la collision avec le bord de l'écran
          int y1BlocActual = tetrominos[tetrominoIndex].startY + (yActual * BLOC_HEIGHT);
          int y2BlocActual = y1BlocActual + BLOC_HEIGHT;

          /*
           * Le point dans la matrice doit être un bloc pour vérifier si il touche le bord de l'écran
           * Si il touche bien le bord de l'écran, alors on le rend immobile
           * */
          if ((tetrominos[tetrominoIndex].coordinate[xActual][yActual] == 1)
              && tetrominoCollideScreen(y2BlocActual)) {
            tetrominos[tetrominoIndex].move = false;
          }
        }
      }
    }
  }
}
