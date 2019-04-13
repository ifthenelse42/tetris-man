#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/error.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "engine/run.hpp"
#include "game/game.hpp"
#include "game/input.hpp"
#include "game/character.hpp"
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
  Game::Character character;
  Game::Input input;

  bool left = false; // Variable activant le mouvement vers la gauche
  bool right = false; // Variable activant le mouvement vers la droite
  bool run = true; // Cette variable indique si le jeu est démarré ou non
  int ticks = 0; // Cette variable est incrémenté à chaque itération de la boucle du jeu
  int max = 0; // Variable contenant le nombre max de tetromino, qui est utilisé par plusieurs fonctions dans le code source
  int height = render.height; // Hauteur de la zone de jeu - manipulé par la caméra
  Game::Character::position position; // On initialise la position du personnage
  SDL_Texture* personnage = character.create(renderer, IMG_Load("assets/image/perso.png"), &position); // On créé le personnage joué par le joueur

  // On génère un texte
  const char* activeText = "1";
  const char* inactiveText = "0";
  SDL_Texture* active = texture.createText(renderer, { 0, 0, 0 }, font, activeText);
  SDL_Texture* inactive = texture.createText(renderer, { 0, 0, 0 }, font, inactiveText);

  SDL_Event event;
  Game::Tetromino::blocs tetrominos[Game::Tetromino::max];

  // On rajoute un tetromino de type 1
  //tetromino.add(tetrominos, 275, -1000, 5, 0, &max);
  //tetromino.add(tetrominos, 225, -800, 1, 0, &max);
  //tetromino.add(tetrominos, 200, -600, 2, 0, &max);
  //tetromino.add(tetrominos, 150, -400, 3, 0, &max);
  //tetromino.add(tetrominos, 250, -200, 3, 0, &max);
  //tetromino.add(tetrominos, 200, -200, 3, 0, &max);
  //tetromino.add(tetrominos, 150, -200, 3, 0, &max);
  //tetromino.add(tetrominos, 100, -200, 3, 0, &max);
  //tetromino.add(tetrominos, 50, -200, 3, 0, &max);
  //tetromino.add(tetrominos, 0, -200, 3, 0, &max);
  tetromino.add(tetrominos, 125, height - 50, 2, 1, &max);
  tetromino.add(tetrominos, 225, height - 50, 2, 1, &max);
  tetromino.add(tetrominos, 325, height - 50, 2, 1, &max);
  tetromino.add(tetrominos, 425, height - 50, 2, 1, &max);
  tetromino.add(tetrominos, 525, height - 50, 2, 1, &max);

  tetromino.add(tetrominos, 100, -600, 3, 0, &max);
  tetromino.add(tetrominos, 150, -1400, 3, 0, &max);
  tetromino.add(tetrominos, 200, -2600, 3, 0, &max);
  tetromino.add(tetrominos, 250, -3400, 3, 0, &max);
  tetromino.add(tetrominos, 300, -1900, 3, 0, &max);
  tetromino.add(tetrominos, 350, -400, 3, 0, &max);
  tetromino.add(tetrominos, 400, -100, 3, 0, &max);
  tetromino.add(tetrominos, 450, -200, 3, 0, &max);
  tetromino.add(tetrominos, 500, -800, 3, 0, &max);
  tetromino.add(tetrominos, 550, -900, 3, 0, &max);

  // Maintenant on fait apparaître un tetromino aléatoire s'imbricant avec le tetromino actuel
  SDL_Texture* bloc = texture.createBloc(renderer);

  /**
   * Boucle d'exécution du jeu. 
   * Chaque itération correspond à un frame.
   */
  while (run) {
    if (ticks == 10) {
      tetromino.handleSpawn(tetrominos, &max, &height);
      ticks = 0;
    }

    // On balaye l'affichage
    render.clear(renderer);


    /**
     * Invocation des fonctions affectant les entrées de l'utilisateur.
     */
    input.handle(tetrominos, &max, &event, &position, &left, &right, &run);

    /**
     * Invocation des fonctions affectant les tetrominos.
     */
    for (int i = 0; i < max; i++) { // Boucle itérant sur chaque tetromino existant dans la partie
      collision.collide(tetrominos, i, &max, &height);
    }
    tetromino.limit(tetrominos, &max, &height);
    tetromino.display(renderer, bloc, tetrominos, active, inactive, &max);
    tetromino.fall(tetrominos, &max);

    /**
     * Invocation des fonctions affectant le personnage.
     */
    character.display(renderer, personnage, &position);
    character.handleVelocity(tetrominos, &max, &position, &run);
    character.movement(&position);

    //std::cout << "VelocityX : " << position.velocityX << std::endl;
    //std::cout << "VelocityY : " << position.velocityY << std::endl;
    //std::cout << "height : " << height << std::endl;
    std::cout << "posY1 : " << position.y << std::endl;
    std::cout << "posY2 : " << position.y + position.height << std::endl;
    std::cout << "velocityX : " << position.velocityX << std::endl;
    std::cout << "velocityY : " << position.velocityY << std::endl;

    std::cout << "Tetromino 5 - Y1 : " << tetrominos[5].startY << " - Y2 : " << tetrominos[5].startY + (tetromino.maxBloc * tetromino.maxSize) << std::endl; 
    std::cout << "Tetromino 6 - Y1 : " << tetrominos[6].startY << " - Y2 : " << tetrominos[6].startY + (tetromino.maxBloc * tetromino.maxSize) << std::endl; 

    // Rafraichissement de l'affichage
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
