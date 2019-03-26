#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Engine::Run {
  public:
  void SDL();
  void loop(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);
  void close(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);
};
