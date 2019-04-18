#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "game/game.hpp"
#include "game/tetromino.hpp"
#include "game/character.hpp"
#include <iostream>
#include <math.h>

/**
 * Fonction: Game::Tetromino::speedUp
 * -------------------
 * Cette fonction augmente la vélocité des tetrominos suivant le score actuel du joueur. Cette 
 * fonction gère principalement la difficulté du jeu.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire.
 * @param max Pointeur vers le dernier index des tetrominos.
 * @param speed Pointeur vers la vitesse actuelle des tetrominos.
 * @param score Pointeur vers le score actuel du joueur.
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::speedUp(blocs* tetrominos, int* max, int* speed, int* score)
{
  // Cette variable passe en true si le score + la vitesse du jeu est égale à un montant voulu.
  bool up = false;

  /**
   * Suivant le score, on augmente la vélocité de tous les tetrominos actuels. 
   */

  switch (*score + *speed) {
    case 40:
      up = true;
      break;
    case 150:
      up = true;
      break;
    case 400:
      up = true;
      break;
    case 900:
      up = true;
      break;
    case 1400:
      up = true;
      break;
    case 2000:
      up = true;
      break;
    case 3000:
      up = true;
      break;
    case 4000:
      up = true;
      break;
    case 5000:
      up = true;
      break;
  }

  if (up) {
    (*speed)++;

    for (int k = 0; k < *max; k++) {
      //On ne change que la vélocité des tetrominos qui bougent déjà
      if (tetrominos[k].velocity != 0) {
        tetrominos[k].velocity++;
      }
    }

    // Une fois le changement appliqué, on arrête
    up = false;
  }
}

/**
 * Fonction: Game::Tetromino::fall
 * -------------------
 * Fait tomber les tetrominos.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::fall(blocs* tetrominos, int* max, int* speed)
{
  for (int k = 0; k < *max; k++) {
    tetrominos[k].startY += tetrominos[k].velocity;
  }
}

/**
 * Fonction: Game::Tetromino::firstSpawn
 * -------------------
 * Fait apparaître les premiers tetrominos de la partie.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Pointeur vers le dernier index des tetrominos.
 * @param speed Pointeur vers la vitesse actuelle des tetrominos.
 */
