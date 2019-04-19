#include "game/input.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "engine/engine.hpp"
#include "engine/render.hpp"
#include "engine/collision.hpp"
#include "game/game.hpp"
#include "game/character.hpp"
#include "game/menu.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/**
 * Fonction: Game::Input::keyDown
 * -------------------
 * Gère la pression d'une touche par l'utilisateur.
 *
 * @param event Pointeur vers les entrées de l'utilisateur.
 * @param left Pointeur vers l'interrupteur 'left' du personnage.
 * @param right Pointeur vers l'interrupteur 'right' du personnage.
 * @param jump Pointeur vers l'interrupteur 'jump' du personnage.
 *
 * @see Game::Input::Handle
 */
void Game::Input::keyDown(SDL_Event* event, bool* left, bool* right, bool* jump)
{
  /**
   * Cette condition était auparavant dans un 'else if', ce qui faisait que soit une touche 
   * était pressée, soit relâchée. Cela provoquait alors un conflit lors la pression/relâchement 
   * de plusieurs touches, notamment pour les sauts.
   *
   * Cela est maintenant corrigé.
   */
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_DOWN:
    break;
    case SDLK_UP:
      *jump = true;
      break;
    case SDLK_LEFT:
      *left = true;
      break;
    case SDLK_RIGHT:
      *right = true;
      break;
    }
  }
}

/**
 * Fonction: Game::Input::keyUp
 * -------------------
 * Gère le relâchement d'une touche par l'utilisateur.
 *
 * @param event Pointeur vers les entrées de l'utilisateur.
 * @param left Pointeur vers l'interrupteur 'left' du personnage.
 * @param right Pointeur vers l'interrupteur 'right' du personnage.
 * @param jump Pointeur vers l'interrupteur 'jump' du personnage.
 *
 * @see Game::Input::Handle
 */
void Game::Input::keyUp(SDL_Event* event, bool* left, bool* right, bool* jump)
{
  /**
   * Même remarque que dans Game::Input::keyDown.
   */
  if (event->type == SDL_KEYUP) {
    switch (event->key.keysym.sym) {
    case SDLK_DOWN:
      break;
    case SDLK_UP:
      *jump = false;
      break;
    case SDLK_LEFT:
      *left = false;
      break;
    case SDLK_RIGHT:
      *right = false;
      break;
    }
  }
}

/**
 * Fonction: Game::Input::quit
 * -------------------
 * Gère la réaction de l'application lorsque l'utilisateur essaye de la fermer, 
 * soit en appuyant sur la croix sur un gestionnaire de fenêtre traditionnel, soit en
 * effectuant la combinaison de touche CTRL-C.
 *
 * @param event Pointeur vers les entrées de l'utilisateur.
 * @param run Pointeur vers l'interrupteur faisant tourner le jeu.
 *
 * @see Game::Input::Handle
 */
void Game::Input::quit(SDL_Event* event, bool* run)
{
  if (event->type == SDL_QUIT) {
    *run = false;
  }
}

/**
 * Fonction: Game::Input::mouseHover
 * -------------------
 * On repère la position de la souris, pour l'interaction avec le menu et ses différents 
 * boutons.
 *
 * @param x Pointeur vers la position X de la souris.
 * @param y Pointeur vers la position Y de la souris.
 */
void Game::Input::mouseHover(int* x, int* y, Game::Menu::button* button)
{
  Engine::Collision collision;

  /**
   * On récupère la largeur et hauteur du bouton start du menu principal. On initialise leur 
   * valeur puis on les remplis à l'aide de SDL_QueryTexture. Ensuite on en déduis les coordonnées 
   * x2 et y2 que l'on va utiliser pour gérer la collision. Sachant que les boutons actifs 
   * et inactifs ont les mêmes tailles, on ne prend que le bouton actif en compte pour 
   * les calculs.
   */
  int startWidth = 0;
  int startHeight = 0;

  SDL_QueryTexture(button->active, NULL, NULL, &startWidth, &startHeight);

  int paddingX = 20; // On rajoute de la marge pour que la zone de survol soit plus large que la texture elle-même
  int paddingY = 10; // On rajoute de la marge pour que la zone de survol soit plus large que la texture elle-même
  int startX2 = button->x + startWidth + paddingX;
  int startY2 = button->y + startHeight + paddingY;

  /**
   * On vérifie maintenant si la souris touche le bouton. Si elle le touche, on 
   * active sa sélection. S'il n'y a pas de collision, on remet/laisse en false.
   */
  if (collision.xCollide(button->x - paddingX, startX2, *x, *x)
      && collision.yCollide(button->y - paddingY, startY2, *y, *y)) {
    button->selected = true;
  } else {
    button->selected = false;
  }
}

