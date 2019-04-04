#include "SDL2/SDL.h"

class Engine::Render {
  public:
  static const int width = 350;
  static const int height = 680;

  SDL_Window* createWindow();
  SDL_Renderer* create(SDL_Window* window);
  void clear(SDL_Renderer* renderer);
};
