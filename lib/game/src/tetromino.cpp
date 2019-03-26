#include "game/tetromino.hpp"
#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/texture.hpp"
#include "game/game.hpp"
#include <iostream>
#include <math.h>

/**
 * Fonction: Game::Tetromino::lastIndex
 * -------------------
 * Retrouve le dernier index réel du tableau contenant les tetrominos. 
 * Cela permet de limiter les itérations, le nombre maximum de tetromino dans la mémoire étant fixe.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @see Game::Tetromino
 * @return Le dernier index réel
 */
int Game::Tetromino::lastIndex(blocs* tetrominos)
{
  int last = 0;

  for (int i = 0; i < Tetromino::max; i++) {
    // Si le type est défini à 0, ca veut dire que ca ne correspond à aucun tetromino
    if (tetrominos[i].type == 0) {
      last = i;
      break;
    }
  }

  return last;
}

/**
 * Fonction: Game::Tetromino::fall
 * -------------------
 * Fait tomber les tetrominos.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @see Game::Tetromino
 */
void Game::Tetromino::fall(blocs* tetrominos)
{
  int last = lastIndex(tetrominos);
  for (int k = 0; k < last; k++) {
    if (tetrominos[k].move) {
      tetrominos[k].startY += 1;
    }
  }
}

/**
 * Fonction: Game::Tetromino::add
 * -------------------
 * Ajoute un tetromino dans la mémoire, 
 * display() se charge ensuite de les afficher.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param startX Position initiale X de la matrice représentant le tetromino
 * @param startY Position initiale Y de la matrice représentant le tetromino
 * @param type Type du tetromino (ce qui défini donc sa forme)
 * @param rotation Nombre de rotation initiale du tetromino
 * @see Game::Tetromino
 */
void Game::Tetromino::add(blocs* tetrominos, int startX, int startY, int type, int rotation)
{
  // On obtiens le dernier index des tetrominos
  int last = lastIndex(tetrominos);

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

/**
 * Fonction: Game::Tetromino::rotationAmount
 * ------------------------
 * Renvoie en tant que condition le nombre de rotation possible d'un tetromino, selon son type.
 *
 * @param type Type du tetromino (ce qui défini donc sa forme)
 * @param rotation Nombre de rotation initiale du tetromino
 * @see Game::Tetromino
 * @return Condition d'intervalle sur le nombre de rotation possible
 */
bool Game::Tetromino::rotationAmount(int type, int rotation)
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

/**
 * Fonction: Game::Tetromino::shift
 * ------------------------
 * Cette fonction viens déplacer la position initiale du tetromino après rotation,
 * car la rotation implique un léger déplacement, 
 * cette fonction viens alors régler ce léger déplacement, 
 * pour que la rotation ne semble pas bouger pour l'utilisateur.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param rotation Nombre de rotation du tetromino
 * @see Game::Tetromino
 */
void Game::Tetromino::shift(blocs* tetrominos, int rotation)
{
  Tetromino tetromino;

  switch (tetrominos->type) {
  // Tetromino L
  case 1:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth * 2;
      tetrominos->startY += tetromino.blocHeight;
    } else if (rotation == 2) {
      tetrominos->startX -= tetromino.blocWidth * 3;
      tetrominos->startY -= tetromino.blocHeight;
    } else if (rotation == 3) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight * 2;
    }
    break;
  // Tetromino I
  case 2:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY += tetromino.blocHeight;
    }

    break;
  case 3:
    // Aucune rotation pour lui
    break;
  case 4:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth * 2;
      tetrominos->startY -= tetromino.blocHeight;
    } else if (rotation == 2) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight * 2;
    }
    break;
  case 5:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight;
    } else if (rotation == 2) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight * 2;
    }
    break;
  case 6:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth * 2;
      tetrominos->startY -= tetromino.blocHeight;
    } else if (rotation == 2) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight * 3;
    } else if (rotation == 3) {
      tetrominos->startX += tetromino.blocWidth;
      tetrominos->startY -= tetromino.blocHeight * 2;
    }

    break;
  }
}

/**
 * Fonction: Game::Tetromino::transpose
 * ------------------------
 * Transposition de la matrice représentant un tetromino, 
 * ce qui implique une rotation de la matrice.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param rotation Nombre de rotation du tetromino
 * @see Game::Tetromino
 */
void Game::Tetromino::transpose(blocs* tetrominos, int rotation)
{
  Tetromino tetromino;
  /*
   * Différents types de tetrominos
   * 1: L -> 3 rotations
   * 2: I -> 1 rotation
   * 3:
   * 4:
   * */
  blocs transposed;

  // On empêche la rotation si il y a plus de 4 rotation choisie, ce qui ne sert à rien
  if (rotationAmount(tetrominos->type, rotation)) {
    // On fait une itération par rotation (si 0, la boucle n'est donc pas exécutée et il n'y a pas rotation)
    for (int n = 0; n < rotation; n++) {
      for (int x = 0; x < tetromino.maxSize; x++) {
        for (int y = 0; y < tetromino.maxSize; y++) {
          // On fait la transposition en faisant un mirroir au niveau des index y
          transposed.coordinate[y][x] = tetrominos->coordinate[x][tetromino.maxSize - y - 1];
        }
      }
      // On remplace le contenu de tetromino par la transposition - transposed sera ensuite ré-écris avec les nouvelles positions du tetromino
      tetrominos->coordinate = transposed.coordinate;
    }

    // Et enfin on décale le tetromino pour que le déplacement du à la transposition soit transparente
    Tetromino::shift(tetrominos, rotation);
  }
}

/**
 * Fonction: Game::Tetromino::display
 * ------------------------
 * Cette fonction viens afficher un tetromino dans le rendu, afin d'être visible par l'utilisateur.
 *
 * @param renderer Rendu où afficher le tetromino - il y en a forcément un seul ceci dis -
 * @param bloc Texture du bloc à afficher; pour l'instant, tout les tetrominos utilisent le même bloc, donc il faudra revoir ca pour les multiples couleurs
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param rotation Nombre de rotation du tetromino
 * @see Game::Tetromino
 */
void Game::Tetromino::display(SDL_Renderer* renderer, SDL_Texture* bloc, blocs* tetrominos, SDL_Texture* active, SDL_Texture* inactive)
{
  Tetromino tetromino;
  Engine::Texture texture;

  // Pour chaque tetromino
  for (int k = 0; k < lastIndex(tetrominos); k++) {
    // On fouille ses coordonnées, qui est une matrice 6x6
    // Si les coordonnées actuelle correspondent au début d'un tetromino
    // Alors on circule dedans pour construire le tetromino
    for (int x = 0; x < tetromino.maxSize; x++) {
      for (int y = 0; y < tetromino.maxSize; y++) {
        if (tetrominos[k].coordinate[x][y] == 1)
          texture.display(renderer, bloc, tetrominos[k].startX + (tetromino.blocWidth * x), tetrominos[k].startY + (tetromino.blocHeight * y), tetromino.blocWidth, tetromino.blocHeight);
      }
    }
  }
}
