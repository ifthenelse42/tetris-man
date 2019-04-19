#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "engine/engine.hpp"
#include "engine/error.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "engine/run.hpp"
#include "game/game.hpp"
#include "game/hud.hpp"
#include "game/menu.hpp"
#include "game/tetromino.hpp"
#include <iostream>
#include <string>
#include <sstream>

/**
 * Fonction: Game::HUD::score
 * -------------------
 * Affiche le score du joueur dans le rendu. Cette fonction se trouve dans la boucle 
 * du jeu, donc est chargée à chacune de ses itérations.
 *
 * @param renderer Rendu où afficher le score.
 * @param font Pointeur vers la police d'écriture à utiliser pour afficher le score.
 * @param time Pointeur vers le temps de jeu.
 * @param height Pointeur vers la hauteur actuel du rendu.
 * @param dead Pointeur vers l'interrupteur de mort du joueur
 *
 * @see Game::Run::Loop
 */
int Game::HUD::score(SDL_Renderer* renderer, TTF_Font* scoreFont, TTF_Font* timeFont, float* time, int* speed, bool* dead)
{
  Engine::Texture texture;
  Engine::Render render;

  /**
   * On charge une texture contenant le texte du score, que l'on re-charge donc à chaque 
   * itération de la boucle du jeu, pour afficher le score à jour.
   *
   * En premier, on obtiens le score qui est un entier naturel. 
   * En deuxième, on change la précision du string que l'on va convertir en 'const char*', 
   * pour afficher seulement les chiffres. 
   */
  SDL_Color color = { 0, 0, 0, 255 };
  int scoreNumber = (*time / 1000) * *speed;


  // On défini le timer, en définissant sa précision à 3 chiffres après la virgule.
  std::ostringstream timeRaw;
  timeRaw.precision(3);
  timeRaw << std::fixed << (*time / 1000);
  std::string timeString = timeRaw.str();
  const char* timeText = timeString.c_str();

  // Puis on défini le score.
  std::ostringstream scoreRaw;
  scoreRaw << std::fixed << scoreNumber;
  std::string scoreString = scoreRaw.str();
  const char* scoreText = scoreString.c_str();

  const char* timeTitleText = "Time";
  const char* scoreTitleText = "Score";

  SDL_Texture* timeShow = texture.createText(renderer, color, timeFont, timeText);
  SDL_Texture* timeTitle = texture.createText(renderer, color, timeFont, timeTitleText);

  SDL_Texture* scoreShow = texture.createText(renderer, color, scoreFont, scoreText);
  SDL_Texture* scoreTitle = texture.createText(renderer, color, scoreFont, scoreTitleText);

  // On n'affiche le score et le timer que si on est pas mort
  if (!*dead) {
    /**
     * On affiche le timer.
     */
    texture.displayImage(renderer, timeTitle, render.width - 110, render.height - 25); 
    texture.displayImage(renderer, timeShow, render.width - 40, render.height - 25); 

    /**
     * Puis le score.
     */
    texture.displayImage(renderer, scoreTitle, render.width - 110, render.height - 50); 
    texture.displayImage(renderer, scoreShow, render.width - 40, render.height - 50); 

  }
  // On retourne le score qui est utilisé pour gérer la difficulté
  return scoreNumber;
}

/**
 * Fonction: Game::HUD::gameover
 * -------------------
 * Affiche le message de gameover au joueur.
 *
 * @param renderer Rendu où afficher le score.
 * @param deathTime Temps où le joueur a péri.
 * @param deadFont Pointeur vers une police d'écriture.
 * @param scoreFont Pointeur vers une police d'écriture.
 *
 * @see Game::Run::Loop
 */
void Game::HUD::gameover(SDL_Renderer* renderer, float* deathTime, TTF_Font* deadFont, TTF_Font* bigFont, Game::Menu::button* replay, Game::Menu::button* quit2, int* speed)
{
  Engine::Texture texture;

  /**
   * On paramètre les boutons pour les afficher plus tard.
   */
  SDL_Color active = { 255, 20, 20, 255 };
  SDL_Color inactive = { 40, 40, 40, 255 };

  replay->text = "Replay";
  replay->active = texture.createText(renderer, active, deadFont, replay->text);
  replay->inactive = texture.createText(renderer, inactive, deadFont, replay->text);

  replay->x = 100;
  replay->y = 300;

  quit2->text = "Exit";
  quit2->active = texture.createText(renderer, active, deadFont, quit2->text);
  quit2->inactive = texture.createText(renderer, inactive, deadFont, quit2->text);

  quit2->x = 500;
  quit2->y = 300;

  SDL_Color color = { 60, 0, 0, 255 };
  SDL_Color color2 = { 255, 0, 0, 255 };
  int scoreNumber = (*deathTime / 1000) * *speed;

  std::ostringstream scoreRaw;
  scoreRaw << std::fixed << scoreNumber;
  std::string scoreString = scoreRaw.str();
  const char* scoreText = scoreString.c_str();

  const char* scoreTitleText = "YOU ARE DEAD";

  SDL_Texture* scoreShow = texture.createText(renderer, color2, deadFont, scoreText);
  SDL_Texture* scoreTitle = texture.createText(renderer, color, bigFont, scoreTitleText);

  /**
   * Puis le score.
   */
  texture.displayImage(renderer, scoreTitle, 100, 100); 
  texture.displayImage(renderer, scoreShow, 100, 200); 

  // Et enfin les boutons
  if (replay->selected) {
    texture.displayImage(renderer, replay->active, replay->x, replay->y);
  } else {
    texture.displayImage(renderer, replay->inactive, replay->x, replay->y);
  }

  if (quit2->selected) {
    texture.displayImage(renderer, quit2->active, quit2->x, quit2->y);
  } else {
    texture.displayImage(renderer, quit2->inactive, quit2->x, quit2->y);
  }
}
