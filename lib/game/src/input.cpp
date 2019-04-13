#include "game/input.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "game/character.hpp"
#include "game/game.hpp"
#include <stdio.h>
#include <stdlib.h>

/**
 * Fonction: Game::Input::Handle
 * -------------------
 * Gère les touches du clavier entrés par le joueur.
 *
 * @see Game::Loop:Run
 */
void Game::Input::handle(Game::Tetromino::blocs* tetrominos, int* max, SDL_Event* event, Game::Character::position* position, bool* left, bool* right, bool* run)
{
  Game::Character character;
  Engine::Collision collision;

  while (SDL_PollEvent(event) != 0) {
    // Si l'utilisateur demande à fermer la fenêtre du jeu
    if (event->type == SDL_QUIT) {
      *run = false;
    }
    // Si l'utilisateur appuis sur une touche
    else if (event->type == SDL_KEYDOWN) {
      // Si une touche est baissée
      switch (event->key.keysym.sym) {
      case SDLK_DOWN:
        break;
      case SDLK_UP:
        character.moveUp(position);
        break;
      case SDLK_LEFT:
        *left = true;
        break;
      case SDLK_RIGHT:
        *right = true;
        break;
      }
    } else if (event->type == SDL_KEYUP) {
      // Si une touche est levée
      switch (event->key.keysym.sym) {
      case SDLK_DOWN:
        break;
      case SDLK_UP:
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
}
