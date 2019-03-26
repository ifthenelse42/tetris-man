#include "SDL2/SDL.h"

class Engine::Render {
  public:
  const int width = 860;
  const int height = 480;

  SDL_Window* createWindow();
  SDL_Renderer* create(SDL_Window* window);
  void clear(SDL_Renderer* renderer);
};
