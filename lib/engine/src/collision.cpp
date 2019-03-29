#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/render.hpp"

/**
 * Fonction: Engine::Collision::xCollide
 * -------------------
 * Cette fonction viens vérifier la collision entre deux blocs - sur leurs points X1 (gauche) et X2 (droite). 
 * Le bloc actuel 'actual' et tous les autres, 'all'. 
 * Le bloc actuel correspond au bloc de l'itération présente dans la boucle de fonctionnement du jeu.
 *
 * @param actualX1 Coordonnée X1 du bloc actuel
 * @param actualX2 Coordonnée X2 du bloc actuel
 * @param allX1 Coordonnée X1 de tous les autres blocs
 * @param allX2 Coordonnée X2 de tous les autres blocs
 *
 * @return 0 si aucune collision X, 1 sinon
 *
 * @see Engine::Collision
 * @see Engine::Run::Loop
 */
bool Engine::Collision::xCollide(int actualX1, int actualX2, int allX1, int allX2)
{
  return actualX2 > allX1 && allX2 > actualX1;
}

/**
 * Fonction: Engine::Collision::yCollide
 * -------------------
 * Cette fonction viens vérifier la collision entre deux blocs - sur leurs points Y1 (haut) et Y2 (bas). 
 * Le bloc actuel 'actual' et tous les autres, 'all'. 
 * Le bloc actuel correspond au bloc de l'itération présente dans la boucle de fonctionnement du jeu.
 *
 * @param actualY1 Coordonnée Y1 du bloc actuel
 * @param actualY2 Coordonnée Y2 du bloc actuel
 * @param allY1 Coordonnée Y1 de tous les autres blocs
 * @param allY2 Coordonnée Y2 de tous les autres blocs
 *
 * @return 0 si aucune collision X, 1 sinon
 *
 * @see Engine::Collision
 * @see Engine::Run::Loop
 */
bool Engine::Collision::yCollide(int actualY1, int actualY2, int allY1, int allY2)
{
  return actualY2 >= allY1 && allY2 >= actualY1;
}

/**
 * Fonction: Engine::Collision::tetrominoCollide
 * -------------------
 * Par soucis d'optimisation, cette fonction a été concue. 
 * Elle vient vérifier si la matrice représentant un tetromino touche une des autres,
 * et ce SANS compter les points de la matrice correspondant à un bloc. 
 * Cela limite le nombre de calcul; on vérifie les matrices entre elles, puis si il y a collision, on viens vérifier les blocs entre eux.
 *
 * @param actualY1 Coordonnée Y1 de la matrice actuelle
 * @param actualY2 Coordonnée Y2 de la matrice actuelle
 * @param allY1 Coordonnée Y1 de toutes les autres matrices
 * @param allY2 Coordonnée Y2 de toutes les autres matrices
 *
 * @return 0 si aucune collision X, 1 sinon
 *
 * @see Engine::Collision::collide
 */
bool Engine::Collision::tetrominoCollide(int actualX1, int actualY1, int actualX2, int actualY2, int allX1, int allY1, int allX2, int allY2)
{
  // La vérification des x et y sont fait en un seul return
  return (actualX2 > allX1 && allX2 > actualX1) && (actualY2 >= allY1 && allY2 >= actualY1);
}

/**
 * Fonction: Engine::Collision::screenCollide
 * -------------------
 * Vérifie si la coordonnée Y2 du tetromino actuel (ou quoi que ce soit d'autre) touche le bord inférieur de l'écran.
 *
 * @param actualY2 Coordonnée Y2 du tetromino actuel (ou quoi que ce soit d'autre)
 *
 * @return 0 si aucune collision, 1 sinon
 *
 * @see Engine::Collision::collide
 */
bool Engine::Collision::screenCollide(int actualY2)
{
  Engine::Render render;

  return (actualY2 >= render.height);
}

/**
 * Fonction: Engine::Collision::screenCollide
 * -------------------
 * Exécute les vérifications des collisions du bloc de tetromino actuel avec tous les autres. 
 * S'il y a collision, modifie la structure du tetromino actuel, pour le rendre immobile, 
 * en modifiant sa variable booléenne 'move' pour devenir false, donc immobile.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param tetrominoIndex Index du tetromino actuel dans le tableau contenant tous les tetrominos en mémoire
 *
 * @see Engine::Collision::xCollide
 * @see Engine::Collision::yCollide
 */