void Game::Tetromino::firstSpawn(blocs* tetrominos, int* max, int* speed, int* height)
{
  Game::Tetromino tetromino;
  //tetromino.add(tetrominos, 275, -1000, 5, 0, max);
  //tetromino.add(tetrominos, 225, -800, 1, 0, max);
  //tetromino.add(tetrominos, 200, -600, 2, 0, max);
  //tetromino.add(tetrominos, 150, -400, 3, 0, max);
  //tetromino.add(tetrominos, 250, -200, 3, 0, max);
  //tetromino.add(tetrominos, 200, -200, 3, 0, max);
  //tetromino.add(tetrominos, 150, -200, 3, 0, max);
  //tetromino.add(tetrominos, 100, -200, 3, 0, max);
  //tetromino.add(tetrominos, 50, -200, 3, 0, max);
  //tetromino.add(tetrominos, 0, -200, 3, 0, max);
  tetromino.add(tetrominos, 125, *height - 50, 2, 1, max, speed);
  tetromino.add(tetrominos, 225, *height - 50, 2, 1, max, speed);
  tetromino.add(tetrominos, 325, *height - 50, 2, 1, max, speed);
  tetromino.add(tetrominos, 425, *height - 50, 2, 1, max, speed);
  tetromino.add(tetrominos, 525, *height - 50, 2, 1, max, speed);

  tetromino.add(tetrominos, 100, -600, 3, 0, max, speed);
  tetromino.add(tetrominos, 150, -1400, 3, 0, max, speed);
  tetromino.add(tetrominos, 200, -2600, 3, 0, max, speed);
  tetromino.add(tetrominos, 250, -3400, 3, 0, max, speed);
  tetromino.add(tetrominos, 300, -1900, 3, 0, max, speed);
  tetromino.add(tetrominos, 350, -400, 3, 0, max, speed);
  tetromino.add(tetrominos, 400, -100, 3, 0, max, speed);
  tetromino.add(tetrominos, 450, -100, 3, 0, max, speed);
  tetromino.add(tetrominos, 500, -800, 3, 0, max, speed);
  tetromino.add(tetrominos, 550, -900, 3, 0, max, speed);
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
void Game::Tetromino::add(blocs* tetrominos, int startX, int startY, int type, int rotation, int* max, int* speed)
{
  int last = *max; // On récupère le dernier index des tetrominos

  // On l'ajoute dans le tableau tetrominos en mémoire
  tetrominos[last].startX = startX;
  tetrominos[last].startY = startY;
  tetrominos[last].type = type;
  tetrominos[last].rotation = 0;
  tetrominos[last].velocity = *speed;
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
void Game::Tetromino::addRandom(Game::Tetromino::blocs* tetrominos, int tetrominoActual, Game::Tetromino::compatible* interlocks, int* max, int* height, int* speed)
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
  int spawnX2 = spawnX1 + (tetromino.maxSize * tetromino.blocWidth);
  int spawnY1 = -((rand() % (1000 + 25) + 25) + 25);
  int spawnY2 = spawnY1 + (tetromino.maxSize * tetromino.blocHeight);
  bool canSpawn = true;

  if (*max < tetromino.max) {
    /**
     * Le tetromino que l'on va faire apparaître ne doit pas toucher un autre qui vient d'apparaître.
     */
    for (int i = 0; i < *max; i++) {
      int allX1 = tetrominos[i].startX;
      int allX2 = allX1 + (tetromino.maxSize * tetromino.blocWidth);
      int allY1 = tetrominos[i].startY;
      int allY2 = allY1 + (tetromino.maxSize * tetromino.blocHeight);

      if (((spawnX2 >= allX1 && allX2 >= spawnX1) && (spawnY2 >= allY1 && allY2 >= spawnY1))) {
        canSpawn = false;
        break;
      }
    }

    if (canSpawn) {
        tetromino.add(tetrominos, spawnX1, spawnY1, interlocks[indexRand].type, interlocks[indexRand].rotation, max, speed);
        tetrominos[tetrominoActual].zombie = true; // le tetromino deviens zombie seulement si un spawn a eu lieu.
    }
  }
}

/**
 * Fonction: Game::Tetromino::fallingTetrominos
 * ------------------------
 * Retourne le nombre de tetrominos tombant dans la partie. 
 * C'est-à-dire qui a la propriété velocity = 0.
 *
 * @param tetrominos Pointeur vers les tetrominos existant dans la partie
 *
 * @return Le nombre de tetrominos tombant
 */
unsigned int Game::Tetromino::fallingTetrominos(Game::Tetromino::blocs* tetrominos, int* max)
{
  int amount = 0;

  for (int i = 0; i < *max; i++) {
    if (tetrominos[i].velocity != 0) {
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
 *
 * @see Game::Tetromino::handleSpawn
 * @see Game::Tetromino::fall
 * @see Game::Run::loop
 */
void Game::Tetromino::spawnDetector(blocs* tetrominos, int* max, int* height, int* speed)
{
  Game::Tetromino tetromino;

  /**
   * On vérifie ici les tetrominos non zombie et tombant, donc ceux qui tombent simplement. 
   * Ca nous permet de faire une vérification sur les tetrominos qui viennent d'apparaître. 
   */
  for (int i = 5; i < *max; i++) {
    int allX1 = tetrominos[i].startX;
    int allX2 = allX1 + (tetromino.maxSize * tetromino.blocWidth);
    int allY1 = tetrominos[i].startY;
    int allY2 = allY1 + (tetromino.maxSize * tetromino.blocHeight);

      //bool Engine::Collision::xCollide(int actualX1, int actualX2, int allX1, int allX2)
    /**
     * On vérifie si le tetromino actuel n'est pas zombie, dans l'affichage, et si 
     * le spawner entre en collision avec lui. Si oui, on peut ajouter le tetromino.
     */
    if (!tetrominos[i].zombie && tetrominos[i].startY >= 0) {
      compatible interlocks[tetromino.maxInterlock];
      tetromino.interlock(tetrominos[i], interlocks, i);

      tetromino.addRandom(tetrominos, i, interlocks, max, height, speed);
    }
  }
}

/**
 * Fonction: Game::Tetromino::handleSpawn
 * ------------------------
 * Gère le spawner de tetrominos.
 *
 * @param tetrominos Pointeur vers le tableau contenant tous les tetrominos dans la partie
 *
 * @see Game::Tetromino::spawn
 */
void Game::Tetromino::handleSpawn(blocs* tetrominos, int* max, int* height, int* speed)
{
  Game::Tetromino tetromino;

  /**
   * On fait bouger de droite à gauche le spawn, 
   * puis on vérifie si un tetromino peut apparaître à l'emplacement où il se trouve. 
   * Si c'est le cas, on fait apparaître le tetromino.
   */
  tetromino.spawnDetector(tetrominos, max, height, speed);
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
  // On ne met pas cette incrémentation dans la boucle des tetrominos, ça n'a aucun sens
  (*height) += amount;

  for (int i = 0; i < *max; i++) {
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
 *
 * @return Si un tetromino touche la limite de la zone d'affichage ou non.
 */
bool Game::Tetromino::limit(blocs* tetrominos, int* max)
{
  bool push = false;

  for (int i = 0; i < *max; i++) {
    /**
     * Le tetromino doit bientôt atteindre la bordure supérieure de l'affichage en étant immobile.
     */
    if (tetrominos[i].startY < 400 && tetrominos[i].velocity == 0) {
      push = true;
      break;
    }
  }

  return push;
}

/**
 * Fonction: Game::Tetromino::clean
 * ------------------------
 * Détecte si un tetromino est maintenant en dehors de l'affichage, donc qu'il est temps de l'effacer de la mémoire.
 *
 * @param tetrominos Pointeur vers un tableau contenant tout les tetrominos en mémoire
 * @param max Pointeur vers le nombre maximum de tetromino en mémoire.
 * @param height Pointeur vers la hauteur de l'affichage
 */
void Game::Tetromino::clean(blocs* tetrominos, int* max, int* height)
{
  // TODO: continuer cette fonction, elle est essentielle pour garder les performances pendant la durée du jeu. Trouver comment nettoyer de la mémoire efficacement un tetromino.
  Engine::Render render;
  int setMax = 0;

  /**
   * On fait une boucle sur tous les tetrominos existant. Si le startY est supérieur à la hauteur de l'affichage, 
   * on remplace tout son contenu par les valeurs par défaut, qui feront donc que le tetromino n'existera plus.
   */
  for (int i = 0; i < *max; i++) {
    if (tetrominos[i].type != 0) {
      setMax++;
    }
    if (tetrominos[i].startY - 200 > render.height) {
      std::cout << "cleaned tetromino " << i << std::endl;
      tetrominos[i].type = 0;
      tetrominos[i].rotation = 0;
      tetrominos[i].coordinate = { { 0, 0, 0, 0}, {0, 0, 0, 0} };
    }
  }

  std::cout << setMax << std::endl;
  *max = setMax;
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
void Game::Tetromino::display(SDL_Renderer* renderer, SDL_Texture* bloc, blocs* tetrominos, int* max)
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
