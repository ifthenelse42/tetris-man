#include "SDL2/SDL.h"
#include "textureManager.h"

SDL_Texture* bloc1(SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* bloc = mkTetromino(renderer, width, height);
  return bloc;
}
