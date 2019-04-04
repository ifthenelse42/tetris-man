#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/error.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "engine/run.hpp"
#include <iostream>

/**
 * Fonction: Engine::Run::SDL
 * -------------------
 * Initialise la librairie SDL.
 *
 * @see main.cpp
 * @see Engine::Error::initSDL
 */
void Engine::Run::SDL()
{
  Engine::Error error;

  error.initSDL();
}

/**
 * Fonction: Engine::Run::loop
 * -------------------
 * Boucle d'exécution du jeu. 
 * Tout le jeu se déroule donc ici.
 *
 * @param renderer Rendu où le jeu est affiché
 * @param window Fenêtre où le rendu/le jeu se trouve
 * @param font Police d'écriture utilisé dans le jeu
 *
 * @see main.cpp
 * @see Engine::Error::initSDL
 * @see Engine::Texture
 * @see Engine::Render
 * @see Engine::Collision
 * @see Game
 */
void Engine::Run::loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  Engine::Texture texture;
  Engine::Render render;
  Engine::Collision collision;
  Game::Tetromino tetromino;

  bool run = true; // Cette variable indique si le jeu est démarré ou non
  int ticks = 0; // Cette variable est incrémenté à chaque itération de la boucle du jeu
  int max = 0; // Variable contenant le nombre max de tetromino, qui est utilisé par plusieurs fonctions dans le code source
  int width = render.width; // Largeur de la zone de jeu - manipulé par la caméra
  int height = render.height; // Hauteur de la zone de jeu - manipulé par la caméra

  /**
   * On fait apparaître un spawner unique qui se déplace de gauche à 
   * droite pour faire apparaître de nouveaux blocs dans la partie. 
   * Ces nouveaux blocs viendront s'imbriquer parfaitement dans la partie.
   */
  Game::Tetromino::spawn spawner;
  Game::Tetromino::spawn* larry = &spawner;
  larry->x1 = render.width - tetromino.blocWidth;
  larry->x2 = larry->x1 + tetromino.blocWidth;
  larry->y1 = 50; // Variable négative car c'est au dessus de l'écran
  larry->y2 = larry->y1 + (tetromino.maxSize * tetromino.blocHeight);

  // On génère un texte
  const char* activeText = "1";
  const char* inactiveText = "0";
  SDL_Texture* active = texture.createText(renderer, { 0, 0, 0 }, font, activeText);
  SDL_Texture* inactive = texture.createText(renderer, { 0, 0, 0 }, font, inactiveText);

  SDL_Event e;
  Game::Tetromino::blocs tetrominos[Game::Tetromino::max];

  // On rajoute un tetromino de type 1
  //tetromino.add(tetrominos, 275, -3900, 5, 0, &max);
  //tetromino.add(tetrominos, 225, -2800, 1, 0, &max);
  //tetromino.add(tetrominos, 200, -1800, 2, 0, &max);
  tetromino.add(tetrominos, 150, -800, 3, 0, &max);
  //tetromino.add(tetrominos, 75, -200, 4, 0, &max);
  //tetromino.add(tetrominos, 0, 0, 2, 1, &max);

  // Maintenant on fait apparaître un tetromino aléatoire s'imbricant avec le tetromino actuel
  SDL_Texture* bloc = texture.createBloc(renderer);

  /**
   * Boucle d'exécution du jeu. 
   * Chaque itération correspond à un frame.
   */
  while (run) {
    if (ticks == 50) {
      tetromino.handleSpawn(tetrominos, larry, &max, &height);
      ticks = 0;
    }

    tetromino.limit(tetrominos, &max, &height);

    render.clear(renderer);

    for (int i = 0; i < max; i++) { // Boucle itérant sur chaque tetromino existant dans la partie
      collision.collide(tetrominos, i, &max, &width, &height);
    }

    while (SDL_PollEvent(&e) != 0) {
      // Si l'utilisateur demande à fermer la fenêtre du jeu
      if (e.type == SDL_QUIT) {
        run = false;
      }
      // Si l'utilisateur appuis sur une touche
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_DOWN:
          break;
        case SDLK_UP:
          break;
        case SDLK_LEFT:
          break;
        case SDLK_RIGHT:
          break;
        }
      }
    }

    tetromino.display(renderer, bloc, tetrominos, active, inactive, &max);
    tetromino.fall(tetrominos, &max);
    SDL_RenderPresent(renderer);

    ticks++;
  }
}

/**
 * Fonction: Engine::Run::close
 * -------------------
 * Lorsque le jeu se ferme, le moteur doit nettoyer la mémoire. 
 * On efface donc de la mémoire le rendu, la fenêtre, la police d'écriture puis la librairie SDL. 
 * Détruire le rendu a aussi comme effet de détruire tout son contenu, par exemple les textures.
 *
 * @param renderer Rendu où le jeu est affiché
 * @param window Fenêtre où le rendu/le jeu se trouve
 * @param font Police d'écriture utilisé dans le jeu
 *
 * @see main.cpp
 */
void Engine::Run::close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}
