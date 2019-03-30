#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "game/tetromino.hpp"
#include "engine/texture.hpp"
#include "engine/render.hpp"
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
 *
 * @return Le dernier index réel
 *
 * @see Game::Tetromino
 */
int Game::Tetromino::lastIndex(blocs* tetrominos)
{
  int last = 0;

  // Si le type est défini à 0, ca veut dire que ca ne correspond à aucun tetromino
  for (int i = 0; i < Tetromino::max; i++) {
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
 *
 * @see Game::Tetromino
 */
void Game::Tetromino::fall(blocs* tetrominos)
{
  Game::Tetromino tetromino;

  int last = lastIndex(tetrominos);
  for (int k = 0; k < last; k++) {
    if (tetrominos[k].move) {
      if (tetrominos[k].startY + (tetromino.maxSize * tetromino.blocWidth) < 0) {
      tetrominos[k].startY += 25;
      } else {
      tetrominos[k].startY += 1;
      }
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
void Game::Tetromino::add(blocs* tetrominos, int startX, int startY, int type, int rotation)
{
  // On obtiens le dernier index des tetrominos
  int last = lastIndex(tetrominos);

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
}

/**
 * Fonction: Game::Tetromino::addRandom
 * ------------------------
 * Fait apparaître dans le jeu un tetromino s'imbricant parfaitement avec le tetromino actuel. 
 * Il apparaît au dessus du bloc actuel.
 *
 * @param tetrominos Pointeur vers les tetrominos existant dans la partie
 * @param interlocks Pointeur vers les tetrominos compatible avec le tetromino actuel
 *
 * @see Game::Tetromino::interlock
 * @see Game::Tetromino::add
 */
void Game::Tetromino::addRandom(Game::Tetromino::blocs* tetrominos, int tetrominoActual, Game::Tetromino::compatible* interlocks)
{
  Game::Tetromino tetromino;
  int indexRand;

  int i = 0;                              // Sert à suivre l'itération dans les tetrominos compatible
  int indexMax = 0;                       // Correspond au dernier index où se trouve un tetromino compatible

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

  int startX = tetrominos[tetrominoActual].startX + interlocks[indexRand].shiftX; // On déclare cette variable après avoir selectionné aléatoirement un interlock
  int startY = tetromino.interlockStartY; // tout les tetrominos apparaissent hors écran


  // Enfin, le tetromino est inséré dans la mémoire
  tetromino.add(tetrominos, startX, startY, interlocks[indexRand].type, interlocks[indexRand].rotation);
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
unsigned int Game::Tetromino::fallingTetrominos(Game::Tetromino::blocs* tetrominos)
{
  Tetromino tetromino;

  int max = tetromino.lastIndex(tetrominos);
  int amount = 0;

  for (int i = 0; i < max; i++) {
    if (tetrominos[i].move) {
      amount++;
    }
  }

  return amount;
}

/**
 * Fonction: Game::Tetromino::generateRandom
 * ------------------------
 * Fait apparaître dans la partie des tetrominos aléatoirement pour remplir la zone de jeu. 
 * Le joueur doit donc éviter ces tetrominos le plus longtemps possible. 
 * Cette fonction est invoquée tout les x frames.
 *
 * @param tetrominos Pointeur vers les tetrominos existant dans la partie
 * @param interlocks Pointeur vers les tetrominos compatible avec le tetromino actuel
 *
 * @see Game::Tetromino::interlock
 * @see Game::Tetromino::add
 * @see Game::Tetromino::addRandom
 */
void Game::Tetromino::generateRandom(Game::Tetromino::blocs* tetrominos, Game::Tetromino::compatible* interlocks)
{
  //TODO
  Tetromino tetromino;

  int amount = fallingTetrominos(tetrominos); // Nombre de tetrominos en train de tomber
  int last = lastIndex(tetrominos);           // Dernier index où un tetromino est présent

  //srand(time(NULL)); // Nécessaire pour la génération aléatoire
  //indexRand = (rand() % (k + 1) + 1) - 1;
  //tetromino.addRandom(tetrominos, tetrominosNotZombies[indexRand], interlocks);
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

      interlocks[1].shiftX = tetromino.blocWidth;
      interlocks[1].type = 3;
      interlocks[1].rotation = 0;

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
bool Game::Tetromino::spawnDetector(blocs* tetrominos, int index, Game::Tetromino::spawn* larry)
{
  Game::Tetromino tetromino;

  bool res = false;

  /**
   * On vérfie la collision sur les x seulement, puisque de toute manière un tetromino immobile sera forcément 
   * en dessous du spawner.
   */
    if (larry->x1 >= tetrominos[index].startX && larry->x2 <= tetrominos[index].startX + (tetromino.blocWidth * tetromino.maxSize)) {
    if (!tetrominos[index].zombie && !tetrominos[index].move) {
      res = true;
    }
  }
  
  return res;
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
void Game::Tetromino::handleSpawn(blocs* tetrominos, Game::Tetromino::spawn* larry)
{
  Game::Tetromino tetromino;

  int last = tetromino.lastIndex(tetrominos);

  /**
   * On fait bouger de droite à gauche le spawn, 
   * puis on vérifie si un tetromino peut apparaître à l'emplacement où il se trouve. 
   * Si c'est le cas, on fait apparaître le tetromino.
   */
  tetromino.moveSpawn(larry);

  /**
   * On fait une boucle dans tous les tetrominos présent dans la mémoire, 
   * puis on regarde si la matrice est en dessous du spawner, sans vérifier les blocs. 
   * S'il y en a un, immobile et non-zombie, on invoque addRandom et on le fait devenir zombie.
   */
  for (int i = 0; i < last; i++) {
  if (tetromino.spawnDetector(tetrominos, i, larry)) {
    compatible interlocks[tetromino.maxInterlock];
    tetromino.interlock(tetrominos[i], interlocks, i);

    tetrominos[i].zombie = true;
    tetromino.addRandom(tetrominos, i, interlocks);
  }
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
void Game::Tetromino::display(SDL_Renderer* renderer, SDL_Texture* bloc, blocs* tetrominos, SDL_Texture* active, SDL_Texture* inactive)
{
  Tetromino tetromino;
  Engine::Texture texture;

  int last = tetromino.lastIndex(tetrominos);
  std::cout << last << std::endl;

  // Pour chaque tetromino
  for (int k = 0; k < last; k++) {
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
