#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "game/game.hpp"
#include "game/tetromino.hpp"
#include <iostream>
#include <math.h>

/**
 * Fonction: Game::Tetromino::fall
 * -------------------
 * Fait tomber les tetrominos.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::fall(blocs* tetrominos, int* max)
{
  for (int k = 0; k < *max; k++) {
    if (tetrominos[k].move) {
      tetrominos[k].startY += 5;
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
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::add(blocs* tetrominos, int startX, int startY, int type, int rotation, int* max)
{
  int last = *max; // On récupère le dernier index des tetrominos

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].startX = startX;
  tetrominos[last].startY = startY;
  tetrominos[last].type = type;
  tetrominos[last].rotation = 0;
  tetrominos[last].move = true;
  tetrominos[last].zombie = false;

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

  // Et on incrémente la variable max qui sert à donner le nombre maximum de tetromino.
  (*max)++;
}

/**
 * Fonction: Game::Tetromino::addRandom
 * ------------------------
 * Fait apparaître dans le jeu un tetromino s'imbricant parfaitement avec le tetromino actuel. 
 * Il apparaît au dessus du bloc actuel.
 *
 * @param tetrominos Copie mémoire du tableau pointant vers les tetrominos existant dans la partie. Afin d'éviter de faire tourner la boucle sur les nouveaux tetrominos.
 * @param interlocks Pointeur vers les tetrominos compatible avec le tetromino actuel
 *
 * @see Game::Tetromino::interlock
 * @see Game::Tetromino::add
 */
void Game::Tetromino::addRandom(Game::Tetromino::blocs* tetrominos, int tetrominoActual, Game::Tetromino::compatible* interlocks, int* max, int* height)
{
  Game::Tetromino tetromino;
  Engine::Collision collision;
  Engine::Render render;
  int indexRand;

  int i = 0;        // Sert à suivre l'itération dans les tetrominos compatible
  int indexMax = 0; // Correspond au dernier index où se trouve un tetromino compatible

  /**
   * On fait une boucle sur toute les imbrications possible du tetromino actuel, pour obtenir 
   * l'index minimum et l'index maximum où se trouve une imbrication possible. 
   * Ensuite, on prendra cette intervalle pour piocher dedans une imbrication au hasard.
   */
  while (interlocks[i].type != 0 && i < tetromino.maxInterlock) {
    i++;
  }
  indexMax = i - 1; // Après l'arrêt de la boucle, on peut récupérer le dernier index

  /**
   * On choisit maintenant un index aléatoire. 
   * On doit absolument prendre une intervalle entre 0 et le dernier index
   */
  srand(time(NULL)); // Nécessaire pour la génération aléatoire
  indexRand = (rand() % (indexMax + 1) + 1) - 1;

  // On récupère les coordonnées du tetromino actuel
  int spawnX1 = tetrominos[tetrominoActual].startX + interlocks[indexRand].shiftX;
  int spawnX2 = spawnX1 + (tetromino.maxSize * tetromino.blocWidth * 2);
  int spawnY1 = 0;
  int spawnY2 = spawnY1 + (tetromino.maxSize * tetromino.blocHeight * 2);
  bool canSpawn = true;

  // TODO: arrêter de spawn un random pour tout les tetrominos entre eux et entre les nouveaux spawn
  if (*max < 3) {
    /**
     * Le tetromino que l'on va faire apparaître ne doit pas toucher un autre qui vient d'apparaître.
     */
    for (int i = 0; i < *max; i++) {
      if (i != tetrominoActual) {
        // TODO CONTINUER LES COLLISIONS
      int allX1 = tetrominos[i].startX;
      int allX2 = allX1 + (tetromino.maxSize * tetromino.blocWidth);
      int allY1 = tetrominos[i].startY;
      int allY2 = allY1 + (tetromino.maxSize * tetromino.blocHeight);
      std::cout << "spawnY1: " << spawnY1 << " - spawnX1: " << spawnX1 << std::endl;
      std::cout << "spawnY2: " << spawnY2 << " - spawnX2: " << spawnX2 << std::endl;

      std::cout << "tetris " << i << std::endl;

      std::cout << "allY1: " << allY1 << " - allX1: " << allX1 << std::endl;
      std::cout << "allY2: " << allY2 << " - allX2: " << allX2 << std::endl;
      std::cout << "test: " << allY1 + (tetromino.maxSize * tetromino.blocWidth) << std::endl;

      // TODO - la collision des X entre eux n'est pas bonne, c'est ce qui empêche tout de fonctionner correctement! - égalemement pour Y, il suffit de corriger ca!
      if (((spawnX2 > allX1 && allX2 > spawnX1) && (spawnY2 >= allY1 && allY2 >= spawnY1))) {
        std::cout << "COLLISION" << std::endl;
        canSpawn = false;
        break;
      }
      }
    }

    if (canSpawn) {
        std::cout << "SPAWN RANDOM - height: " << *height << std::endl;
        tetromino.add(tetrominos, spawnX1, spawnY1, interlocks[indexRand].type, interlocks[indexRand].rotation, max);
        tetrominos[tetrominoActual].zombie = true; // le tetromino deviens zombie seulement si un spawn a eu lieu.
    }
  }
}