void Engine::Collision::collide(Game::Tetromino::blocs* tetrominos, int tetrominoIndex)
{
  Game::Tetromino tetromino;
  int last = tetromino.lastIndex(tetrominos);

  // On fouille dans tous les tetrominos existant
  for (int k = 0; k < last; k++) {
    int tetrominoActualx1 = tetrominos[tetrominoIndex].startX;
    int tetrominoActualy1 = tetrominos[tetrominoIndex].startY;
    int tetrominoActualx2 = tetrominoActualx1 + (Game::Tetromino::blocWidth * Game::Tetromino::maxSize);
    int tetrominoActualy2 = tetrominoActualy1 + (Game::Tetromino::blocHeight * Game::Tetromino::maxSize);

    int tetrominoAllx1 = tetrominos[k].startX;
    int tetrominoAlly1 = tetrominos[k].startY;
    int tetrominoAllx2 = tetrominoAllx1 + (Game::Tetromino::blocWidth * Game::Tetromino::maxSize);
    int tetrominoAlly2 = tetrominoAlly1 + (Game::Tetromino::blocHeight * Game::Tetromino::maxSize);
    /**
     * On compare le tetromino actuel avec tous les autres, donc dans cette boucle on ne compte pas le bloc actuel
     * Il doit y avoir collision entre les matrices des tetrominos pour commencer à comparer les blocs entre eux, pour les collisions
     * De cette manière le calcul des collision est plus optimisé
     * Il faut aussi que l'une des deux matrices soit en train de bouger, sinon ca ne sert à rien
     */
    if ((k != tetrominoIndex)
        && (tetrominoCollide(tetrominoActualx1, tetrominoActualy1, tetrominoActualx2, tetrominoActualy2, tetrominoAllx1, tetrominoAlly1, tetrominoAllx2, tetrominoAlly2))
        && (tetrominos[tetrominoIndex].move || tetrominos[k].move)) {
      // On voit si chaque startX/Y + les points de la matrice où un bloc est présent touche un autre
      // Si la coordonnée actuelle est un bloc - on le compare avec tous les autres blocs
      for (int xActual = 0; xActual < Game::Tetromino::maxSize; xActual++) {
        for (int yActual = 0; yActual < Game::Tetromino::maxSize; yActual++) {
          if (tetrominos[tetrominoIndex].coordinate[xActual][yActual] == 1) {
            // On récupère les coordonnées du bloc actuel
            int x1BlocActual = tetrominos[tetrominoIndex].startX + (xActual * Game::Tetromino::blocWidth);
            int y1BlocActual = tetrominos[tetrominoIndex].startY + (yActual * Game::Tetromino::blocHeight);
            int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
            int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;

            // On fait une autre boucle pour tous les blocs à l'intérieur du tetromino actuel
            for (int xAll = 0; xAll < Game::Tetromino::maxSize; xAll++) {
              for (int yAll = 0; yAll < Game::Tetromino::maxSize; yAll++) {
                if (tetrominos[k].coordinate[xAll][yAll] == 1) {
                  // On récupère les coordonnées de tous les autres blocs
                  int x1BlocAll = tetrominos[k].startX + (xAll * Game::Tetromino::blocWidth);
                  int y1BlocAll = tetrominos[k].startY + (yAll * Game::Tetromino::blocHeight);
                  int x2BlocAll = x1BlocAll + Game::Tetromino::blocWidth;
                  int y2BlocAll = y1BlocAll + Game::Tetromino::blocHeight;

                  // On compare les blocs du tetromino actuel avec tous les autres pour voir s'il y a collision
                  if (xCollide(x1BlocActual, x2BlocActual, x1BlocAll, x2BlocAll)
                      && yCollide(y1BlocActual, y2BlocActual, y1BlocAll, y2BlocAll)) {

                    // Si le bloc actuel touche un autre bloc, on le rend lui aussi immobile
                    tetrominos[tetrominoIndex].move = false;

                    // L'autre bloc doit alors devenir zombie
                    //tetrominos[k].zombie = true;
                  }
                }
              }
            }
          }
        }
      }
    } else {
      // On veut quand même voir si les blocs du tetromino actuel touchent le sol ou non
      for (int xActual = 0; xActual < Game::Tetromino::maxSize; xActual++) {
        for (int yActual = 0; yActual < Game::Tetromino::maxSize; yActual++) {
          // On récupère les coordonnées du bloc actuel, qu'on utilise pour vérifier la collision avec le bord de l'écran
          int y1BlocActual = tetrominos[tetrominoIndex].startY + (yActual * Game::Tetromino::blocHeight);
          int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;

          /*
           * Le point dans la matrice doit être un bloc pour vérifier si il touche le bord de l'écran
           * Si il touche bien le bord de l'écran, alors on le rend immobile
           * */
          if ((tetrominos[tetrominoIndex].coordinate[xActual][yActual] == 1)
              && screenCollide(y2BlocActual)) {
            tetrominos[tetrominoIndex].move = false;
          }
        }
      }
    }
  }
}
