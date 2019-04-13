#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "engine/error.hpp"
#include "game/game.hpp"
#include "game/character.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/**
 * Fonction: Game::Character::create
 * -------------------
 * Créé le personnage à partir d'une image donnée, et retourne une texture avec.
 *
 * @param renderer Rendu où affecter la texture.
 * @param image Localisation de l'image à utiliser.
 * @param position Pointeur vers les coordonnées du personnage.
 *
 * @return La texture créé depuis l'image.
 *
 * @see Game::Loop:Run
 */
SDL_Texture* Game::Character::create(SDL_Renderer* renderer, SDL_Surface* image, Game::Character::position* position)
{
  Engine::Error error;

  SDL_Surface* imagePersonnage = IMG_Load("assets/image/perso.png");
  SDL_Texture* personnage = SDL_CreateTextureFromSurface(renderer, imagePersonnage);

  error.textureLoad(personnage);
  
  // On récupère les dimensions de l'image pour renseigner les coordonnées du personnage
  int width = 0;
  int height = 0;

  SDL_QueryTexture(personnage, NULL, NULL, &width, &height);
  position->x = 300;
  position->width = width;

  position->y = 0;
  position->height = height;

  std::cout << height << std::endl;

  return personnage;
}

/**
 * Fonction: Game::Character::display
 * -------------------
 * Affiche le personnage.
 *
 * @param renderer Rendu où afficher le personnage.
 * @param personnage Personnage à afficher.
 *
 * @see Game::Loop:Run
 */
void Game::Character::display(SDL_Renderer* renderer, SDL_Texture* personnage, Game::Character::position* position)
{
  Engine::Texture texture;

  texture.displayImage(renderer, personnage, position->x, position->y);
}

/**
 * Fonction: Game::Character::handleVelocity
 * -------------------
 * Gère la vélocité du personnage, pour la graviter ou les déplacements. 
 * Cette fonction viens également arrêter les vélocités s'il y a collision.
 *
 * @param tetrominos Pointeur vers les tetrominos présent dans le jeu.
 * @param max Pointeur vers le nombre maximum de tetromino présent dans le jeu.
 * @param position Pointeur vers les coordonnées du personnage.
 * @param run Pointeur vers le booléen run, qui s'il devient false, arrête le jeu.
 */
void Game::Character::handleVelocity(Game::Tetromino::blocs* tetrominos, int* max, Game::Character::position* position, bool* run)
{
  Engine::Collision collision;

  /**
   * Chaque vélocité se réduira toute seul à chaque invocation de cette fonction. 
   * Cela permet de faire un effet de glissement pour les déplacements x et des chutes 
   * fluides pour les déplacements y.
   */
  Game::Character::position nextX;
  nextX.x = position->x + position->velocityX; // ici on ajoute la position suivis de la prochaine vélocité, pour calculer la collision sur ce prochain mouvement
  nextX.y = position->y + position->velocityY;
  nextX.width = position->width;
  nextX.height = position->height;
  nextX.velocityX = position->velocityX;
  nextX.velocityY = position->velocityY;

  if (!collision.collideCharacterY1(tetrominos, max, &nextX, run)) {
    if (position->velocityX > 0) {
      position->velocityX--;
    } else if (position->velocityX < 0) {
      position->velocityX++;
    }
  } else {
    position->velocityX = 0;
  }

  /**
   * Ici on vérifie les chûtes.
   */
  Game::Character::position nextY;
  nextY.x = position->x;
  nextY.y = position->y + position->velocityY; // On prédis la prochaine position ici
  nextY.width = position->width;
  nextY.height = position->height;
  nextY.velocityX = position->velocityX;
  nextY.velocityY = position->velocityY;
  std::cout << "NextY : " << nextY.y << std::endl;

  if (!collision.collideCharacter(tetrominos, max, &nextY, run)) {
    if (position->velocityY >= 0 && position->velocityY <= 5) {
      position->velocityY++;
    } else if (position->velocityY <= 0 && position->velocityY >= -50) {
      position->velocityY++;
    }
  } else {
    position->velocityY = 0;
  }
}

/**
 * Fonction: Game::Character::movement
 * -------------------
 * Utilise la vélocité du personnage pour le déplacer dans le jeu.
 *
 * @param position Pointeur vers les coordonnées du personnage.
 */
void Game::Character::movement(Game::Character::position* position)
{
  /**
   * On modifie les x et y du personnage en fonction des vélocités
   */
  position->x += position->velocityX;
  position->y += position->velocityY;
}

/**
 * Fonction: Game::Character::moveLeft
 * -------------------
 * Déplace le personnage vers la gauche.
 *
 * @param position Pointeur vers les coordonnées du personnage.
 */
void Game::Character::moveLeft(Game::Character::position* position)
{
  position->velocityX = -5;
}

/**
 * Fonction: Game::Character::moveRight
 * -------------------
 * Déplace le personnage vers la droite.
 *
 * @param position Pointeur vers les coordonnées du personnage.
 */
void Game::Character::moveRight(Game::Character::position* position)
{
  position->velocityX = 5;
}

/**
 * Fonction: Game::Character::moveUp
 * -------------------
 * Fait sauter le personnage.
 *
 * @param position Pointeur vers les coordonnées du personnage.
 */
void Game::Character::moveUp(Game::Character::position* position)
{
  // On ne peut sauter que si on est pas en train de tomber ou en plein saut
  if (position->velocityY == 0) {
    std::cout << "Jump!" << std::endl;
    position->velocityY = -20;
  }
}