/**
 * Fonction: Game::Tetromino::fallingTetrominos
 * ------------------------
 * Retourne le nombre de tetrominos tombant dans la partie. 
 * C'est-à-dire qui a la propriété .move = true.
 *
 * @param tetrominos Pointeur vers les tetrominos existant dans la partie
 *
 * @return Le nombre de tetrominos tombant
 */
unsigned int Game::Tetromino::fallingTetrominos(Game::Tetromino::blocs* tetrominos, int* max)
{
  int amount = 0;

  for (int i = 0; i < *max; i++) {
    if (tetrominos[i].move) {
      amount++;
    }
  }

  return amount;
}

/**
 * Fonction: Game::Tetromino::rotationAmount
 * ------------------------
 * Renvoie en tant que condition le nombre de rotation possible d'un tetromino, selon son type.
 *
 * @param type Type du tetromino (ce qui défini donc sa forme)
 * @param rotation Nombre de rotation initiale du tetromino
 *
 * @return Condition d'intervalle sur le nombre de rotation possible
 *
 * @see Game::Tetromino
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
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::shift(blocs* tetrominos, int rotation)
{
  Tetromino tetromino;

  switch (tetrominos->type) {
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
  case 2:
    if (rotation == 1) {
      tetrominos->startX -= tetromino.blocWidth;
      tetrominos->startY += tetromino.blocHeight;
    }

    break;
  case 3:
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
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::transpose(blocs* tetrominos, int rotation)
{
  Tetromino tetromino;
  /*
   * Différents types de tetrominos
   * 1: L -> 3 rotations
   * 2: I -> 1 rotation
   * 3: O -> 0 rotation
   * 4: s -> 1 rotation
   * 5: z -> 1 rotation
   * 6: T -> 3 rotations
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

    // Puis on met à jour la structure du tetromino pour refléter la nouvelle rotation - qui est effectuée une seule fois dans la vie d'un tetromino
    tetrominos->rotation = rotation;
  }
}

/**
 * Fonction: Game::Tetromino::interlock
 * ------------------------
 * Renvoie un tableau contenant les types et rotation des tetrominos s'imbricant
 * avec le tetromino actuel. 
 * Cette fonction servira pour l'apparition aléatoire de tetrominos.
 *
 * @param tetrominos Pointeur vers le tetromino actuel
 *
 * @return Tableau pointant vers les tetrominos compatible avec le tetromino actuel
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::interlock(blocs tetrominos, compatible* interlocks, int index)
{
  Game::Tetromino tetromino;          // On déclare un objet pour accéder à la classe Tetromino
  int type = tetrominos.type;         // On récupère le type du tetromino
  int rotation = tetrominos.rotation; // Et sa rotation

  // On regarde le type du tetromino actuel
  switch (type) {
  case 1:
    // Et on voit sa rotation également
    switch (rotation) {
    case 0:
      /*
       * Par exemple: dans le cas où le type est 1 et qu'il n'y a aucune rotation, 
       * on fournis une liste de tetrominos s'imbricant avec celui-ci. 
       * Il faut également ajouter un decalage X/Y pour mettre une imbrication parfaite selon le type et la rotation.
       */
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 1;
      interlocks[0].rotation = 2;

      break;
    case 1:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 6;
      interlocks[0].rotation = 2;
      break;
    case 2:
      interlocks[0].shiftX = tetromino.blocWidth * 2;
      interlocks[0].type = 1;
      interlocks[0].rotation = 0;

      interlocks[1].shiftX = tetromino.blocWidth * 2;
      interlocks[1].type = 3;
      interlocks[1].rotation = 0;
      break;
    case 3:
      interlocks[0].shiftX = 0;
      interlocks[0].type = 1;
      interlocks[0].rotation = 0;

      interlocks[1].shiftX = 0;
      interlocks[1].type = 6;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = tetromino.blocWidth;
      interlocks[2].type = 1;
      interlocks[2].rotation = 1;

      interlocks[3].shiftX = 0;
      interlocks[3].type = 4;
      interlocks[3].rotation = 0;
      break;
    }
    break;
  case 2:
    switch (rotation) {
    case 0:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 1;
      interlocks[0].rotation = 2;

      interlocks[1].shiftX = 0;
      interlocks[1].type = 2;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = 0;
      interlocks[2].type = 4;
      interlocks[2].rotation = 1;

      interlocks[3].shiftX = -(tetromino.blocWidth * 2);
      interlocks[3].type = 5;
      interlocks[3].rotation = 1;

      break;
    case 1:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 2;
      interlocks[0].rotation = 1;

      interlocks[1].shiftX = tetromino.blocWidth;
      interlocks[1].type = 4;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = 0;
      interlocks[2].type = 5;
      interlocks[2].rotation = 0;
      break;
    }
    break;
  case 3:
    interlocks[0].shiftX = 0;
    interlocks[0].type = 3;
    interlocks[0].rotation = 0;

    interlocks[1].shiftX = 0;
    interlocks[1].type = 1;
    interlocks[1].rotation = 0;

    break;
  case 4:
    switch (rotation) {
    case 0:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 1;
      interlocks[0].rotation = 1;

      interlocks[1].shiftX = tetromino.blocWidth;
      interlocks[1].type = 3;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = 0;
      interlocks[2].type = 6;
      interlocks[2].rotation = 2;
      break;
    case 1:
      interlocks[0].shiftX = tetromino.blocWidth * 2;
      interlocks[0].type = 4;
      interlocks[0].rotation = 1;

      interlocks[1].shiftX = tetromino.blocWidth * 3;
      interlocks[1].type = 1;
      interlocks[1].rotation = 2;

      break;
    }
    break;
  case 5:
    switch (rotation) {
    case 0:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 4;
      interlocks[0].rotation = 0;

      break;
    case 1:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 5;
      interlocks[0].rotation = 1;

      interlocks[1].shiftX = tetromino.blocWidth * 2;
      interlocks[1].type = 2;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = tetromino.blocWidth * 3;
      interlocks[2].type = 1;
      interlocks[2].rotation = 1;

      break;
    }
    break;
  case 6:
    switch (rotation) {
    case 0:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 1;
      interlocks[0].rotation = 3;

      interlocks[1].shiftX = 0;
      interlocks[1].type = 4;
      interlocks[1].rotation = 0;

      interlocks[2].shiftX = 0;
      interlocks[2].type = 5;
      interlocks[2].rotation = 0;

      interlocks[3].shiftX = tetromino.blocWidth;
      interlocks[3].type = 2;
      interlocks[3].rotation = 1;
      break;
    case 1:
      interlocks[0].shiftX = tetromino.blocWidth;
      interlocks[0].type = 5;
      interlocks[0].rotation = 1;

      interlocks[1].shiftX = tetromino.blocWidth * 3;
      interlocks[1].type = 1;
      interlocks[1].rotation = 1;
      break;
    case 2:
      interlocks[0].shiftX = tetromino.blocWidth * 2;
      interlocks[0].type = 1;
      interlocks[0].rotation = 1;
      break;
    case 3:
      interlocks[0].shiftX = 0;
      interlocks[0].type = 4;
      interlocks[0].rotation = 1;
      break;
    }
    break;
  }
}

