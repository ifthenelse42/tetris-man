#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

class Engine::Error {
  public:
  int initSDL();
  int initTTF();
  int initMixer();
  int initIMG();
  int rendererAndWindow(SDL_Renderer* renderer, SDL_Window* window);
  int textureLoad(SDL_Texture* texture);
  int musicLoad(Mix_Music* music);
  int surfaceGeneration(SDL_Surface* surface);
  int fontLoad(TTF_Font* font);
};
