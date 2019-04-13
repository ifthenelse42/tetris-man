#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/render.hpp"
#include <iostream>

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
bool Engine::Collision::screenCollide(int actualY2, int* height)
{
  Engine::Render render;

  return (actualY2 >= *height);
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
void Engine::Collision::collide(Game::Tetromino::blocs* tetrominos, int tetrominoIndex, int* max, int* height)
{
  Game::Tetromino tetromino;

  // On fouille dans tous les tetrominos existant
  for (int k = 0; k < *max; k++) {
    int tetrominoActualx1 = tetrominos[tetrominoIndex].startX;
    int tetrominoActualy1 = tetrominos[tetrominoIndex].startY;
    int tetrominoActualx2 = tetrominoActualx1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoActualy2 = tetrominoActualy1 + (tetromino.blocHeight * tetromino.maxSize) + tetrominos[tetrominoIndex].velocity;

    int tetrominoAllx1 = tetrominos[k].startX;
    int tetrominoAlly1 = tetrominos[k].startY;
    int tetrominoAllx2 = tetrominoAllx1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoAlly2 = tetrominoAlly1 + (tetromino.blocHeight * tetromino.maxSize);
    /**
     * On compare le tetromino actuel avec tous les autres, donc dans cette boucle on ne compte pas le bloc actuel
     * Il doit y avoir collision entre les matrices des tetrominos pour commencer à comparer les blocs entre eux, pour les collisions
     * De cette manière le calcul des collision est plus optimisé
     * Il faut aussi que l'une des deux matrices soit en train de bouger, sinon ca ne sert à rien
     */
    if ((k != tetrominoIndex)
        && (tetrominoCollide(tetrominoActualx1, tetrominoActualy1, tetrominoActualx2, tetrominoActualy2, tetrominoAllx1, tetrominoAlly1, tetrominoAllx2, tetrominoAlly2))
        && (tetrominos[tetrominoIndex].velocity != 0 || tetrominos[k].velocity != 0)) {
      // On voit si chaque startX/Y + les points de la matrice où un bloc est présent touche un autre
      // Si la coordonnée actuelle est un bloc - on le compare avec tous les autres blocs
      for (int xActual = 0; xActual < tetromino.maxSize; xActual++) {
        for (int yActual = 0; yActual < tetromino.maxSize; yActual++) {
          if (tetrominos[tetrominoIndex].coordinate[xActual][yActual] == 1) {
            // On récupère les coordonnées du bloc actuel
            int x1BlocActual = tetrominos[tetrominoIndex].startX + (xActual * tetromino.blocWidth);
            int y1BlocActual = tetrominos[tetrominoIndex].startY + (yActual * tetromino.blocHeight);
            int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
            int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;
            int y2BlocActualNext = y1BlocActual + Game::Tetromino::blocHeight + tetrominos[tetrominoIndex].velocity * 2;

            // On fait une autre boucle pour tous les blocs à l'intérieur du tetromino actuel
            for (int xAll = 0; xAll < Game::Tetromino::maxSize; xAll++) {
              for (int yAll = 0; yAll < Game::Tetromino::maxSize; yAll++) {
                if (tetrominos[k].coordinate[xAll][yAll] == 1) {
                  // On récupère les coordonnées de tous les autres blocs
                  int x1BlocAll = tetrominos[k].startX + (xAll * tetromino.blocWidth);
                  int y1BlocAll = tetrominos[k].startY + (yAll * tetromino.blocHeight);
                  int x2BlocAll = x1BlocAll + tetromino.blocWidth;
                  int y2BlocAll = y1BlocAll + tetromino.blocHeight;

                  
                  if (xCollide(x1BlocActual, x2BlocActual, x1BlocAll, x2BlocAll)) {
                    /**
                     * On vérifie d'abord si la prochaine position du bloc en touche un autre. 
                     * Si c'est le cas, on le ralentis de 1 si la vélocité n'est pas déjà de 1. 
                     * 
                     * Si ce n'est pas le cas, on vérifie la collision en Y normalement.
                     */
                      if (yCollide(y1BlocActual, y2BlocActualNext, y1BlocAll, y2BlocAll)
                          && tetrominos[tetrominoIndex].velocity > 1) {
                        tetrominos[tetrominoIndex].velocity = 1;
                      } else if (yCollide(y1BlocActual, y2BlocActual, y1BlocAll, y2BlocAll)) {
                        tetrominos[tetrominoIndex].velocity = 0;
                      }
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
              && screenCollide(y2BlocActual, height)) {
            tetrominos[tetrominoIndex].velocity = 0;
          }
        }
      }
    }
  }
}

/**
 * Fonction: Engine::Collision::collideCharacterX
 * -------------------
 * Gère la collision du personnage. Il est également être écrasé (et donc game over) 
 * s'il est entre deux tetrominos.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Dernier index occupé par les tetrominos
 * @param position Pointeur vers les coordonnées du personnage
 * @param run Pointeur vers l'interrupteur du jeu, pour terminer le jeu si le personnage meurt
 *
 * @return Si le personnage touche un autre bloc ou non.
 */
bool Engine::Collision::collideCharacter(Game::Tetromino::blocs* tetrominos, int *max, Game::Character::position* position, bool* run)
{
  Game::Tetromino tetromino;

  bool res = false;

  /**
   * On fait une boucle dans tous les tetrominos, s'il y a une première collision, 
   * on vérifie si le personnage touche un des blocs.
   */
  for (int i = 0; i < *max; i++) {
    // On récupère les coordonnées des tetrominos
    int tetrominoX1 = tetrominos[i].startX;
    int tetrominoX2 = tetrominoX1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoY1 = tetrominos[i].startY;
    int tetrominoY2 = tetrominoY1 + (tetromino.blocHeight * tetromino.maxSize);

    // On vérifie maintenant s'il y a collision
    if (xCollide(position->x, position->x + position->width, tetrominoX1, tetrominoX2)
        && yCollide(position->y, position->y + position->height, tetrominoY1, tetrominoY2)) {
      /** 
       * S'il y a collision avec un tetromino absolu, alors on vérifie si il y a 
       * également collision avec les blocs du tetromino en question. 
       *
       * On fait donc des boucles dans les blocs du tetromino actuel.
       */
      for (int xActual = 0; xActual < tetromino.maxSize; xActual++) {
        for (int yActual = 0; yActual < tetromino.maxSize; yActual++) {
          // Le bloc actuel doit en être un
          if (tetrominos[i].coordinate[xActual][yActual] == 1) {
          // On récupère les coordonnées du bloc actuel
          int x1BlocActual = tetrominos[i].startX + (xActual * tetromino.blocWidth);
          int y1BlocActual = tetrominos[i].startY + (yActual * tetromino.blocWidth);

          int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
          int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;

          // On peut alors vérifier si le personnage touche un des y1 des blocs
        if (xCollide(position->x, position->x + position->width, x1BlocActual, x2BlocActual)
            && yCollide(position->y, position->y + position->height, y1BlocActual, y2BlocActual)) {
            // Si le joueur touche bien le sol d'un des blocs, alors on renvoie true
            res = true;
          }
          }
        }
      }
    }
  }

  return res;
}

/**
 * Fonction: Engine::Collision::collideCharacterSide
 * -------------------
 * Détecte si le personnage, en se déplacant vers le côté d'un tetromino, touche un de ses blocs.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Dernier index occupé par les tetrominos
 * @param position Pointeur vers les coordonnées du personnage
 * @param run Pointeur vers l'interrupteur du jeu, pour terminer le jeu si le personnage meurt
 *
 * @return Si le personnage touche un autre bloc ou non.
 */
bool Engine::Collision::collideCharacterSide(Game::Tetromino::blocs* tetrominos, int *max, Game::Character::position* position, bool* run)
{
  Game::Tetromino tetromino;

  bool res = false;

  /**
   * On fait une boucle dans tous les tetrominos, s'il y a une première collision, 
   * on vérifie si le personnage touche un des blocs.
   */
  for (int i = 0; i < *max; i++) {
    // On récupère les coordonnées des tetrominos
    int tetrominoX1 = tetrominos[i].startX;
    int tetrominoX2 = tetrominoX1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoY1 = tetrominos[i].startY;
    int tetrominoY2 = tetrominoY1 + (tetromino.blocHeight * tetromino.maxSize);

    // On vérifie maintenant s'il y a collision
    if (yCollide(position->y, position->y + position->height, tetrominoY1, tetrominoY2)
        && xCollide(position->x, position->x + position->width, tetrominoX1, tetrominoX2)) {
      /** 
       * S'il y a collision avec un tetromino absolu, alors on vérifie si il y a 
       * également collision avec les blocs du tetromino en question. 
       *
       * On fait donc des boucles dans les blocs du tetromino actuel.
       */
      for (int xActual = 0; xActual < tetromino.maxSize; xActual++) {
        for (int yActual = 0; yActual < tetromino.maxSize; yActual++) {
          // Le bloc actuel doit en être un
          if (tetrominos[i].coordinate[xActual][yActual] == 1) {
          // On récupère les coordonnées du bloc actuel
          int x1BlocActual = tetrominos[i].startX + (xActual * tetromino.blocWidth);
          int y1BlocActual = tetrominos[i].startY + (yActual * tetromino.blocWidth);

          int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
          int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;

          // On peut alors vérifier si le personnage touche un des y1 des blocs (et les x)
          if (position->y + position->height > y1BlocActual
              && xCollide(position->x, position->x + position->width, x1BlocActual, x2BlocActual)) {
            // Si le joueur touche bien le sol d'un des blocs, alors on renvoie true
            res = true;
          }
          }
        }
      }
    }
  }

  return res;
}

/**
 * Fonction: Engine::Collision::collideCharacterFall
 * -------------------
 * Détecte si le personnage touche le côté bas d'un tetromino, ce qui le fera tomber.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Dernier index occupé par les tetrominos
 * @param position Pointeur vers les coordonnées du personnage
 * @param run Pointeur vers l'interrupteur du jeu, pour terminer le jeu si le personnage meurt
 *
 * @return Si le personnage touche un autre bloc ou non.
 */
bool Engine::Collision::collideCharacterFall(Game::Tetromino::blocs* tetrominos, int *max, Game::Character::position* position, bool* run)
{
  Game::Tetromino tetromino;

  bool res = false;

  /**
   * On fait une boucle dans tous les tetrominos, s'il y a une première collision, 
   * on vérifie si le personnage touche un des blocs.
   */
  for (int i = 0; i < *max; i++) {
    // On récupère les coordonnées des tetrominos
    int tetrominoX1 = tetrominos[i].startX;
    int tetrominoX2 = tetrominoX1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoY1 = tetrominos[i].startY;
    int tetrominoY2 = tetrominoY1 + (tetromino.blocHeight * tetromino.maxSize) * 2;

    // On vérifie maintenant s'il y a collision
    if (yCollide(position->y, position->y + position->height, tetrominoY1, tetrominoY2)
        && xCollide(position->x, position->x + position->width, tetrominoX1, tetrominoX2)) {
      /** 
       * S'il y a collision avec un tetromino absolu, alors on vérifie si il y a 
       * également collision avec les blocs du tetromino en question. 
       *
       * On fait donc des boucles dans les blocs du tetromino actuel.
       */
      for (int xActual = 0; xActual < tetromino.maxSize; xActual++) {
        for (int yActual = 0; yActual < tetromino.maxSize; yActual++) {
          // Le bloc actuel doit en être un
          if (tetrominos[i].coordinate[xActual][yActual] == 1) {
          // On récupère les coordonnées du bloc actuel
          int x1BlocActual = tetrominos[i].startX + (xActual * tetromino.blocWidth);
          int y1BlocActual = tetrominos[i].startY + (yActual * tetromino.blocWidth);

          int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
          int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;
          int y2BlocActualNext = y1BlocActual + Game::Tetromino::blocHeight + tetrominos[i].velocity;

          /**
           * On vérifie si le personnage touche le y2 d'un bloc. 
           * Il doit également ne toucher aucun autre bloc, donc être en plein saut.
           *
           * Soit le y1 du personnage est inférieur au y2 d'un bloc, soit 
           * le y1 est inférieur au y2 d'un bloc + la vélocité d'un bloc
           */
          //if (position->y <= y2BlocActual && position->y >= y2BlocActual
          if ((position->y <= y2BlocActualNext && position->y >= y1BlocActual)
              && xCollide(position->x, position->x + position->width, x1BlocActual, x2BlocActual)) {
            // Si le joueur touche bien le sol d'un des blocs, alors on renvoie true
            res = true;
          }
          }
        }
      }
    }
  }

  return res;
}

/**
 * Fonction: Engine::Collision::collideCharacterTop
 * -------------------
 * Détecte si le personnage est allé en dessous d'un bloc, ce qui provoque sa mort.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Dernier index occupé par les tetrominos
 * @param position Pointeur vers les coordonnées du personnage
 * @param run Pointeur vers l'interrupteur du jeu, pour terminer le jeu si le personnage meurt
 *
 * @return Si le personnage touche un autre bloc ou non.
 */
bool Engine::Collision::collideCharacterDead(Game::Tetromino::blocs* tetrominos, int *max, Game::Character::position* position, bool* run)
{
  Game::Tetromino tetromino;

  bool res = false;

  /**
   * On fait une boucle dans tous les tetrominos, s'il y a une première collision, 
   * on vérifie si le personnage touche un des blocs.
   */
  for (int i = 0; i < *max; i++) {
    // On récupère les coordonnées des tetrominos
    int tetrominoX1 = tetrominos[i].startX;
    int tetrominoX2 = tetrominoX1 + (tetromino.blocWidth * tetromino.maxSize);
    int tetrominoY1 = tetrominos[i].startY;
    int tetrominoY2 = tetrominoY1 + (tetromino.blocHeight * tetromino.maxSize);

    // On vérifie maintenant s'il y a collision
    if (yCollide(position->y, position->y + position->height, tetrominoY1, tetrominoY2)
        && xCollide(position->x, position->x + position->width, tetrominoX1, tetrominoX2)) {
      /** 
       * S'il y a collision avec un tetromino absolu, alors on vérifie si il y a 
       * également collision avec les blocs du tetromino en question. 
       *
       * On fait donc des boucles dans les blocs du tetromino actuel.
       */
      for (int xActual = 0; xActual < tetromino.maxSize; xActual++) {
        for (int yActual = 0; yActual < tetromino.maxSize; yActual++) {
          // Le bloc actuel doit en être un
          if (tetrominos[i].coordinate[xActual][yActual] == 1) {
          // On récupère les coordonnées du bloc actuel
          int x1BlocActual = tetrominos[i].startX + (xActual * tetromino.blocWidth);
          int y1BlocActual = tetrominos[i].startY + (yActual * tetromino.blocWidth);

          int x2BlocActual = x1BlocActual + Game::Tetromino::blocWidth;
          int y2BlocActual = y1BlocActual + Game::Tetromino::blocHeight;

          // On peut alors vérifier si le personnage touche un des y1 des blocs (et les x)
          if (position->y < y2BlocActual && position->y + position->height > y1BlocActual
              && xCollide(position->x, position->x + position->width, x1BlocActual, x2BlocActual)) {
            // Si le joueur touche bien le sol d'un des blocs, alors on renvoie true
            res = true;
          }
          }
        }
      }
    }
  }

  return res;
}