/**
 * Fonction: Game::Input::mouseTrack
 * -------------------
 * On repère la position de la souris, pour l'interaction avec le menu.
 *
 * @param button Pointeur vers un bouton.
 */
void Game::Input::mouseTrack(Game::Menu::button* button)
{
  Game::Input input;

  int x = 0;
  int y = 0;

  // On rempli les variables x et y correspondant aux coordonnées du curseur
  SDL_GetMouseState(&x, &y);

  /**
   * On invoque la fonction gérant le survole des boutons des menus.
   */
  input.mouseHover(&x, &y, button);
}

/**
 * Fonction: Game::Input::mouseClick
 * -------------------
 * Détecte un clic de la souris. Pour le menu principal.
 *
 * @param start Pointeur vers le bouton jouer.
 * @param quit Pointeur vers le bouton quitter.
 * @param event Pointeur vers l'entrée de l'utilisateur.
 *
 * @see Game::Menu
 */
void Game::Input::mouseClick(Game::Menu::button* start, Game::Menu::button* quit, Game::Menu::button* replay, Game::Menu::button* quit2, SDL_Event* event, bool* launch, bool* game, bool* dead, bool* run)
{
  Game::Menu menu;
  /**
   * Si on effectue un click et que la souris survole le bouton start, on invoque 
   * la fonction qui le gère
   */
  if (event->type == SDL_MOUSEBUTTONDOWN) {
    if (start->selected) {
      menu.start(launch, game);
    } else if (replay->selected) {
      *launch = true;
      *game = true;
      *dead = false;
    } else if (quit->selected) {
      *run = false;
    } else if (quit2->selected) {
      *run = false;
    }
  }
}

/**
 * Fonction: Game::Input::Handle
 * -------------------
 * Gère les touches du clavier entrés par le joueur.
 *
 * @param tetrominos Pointeur vers l'ensemble des tetrominos présent en mémoire.
 * @param max Pointeur vers le dernier index du tableau des tetrominos.
 * @param event Pointeur vers les entrées de l'utilisateur.
 * @param position Pointeur vers la position du personnage.
 * @param left Pointeur vers l'interrupteur 'left' du personnage.
 * @param right Pointeur vers l'interrupteur 'right' du personnage.
 * @param jump Pointeur vers l'interrupteur 'jump' du personnage.
 * @param run Pointeur vers l'interrupteur faisant tourner le jeu.
 * @param start Pointeur vers le bouton start..
 *
 * @see Game::Loop:Run
 */
void Game::Input::handle(Game::Tetromino::blocs* tetrominos, int* max, SDL_Event* event, Game::Character::position* position, bool* left, bool* right, bool* jump, bool* launch, bool* game, bool* dead, bool* run, Game::Menu::button* start, Game::Menu::button* quit, Game::Menu::button* replay, Game::Menu::button* quit2)
{
  Game::Character character;
  Game::Input input;

  input.mouseTrack(start);
  input.mouseTrack(quit);

  input.mouseTrack(replay);
  input.mouseTrack(quit2);

  while (SDL_PollEvent(event) != 0) {
    // On ne gère la souris que si on est plus dans le menu (donc en jeu)
    if (!*game || *dead) {
      input.mouseClick(start, quit, replay, quit2, event, launch, game, dead, run);
    }
    input.quit(event, run);
    input.keyDown(event, left, right, jump);
    input.keyUp(event, left, right, jump);
  }

  /**
   * Pour des déplacements naturels avec le clavier, les directions gauche et droite 
   * sont des booléens pointés ici, qui dépendent de la pression et le relâchement sur 
   * les touches concernée.
   *
   * On traite maintenant l'activation des déplacements gauche-droite.
   */

  if (*left) {
    character.moveLeft(position);
  }

  if (*right) {
    character.moveRight(position);
  }
  
  if (*jump) {
    character.moveUp(position);
  }
}
