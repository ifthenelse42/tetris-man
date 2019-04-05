#include "../game/tetromino.hpp"

class Engine::Collision {
  private:
  bool screenCollide(int actualY2, int* width, int* height);

  public:
  bool xCollide(int actualX1, int actualX2, int allX1, int allX2);
  bool yCollide(int actualY1, int actualY2, int allY1, int allY2);
  bool tetrominoCollide(int actualX1, int actualY1, int actualX2, int actualY2, int allX1, int allY1, int allX2, int allY2);
  void collide(Game::Tetromino::blocs* tetrominos, int tetrominoIndex, int* max, int* width, int* height);
};
