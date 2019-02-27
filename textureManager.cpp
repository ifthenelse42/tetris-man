#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "errorManager.h"

SDL_Texture* mkTextureFromImage(SDL_Renderer* renderer, SDL_Surface* imgBuffer)
{
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgBuffer);
  SDL_FreeSurface(imgBuffer);

  textureLoadError(texture);

  return texture;
}

SDL_Texture* mkTexture(SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

  textureLoadError(texture);

  return texture;
}

SDL_Texture* mkText(SDL_Renderer* renderer, SDL_Color color, TTF_Font* font, const char* text)
{
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  // La surface ne sert plus à rien une fois créée
  SDL_FreeSurface(surface);

  // Par contre la texture sert toujours, donc on renvoie son pointeur
  return texture;
}

SDL_Texture* mkTetromino(SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* bloc = mkTexture(renderer, width, height);
  textureLoadError(bloc);

  return bloc;
}

void displayTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height)
{
  SDL_Rect position = { x, y, width, height };
  SDL_RenderCopy(renderer, texture, NULL, &position);
}

void displayText(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y)
{
  int width = 0;
  int height = 0;

  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  SDL_Rect position = { x, y, width, height };
  SDL_RenderCopy(renderer, texture, NULL, &position);
}

void drawLine(SDL_Renderer* renderer, int r, int g, int b, int a, int x1, int y1, int x2, int y2)
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
