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

  bool run = true;
  int ticks = 0;

  /**
   * On fait apparaître un spawner unique qui se déplace de gauche à 
   * droite pour faire apparaître de nouveaux blocs dans la partie. 
   * Ces nouveaux blocs viendront s'imbriquer parfaitement dans la partie.
   */
  Game::Tetromino::spawn spawner;
  Game::Tetromino::spawn* larry = &spawner;
  larry->x1 = render.width - tetromino.blocWidth;
  larry->x2 = larry->x1 + tetromino.blocWidth;

  // On génère un texte
  const char* activeText = "1";
  const char* inactiveText = "0";
  SDL_Texture* active = texture.createText(renderer, { 0, 0, 0 }, font, activeText);
  SDL_Texture* inactive = texture.createText(renderer, { 0, 0, 0 }, font, inactiveText);

  SDL_Event e;
  Game::Tetromino::blocs tetrominos[Game::Tetromino::max];
  Game::Tetromino::compatible interlocks[Game::Tetromino::maxInterlock]; // On initialise la structure pointant vers les tetrominos imbricable avec le tetromino actuel

  // On rajoute un tetromino de type 1
  tetromino.add(tetrominos, 225, 0, 4, 0);
  tetromino.add(tetrominos, 225, 300, 2, 0);

  // Maintenant on fait apparaître un tetromino aléatoire s'imbricant avec le tetromino actuel
  SDL_Texture* bloc = texture.createBloc(renderer);

  /**
   * Boucle d'exécution du jeu. 
   * Chaque itération correspond à un frame.
   */
  while (run) {
    std::cout << "Larry x1: " << larry->x1 << std::endl;
    std::cout << "Larry x2: " << larry->x2 << std::endl;

    if (ticks == 50) {
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
      // On invoque la génération aléatoire de tetromino
      tetromino.generateRandom(tetrominos, interlocks);
      ticks = 0;
    }

    render.clear(renderer);
    int last = tetromino.lastIndex(tetrominos); // On récupère le dernier index utilisé dans le tableau contenant tout les tetrominos

    for (int i = 0; i < last; i++) { // Boucle itérant sur chaque tetromino existant dans la partie

      collision.collide(tetrominos, i);
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

    tetromino.display(renderer, bloc, tetrominos, active, inactive);
    tetromino.fall(tetrominos);
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
