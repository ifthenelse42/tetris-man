#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "engine/engine.hpp"
#include "engine/collision.hpp"
#include "engine/error.hpp"
#include "engine/render.hpp"
#include "engine/texture.hpp"
#include "engine/run.hpp"
#include "game/game.hpp"
#include "game/input.hpp"
#include "game/hud.hpp"
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
void Engine::Run::loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* scoreFont, TTF_Font* timeFont)
{
  Engine::Texture texture;
  Engine::Render render;
  Engine::Collision collision;
  Game::Tetromino tetromino;
  Game::Character character;
  Game::Input input;
  Game::HUD hud;

  bool game = true; // Variable venant faire en sorte que le jeu démarre
  bool left = false; // Variable activant le mouvement vers la gauche
  bool right = false; // Variable activant le mouvement vers la droite
  bool jump = false; // Variable activant le saut
  bool run = true; // Cette variable indique si le jeu est démarré ou non
  int ticks = 0; // Cette variable est incrémenté à chaque itération de la boucle du jeu
	int time = 0; // Cette variable sert à récupérer les secondes depuis le début de la partie
  int max = 0; // Variable contenant le nombre max de tetromino, qui est utilisé par plusieurs fonctions dans le code source
  int speed = 1; // Variable contenant la vitesse de chute des tetrominos
  int height = render.height; // Hauteur de la zone de jeu - manipulé par la caméra
  Game::Character::position position; // On initialise la position du personnage
  SDL_Texture* personnage = character.create(renderer, IMG_Load("assets/image/perso.png"), &position); // On créé le personnage joué par le joueur
  Mix_Music* ambiance = Mix_LoadMUS("assets/music/tetris-d.mp3"); // On charge la musique d'ambiance

  /**
   * On charge les différentes couleurs de blocs
   * TODO
   */
  SDL_Color black = { 255, 0, 0, 255 };
  SDL_Texture* blocBlack = texture.createBloc(renderer, black);

  SDL_Color red = { 255, 0, 0, 255 };
  SDL_Texture* blocRed = texture.createBloc(renderer, red);

  SDL_Color orange = { 100, 100, 0, 255 };
  SDL_Texture* blocOrange = texture.createBloc(renderer, orange);

  SDL_Color blue = { 0, 0, 255, 255 };
  SDL_Texture* blocBlue = texture.createBloc(renderer, blue);

  SDL_Color cyan = { 0, 255, 0, 255 };
  SDL_Texture* blocCyan = texture.createBloc(renderer, cyan);

  SDL_Color purple = { 0, 255, 0, 255 };
  SDL_Texture* blocPurple = texture.createBloc(renderer, purple);

  if (ambiance == NULL) {
    printf("Unable to load mp3 file: %s\n", Mix_GetError()); 
  }

  if (Mix_PlayMusic(ambiance, -1) == -1) {
    printf("Mix_PlayMusic: %s\n", Mix_GetError());
  }

  SDL_Event event;
  Game::Tetromino::blocs tetrominos[Game::Tetromino::max];
  tetromino.firstSpawn(tetrominos, &max, &speed, &height);

  /**
   * Boucle d'exécution du jeu. 
   * Chaque itération correspond à un frame.
   */
  while (run) {
    /**
     * Invocation des fonctions affectant les entrées de l'utilisateur. Ces fonctions 
     * sont accessible sans que le jeu soit démarré.
     */
    input.handle(tetrominos, &max, &event, &position, &left, &right, &jump, &run);

    // On balaye l'affichage
    render.clear(renderer);
    if (game) {
    if (ticks == 10) {
      tetromino.handleSpawn(tetrominos, &max, &height, &speed);
      ticks = 0;
    }
		float time = SDL_GetTicks();


    /**
     * Invocation des fonctions gérant l'interface utilisateur (HUD)
     */
    int score = hud.score(renderer, scoreFont, timeFont, &time, &speed);

    /**
     * Invocation des fonctions affectant les tetrominos.
     */
    for (int i = 0; i < max; i++) { // Boucle itérant sur chaque tetromino existant dans la partie
      collision.collide(tetrominos, i, &max, &height);
    }

    /**
     * Si un des tetrominos touche la limite d'affichage de l'écran, 
     * on descend tous les tetrominos et le personnage 
     * pour simuler une caméra ascendante.
     */
    if (tetromino.limit(tetrominos, &max)) {
      int amount = 2; // On monte de x pixel les tetrominos et le personnage

      tetromino.moveAllUp(tetrominos, amount, &max, &height);
      character.allUp(&position, amount);
    }

    tetromino.display(renderer, blocOrange, tetrominos, &max);
    tetromino.speedUp(tetrominos, &max, &speed, &score);
    tetromino.fall(tetrominos, &max, &speed);

    /**
     * Invocation des fonctions affectant le personnage.
     */
    character.display(renderer, personnage, &position);
    character.handleVelocity(tetrominos, &max, &position, &run);
    character.movement(&position);

    ticks++;
    time++;
  } else {
    // TODO: menu ici
  }
    // Rafraichissement de l'affichage
    SDL_RenderPresent(renderer);
  }

  Mix_FreeMusic(ambiance);
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
void Engine::Run::close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* scoreFont, TTF_Font* timeFont)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(scoreFont);
  TTF_CloseFont(timeFont);
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}