/**
 * Fonction: Game::Tetromino::spawnDetector
 * ------------------------
 * Détecte s'il y a un bloc immobile et non-zombie en dessous du spawner. 
 * On vérifie simplement une collision entre les points x1 et x2, puisque 
 * un bloc immobile et non-zombie sera forcément en dessous du spawner.
 *
 * Il suffira de modifier cette fonction, la fonction fall et le nombre de tick par invocation de handleSpawn 
 * pour définir la difficulté du jeu.
 *
 * @param tetrominos Pointeur vers le tableau contenant tous les tetrominos dans la partie
 * @param larry Spawner de tetromino
 *
 * @see Game::Tetromino::handleSpawn
 * @see Game::Tetromino::fall
 * @see Game::Run::loop
 */
void Game::Tetromino::spawnDetector(blocs* tetrominos, Game::Tetromino::spawn* larry, int* max, int* height)
{
  Game::Tetromino tetromino;

  /**
   * On vérifie ici les tetrominos non zombie et tombant, donc ceux qui tombent simplement. 
   * Ca nous permet de faire une vérification sur les tetrominos qui viennent d'apparaître. 
   */
  for (int i = 0; i < *max; i++) {
    if (!tetrominos[i].zombie) {
      compatible interlocks[tetromino.maxInterlock];
      tetromino.interlock(tetrominos[i], interlocks, i);

      tetromino.addRandom(tetrominos, i, interlocks, max, height);
    }
  }
}

