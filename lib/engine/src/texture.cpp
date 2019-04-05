#include "SDL2/SDL.h"
#include "engine/engine.hpp"
#include "engine/texture.hpp"
#include "engine/error.hpp"
#include "game/game.hpp"
#include "game/tetromino.hpp"

/**
 * Fonction: Engine::Texture::create
 * ------------------------
 * Créé une texture et la renvoie.
 *
 * @param renderer Rendu où affecter la nouvelle texture
 * @param width Largeur de la texture désirée
 * @param height Hauteur de la texture désirée
 *
 * @return Le pointeur vers la texture nouvellement créée
 */
SDL_Texture* Engine::Texture::create(SDL_Renderer* renderer, int width, int height)
{
  Engine::Error error;
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

  error.textureLoad(texture);

  return texture;
}

/**
 * Fonction: Engine::Texture::createBloc
 * ------------------------
 * Créé un bloc et le renvoi. 
 * Pour créer un bloc, on créé une texture vide, puis on affecte le rendu à cette texture uniquement, 
 * puis on change la couleur du rendu et on y créé des lignes. 
 * Cela donne un bloc de tetromino.
 *
 * TODO: couleur à choisir en argument de la fonction
 *
 * @param renderer Rendu où affecter le nouveau bloc
 *
 * @return Le pointeur vers le bloc nouvellement créé
 *
 * @see SDL_SetRenderDrawColor
 * @see SDL_SetRenderTarget
 * @see Engine::Texture::createLine
 */
SDL_Texture* Engine::Texture::createBloc(SDL_Renderer* renderer)
{
  Engine::Texture texture;
  Game::Tetromino tetromino;
  Error error;

  SDL_Texture* bloc = texture.create(renderer, tetromino.blocWidth, tetromino.blocHeight);
  error.textureLoad(bloc);

  // On transforme la texture en un rendu
  SDL_SetRenderTarget(renderer, bloc);

  // Puis on change sa couleur
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_RenderClear(renderer);

  // On ajoute des lignes pour faire jolie
  texture.createLine(renderer, 0, 0, 0, 255, 0, 0, tetromino.blocWidth, 0);
  texture.createLine(renderer, 0, 0, 0, 255, 0, tetromino.blocHeight - 1, tetromino.blocWidth, tetromino.blocHeight - 1);
  texture.createLine(renderer, 0, 0, 0, 255, 0, 0, 0, tetromino.blocHeight - 1);
  texture.createLine(renderer, 0, 0, 0, 255, tetromino.blocWidth - 1, 0, tetromino.blocWidth - 1, tetromino.blocHeight - 1);

  // Enfin, on rend le rendu au renderer initial
  SDL_SetRenderTarget(renderer, NULL);

  return bloc;
}

/**
 * Fonction: Engine::Texture::createFromImage
 * ------------------------
 * Créé une texture depuis une image.
 *
 * @param renderer Rendu où affecter la texture
 * @param imgBuffer Image que l'on souhaite transformer en texture
 *
 * @return Le pointeur vers la texture nouvellement créée, depuis l'image
 *
 * @see SDL_ImgLoad
 */
SDL_Texture* Engine::Texture::createFromImage(SDL_Renderer* renderer, SDL_Surface* imgBuffer)
{
  Engine::Error error;

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgBuffer);
  SDL_FreeSurface(imgBuffer);

  error.textureLoad(texture);

  return texture;
}

/**
 * Fonction: Engine::Texture::createText
 * ------------------------
 * Créé un texte.
 *
 * @param renderer Rendu où affecter la texture du texte
 * @param color Couleur du texte
 * @param font Police d'écriture à affecter au texte
 * @param text Texte à transformer en texture
 *
 * @return Le pointeur vers la texture nouvellement créée, qui est le texte
 */
SDL_Texture* Engine::Texture::createText(SDL_Renderer* renderer, SDL_Color color, TTF_Font* font, const char* text)
{
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  // La surface ne sert plus à rien une fois créée
  SDL_FreeSurface(surface);

  // Par contre la texture sert toujours, donc on renvoie son pointeur
  return texture;
}

/**
 * Fonction: Engine::Texture::createLine
 * ------------------------
 * Créé une ligne.
 *
 * @param renderer Rendu où affecter la ligne
 * @param r Code hexadécimal rouge
 * @param g Code hexadécimal vert
 * @param b Code hexadécimal bleu
 * @param a Code hexadécimal opacité
 * @param x1 Point X1 de la ligne (gauche)
 * @param y1 Point Y1 de la ligne (haut)
 * @param x2 Point X2 de la ligne (droite)
 * @param y2 Point Y2 de la ligne (bas)
 */
void Engine::Texture::createLine(SDL_Renderer* renderer, int r, int g, int b, int a, int x1, int y1, int x2, int y2)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

/**
 * Fonction: Engine::Texture::display
 * ------------------------
 * Affiche une texture dans le rendu, donc à l'utilisateur.
 *
 * @param renderer Rendu où afficher la texture
 * @param texture Texture que l'on souhaite afficher dans le rendu
 * @param x Point X où afficher la texutre
 * @param y Point Y où afficher la texture
 * @param width Largeur de la texture
 * @param height Hauteur de la texture
 */
void Engine::Texture::display(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height)
{
  SDL_Rect position = { x, y, width, height };
  SDL_RenderCopy(renderer, texture, NULL, &position);
}

/**
 * Fonction: Engine::Texture::displayText
 * ------------------------
 * Affiche la texture d'un texte. 
 * La différence avec Engine::Texture::display réside dans l'inutilité d'attribuer une largeur et une hauteur à un texte, 
 * ces deux variables sont donc définies par défaut à 0 dans cette fonction.
 *
 * @param renderer Rendu où afficher la texture
 * @param texture Texture que l'on souhaite afficher dans le rendu
 * @param x Point X où afficher la texutre
 * @param y Point Y où afficher la texture
 */
void Engine::Texture::displayText(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
{
  int width = 0;
  int height = 0;

  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  SDL_Rect position = { x, y, width - 50, height - 75 };
  SDL_RenderCopy(renderer, texture, NULL, &position);
}
