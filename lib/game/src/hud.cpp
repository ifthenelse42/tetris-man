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
 *
 * @see Game::Run::Loop
 */
int Game::HUD::score(SDL_Renderer* renderer, TTF_Font* scoreFont, TTF_Font* timeFont, float* time, int* speed)
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

  // On retourne le score qui est utilisé pour gérer la difficulté
  return scoreNumber;
}