/**
 * Fonction: Game::Tetromino::moveSpawn
 * ------------------------
 * Fait bouger de gauche à droite le spawner.
 *
 * @param larry Spawner de tetromino
 *
 * @see Game::Tetromino::handleSpawn
 */
void Game::Tetromino::moveSpawn(Game::Tetromino::spawn* larry)
{
  Game::Tetromino tetromino;
  Engine::Render render;

  /**
   * Si le x1 du spawner va en dessous de 0, on change de sens. 
   * Si le x2 du spawner va au dessus de render.width, on change de sens.
   */
  if (larry->goesLeft) {
    if (larry->x1 - tetromino.blocWidth < 0) {
      larry->goesLeft = !larry->goesLeft;
    } else {
      larry->x1 -= tetromino.blocWidth;
      larry->x2 = larry->x1 + tetromino.blocWidth;
    }
  } else {
    if (larry->x2 > render.width) {
      larry->goesLeft = !larry->goesLeft;
    } else {
      larry->x1 += tetromino.blocWidth;
      larry->x2 = larry->x1 + tetromino.blocWidth;
    }
  }
}

/**
 * Fonction: Game::Tetromino::handleSpawn
 * ------------------------
 * Gère le spawner de tetrominos.
 *
 * @param tetrominos Pointeur vers le tableau contenant tous les tetrominos dans la partie
 * @param larry Spawner de tetromino
 *
 * @see Game::Tetromino::spawn
 */
void Game::Tetromino::handleSpawn(blocs* tetrominos, Game::Tetromino::spawn* larry, int* max, int* height)
{
  Game::Tetromino tetromino;

  /**
   * On fait bouger de droite à gauche le spawn, 
   * puis on vérifie si un tetromino peut apparaître à l'emplacement où il se trouve. 
   * Si c'est le cas, on fait apparaître le tetromino.
   */
  tetromino.moveSpawn(larry);
  tetromino.spawnDetector(tetrominos, larry, max, height);
}

/**
 * Fonction: Game::Tetromino::moveAllUp
 * ------------------------
 * Cette fonction sert à bouger tous les tetrominos en mémoire vers le haut dans le rendu. Cela sert à simuler une camera. 
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Pointeur vers le nombre maximum de tetromino en mémoire.
 */
void Game::Tetromino::moveAllUp(blocs* tetrominos, int amount, int* max, int* height)
{
  for (int i = 0; i < *max; i++) {
    (*height)++;
    tetrominos[i].startY += amount;
  }
}

/**
 * Fonction: Game::Tetromino::limit
 * ------------------------
 * Détecte si un tetromino parmis tous a atteint la limite supérieure de l'affichage. 
 * Si oui, on monte la caméra. Le joueur doit être capable de monter, sinon il perd.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Pointeur vers le nombre maximum de tetromino en mémoire.
 */
void Game::Tetromino::limit(blocs* tetrominos, int* max, int* height)
{
  Game::Tetromino tetromino;

  bool push = false;
  for (int i = 0; i < *max; i++) {
    /**
     * Le tetromino doit bientôt atteindre la bordure supérieure de l'affichage en étant immobile.
     */
    if (tetrominos[i].startY < 200 && !tetrominos[i].move) {
      push = true;
      break;
    }
  }

  if (push) {
    tetromino.moveAllUp(tetrominos, 2, max, height);
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
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::display(SDL_Renderer* renderer, SDL_Texture* bloc, blocs* tetrominos, SDL_Texture* active, SDL_Texture* inactive, int* max)
{
  Tetromino tetromino;
  Engine::Texture texture;

  // Pour chaque tetromino
  for (int k = 0; k < *max; k++) {
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
