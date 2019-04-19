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
#include "game/menu.hpp"
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
 * @param renderer Rendu où le jeu est affiché.
 * @param window Fenêtre où le rendu/le jeu se trouve.
 * @param menuFont Police d'écriture du menu.
 * @param deadFont Police d'écriture de la mort.
 * @param bigFont Police d'écriture de la mort.
 * @param scoreFont Police d'écriture du score.
 * @param timeFont Police d'écriture du temps de jeu.
 *
 * @see main.cpp
 * @see Engine::Error::initSDL
 * @see Engine::Texture
 * @see Engine::Render
 * @see Engine::Collision
 * @see Game
 */
void Engine::Run::loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* menuFont, TTF_Font* deadFont, TTF_Font* bigFont, TTF_Font* scoreFont, TTF_Font* timeFont)
{
  Engine::Error error;
  Engine::Collision collision;
  Engine::Texture texture;
  Engine::Render render;
  Game::Tetromino tetromino;
  Game::Character character;
  Game::Input input;
  Game::Menu menu;
  Game::HUD hud;
  bool launch = false; // Interrupteur qui sert à redémarrer toute les variables du jeu
  bool game = false; // Variable venant faire en sorte que le jeu démarre
  bool dead = false; // Variable affichant l'écran de gameover
  bool left = false; // Variable activant le mouvement vers la gauche
  bool right = false; // Variable activant le mouvement vers la droite
  bool jump = false; // Variable activant le saut
  bool run = true; // Cette variable indique si le jeu est démarré ou non
  int ticks = 0; // Cette variable est incrémenté à chaque itération de la boucle du jeu
  int max = 0; // Variable contenant le nombre max de tetromino, qui est utilisé par plusieurs fonctions dans le code source
  int speed = 1; // Variable contenant la vitesse de chute des tetrominos
  int height = render.height; // Hauteur de la zone de jeu - manipulé par la caméra
  float time;
  float deathTime; // On remplis cette variable d'une copie du moment où le joueur a péri
  int resetTime;
  int globalTicks;
  Game::Character::position position; // On initialise la position du personnage
  Game::Menu::button start; // On initialise la structure contenant le menu principal
  Game::Menu::button quit; // On initialise la structure contenant le menu principal
  Game::Menu::button replay; // On initialise la structure contenant le menu principal
  Game::Menu::button quit2; // On initialise la structure contenant le menu principal
  SDL_Texture* personnage; // On créé le personnage joué par le joueur
  Game::Tetromino::death death1;
  Game::Tetromino::death death2;
  Game::Tetromino::death death3;
  Mix_Music* ambiance;

  /**
   * On charge les différentes couleurs de blocs
   */
  SDL_Color black = { 30, 30, 30, 255 };
  SDL_Texture* blocBlack = texture.createBloc(renderer, black);

  SDL_Color red = { 255, 0, 0, 255 };
  SDL_Texture* blocRed = texture.createBloc(renderer, red);

  SDL_Color orange = { 255, 140, 0, 255 };
  SDL_Texture* blocOrange = texture.createBloc(renderer, orange);

  SDL_Color blue = { 0, 30, 255, 255 };
  SDL_Texture* blocBlue = texture.createBloc(renderer, blue);

  SDL_Color pink = { 255, 0, 190, 255 };
  SDL_Texture* blocPink = texture.createBloc(renderer, pink);

  SDL_Color green = { 40, 255, 0, 255 };
  SDL_Texture* blocGreen = texture.createBloc(renderer, green);

  SDL_Color cyan = { 0, 200, 255, 255 };
  SDL_Texture* blocCyan = texture.createBloc(renderer, cyan);

  SDL_Color purple = { 120, 0, 255, 255 };
  SDL_Texture* blocPurple = texture.createBloc(renderer, purple);

  SDL_Color yellow = { 255, 200, 140, 255 };
  SDL_Texture* blocYellow = texture.createBloc(renderer, yellow);
  
  SDL_Event event;
  Game::Tetromino::blocs tetrominos[Game::Tetromino::max];

  /**
   * On initialise le menu principal
   */
  menu.init(renderer, &start, &quit, menuFont);

  /**
   * Boucle d'exécution du jeu. 
   * Chaque itération correspond à un frame.
   */
  while (run) {
    globalTicks = SDL_GetTicks();

    /**
     * On vérifie si le jeu n'est pas encore lancé mais qu'on viens de le faire. A la fin 
     * de l'initialisation (ou re-initialisation) des variables du jeu, on désactive la variable 
     * launch. Cela sert à exécuter une seule fois la ré-initialisation du jeu.
     *
     * En bref cette condition initialise la partie.
     */
    if (launch && game) {
      /**
       * On initialise ici toute les variables utilisées dans une seule partie. Elles sont donc 
       * ré-initialisé lors du début d'une nouvelle partie.
       */
      tetromino.empty(tetrominos, &max);
      personnage = character.create(renderer, IMG_Load("assets/image/perso.png"), &position);
      ambiance = Mix_LoadMUS("assets/music/tetris-d.mp3");
      speed = 1;
      ticks = 0;
      max = 0;
      globalTicks = 0;
      resetTime = SDL_GetTicks();
      height = render.height;

      /**
       * On créé les lignes de la mort, délimitant la zone de jeu.
       *
       * 1: bas (provoque la mort si touchée)
       * 2: gauche
       * 3: droite
       */
      death1.x1 = 100;
      death1.x2 = render.width - 125;
      death1.y1 = render.height;
      death1.y2 = render.height;

      death2.x1 = 99;
      death2.x2 = 99;
      death2.y1 = 0;
      death2.y2 = render.height;

      death3.x1 = render.width - 125;
      death3.x2 = render.width - 125;
      death3.y1 = 0;
      death3.y2 = render.height;

      error.musicLoad(ambiance);

      // Lancement de la musique d'ambiance tetris-d
      if (Mix_PlayMusic(ambiance, -1) == -1) {
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
      }

      tetromino.firstSpawn(tetrominos, &max, &speed, &height);
      launch = false;
    }
    /**
     * Invocation des fonctions affectant les entrées de l'utilisateur. Ces fonctions 
     * sont accessible sans que le jeu soit démarré.
     */
    input.handle(tetrominos, &max, &event, &position, &left, &right, &jump, &launch, &game, &dead, &run, &start, &quit, &replay, &quit2);

    // On balaye l'affichage
    render.clear(renderer);
    if (game) {
      time = globalTicks - resetTime;
    if (ticks == 10) {
      tetromino.handleSpawn(tetrominos, &max, &height, &speed);
      ticks = 0;
    }

    /**
     * Invocation des fonctions gérant l'interface utilisateur (HUD)
     */
    int score = hud.score(renderer, scoreFont, timeFont, &time, &speed, &dead);

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
      int amount = 3; // On monte de x pixel les tetrominos et le personnage

      tetromino.moveAllUp(tetrominos, amount, &max, &height);
      character.allUp(&position, amount);
    }

    // On affiche les lignes de la mort
    texture.createLine(renderer, 170, 0, 0, 255, death1.x1, death1.y1, death1.x2, death1.y2);
    texture.createLine(renderer, 10, 10, 10, 255, death2.x1, death2.y1, death2.x2, death2.y2);
    texture.createLine(renderer, 10, 10, 10, 255, death3.x1, death3.y1, death3.x2, death3.y2);

    tetromino.display(renderer, tetrominos, &max, blocBlack, blocRed, blocOrange, blocBlue, blocPink, blocGreen, blocCyan, blocPurple, blocYellow);
    tetromino.speedUp(tetrominos, &max, &speed, &score);

    // Quand le joueur perd, tout doit se figer
    if (!dead) {
      tetromino.fall(tetrominos, &max, &speed);
    }

    /**
     * Invocation des fonctions affectant le personnage.
     */
    character.display(renderer, personnage, &position);

    // Quand le joueur perd, tout doit se figer
    if (!dead) {
      character.handleVelocity(tetrominos, &max, &position, &dead, &time, &deathTime, &death1, &death2, &death3);
      character.movement(&position);
    }

    /**
     * Lorsque le joueur meurt, on arrête aussi la musique et on affiche un message pour 
     * lui annoncer son score
     */
    if (dead) {
      Mix_HaltMusic();
      hud.gameover(renderer, &deathTime, deadFont, bigFont, &replay, &quit2, &speed);
    }

    ticks++;
  } else {
    menu.index(renderer, &start, &quit, menuFont);
    Mix_HaltMusic();
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
void Engine::Run::close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* menuFont, TTF_Font* deadFont, TTF_Font* bigFont, TTF_Font* scoreFont, TTF_Font* timeFont)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(menuFont);
  TTF_CloseFont(deadFont);
  TTF_CloseFont(bigFont);
  TTF_CloseFont(scoreFont);
  TTF_CloseFont(timeFont);
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}
