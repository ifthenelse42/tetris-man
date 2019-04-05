#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Engine::Error {
  public:
  int initSDL();
  int initTTF();
  int initIMG();
  int rendererAndWindow(SDL_Renderer* renderer, SDL_Window* window);
  int textureLoad(SDL_Texture* texture);
  int surfaceGeneration(SDL_Surface* surface);
  int fontLoad(TTF_Font* font);
};
