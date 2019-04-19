#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "engine/engine.hpp"
#include "engine/texture.hpp"
#include "engine/render.hpp"
#include "game/game.hpp"
#include "game/menu.hpp"
#include <iostream>

/**
 * Fonction: Game::Menu::init
 * -------------------
 * Charge le contenu du menu principal.
 *
 * @param renderer Rendu où afficher le score.
 * @param menuFont Pointeur vers la police d'écriture à utiliser pour afficher les boutons du menu.
 */
void Game::Menu::init(SDL_Renderer* renderer, Game::Menu::button* start,  Game::Menu::button* quit, TTF_Font* menuFont)
{
  Engine::Texture texture;
  Engine::Render render;

  /**
   * On initialise les couleurs utilisées pour les boutons survolés et non-survolés. 
   *
   * Ensuite on initialise les boutons du menu principal.
   */
  SDL_Color active = { 255, 20, 20, 255 };
  SDL_Color inactive = { 40, 40, 40, 255 };

  start->text = "Play";
  start->active = texture.createText(renderer, active, menuFont, start->text);
  start->inactive = texture.createText(renderer, inactive, menuFont, start->text);

  quit->text = "Exit";
  quit->active = texture.createText(renderer, active, menuFont, quit->text);
  quit->inactive = texture.createText(renderer, inactive, menuFont, quit->text);

  /**
   * On récupère les dimensions de la texture du bouton start, car on voit s'assurer qu'il soit 
   * bien centré.
   */
  int startWidth = 0;
  int startHeight = 0;
  int quitWidth = 0;
  int quitHeight = 0;

  SDL_QueryTexture(start->active, NULL, NULL, &startWidth, &startHeight);
  SDL_QueryTexture(start->active, NULL, NULL, &quitWidth, &quitHeight);

  start->x = (render.width / 2) - startWidth / 2;
  start->y = (render.height / 2 + 1/2) - startHeight;

  quit->x = (render.width / 2) - quitWidth / 2;
  quit->y = (start->y + 200) - quitHeight;
}

/**
 * Fonction: Game::Menu::index
 * -------------------
 * Menu principal lors du lancement du jeu. Cette fonction sert à l'afficher dans la boucle 
 * d'exécution du jeu.
 *
 * @param renderer Rendu où afficher le score.
 * @param menuFont Pointeur vers la police d'écriture à utiliser pour afficher les boutons du menu.
 */
void Game::Menu::index(SDL_Renderer* renderer, Game::Menu::button* start, Game::Menu::button* quit, TTF_Font* menuFont)
{
  Engine::Texture texture;
  
  if (start->selected) {
    texture.displayImage(renderer, start->active, start->x, start->y);
  } else {
    texture.displayImage(renderer, start->inactive, start->x, start->y);
  }
  
  if (quit->selected) {
    texture.displayImage(renderer, quit->active, quit->x, quit->y);
  } else {
    texture.displayImage(renderer, quit->inactive, quit->x, quit->y);
  }
}

/**
 * Fonction: Game::Menu::start
 * -------------------
 * Gère l'événement lorsque l'on clique sur le bouton start.
 *
 * @param launch Pointeur vers l'interrupteur de démarrage du jeu.
 * @param game Pointeur vers l'exécution de la partie.
 */
void Game::Menu::start(bool* launch, bool* game)
{
  *launch = true;
  *game = true;
}
