#include "../game/tetromino.hpp"
#include "../game/character.hpp"

class Engine::Collision {
  private:
  bool screenCollide(int actualY2, int* height);

  public:
  bool xCollide(int actualX1, int actualX2, int allX1, int allX2);
  bool yCollide(int actualY1, int actualY2, int allY1, int allY2);
  bool tetrominoCollide(int actualX1, int actualY1, int actualX2, int actualY2, int allX1, int allY1, int allX2, int allY2);
  void collide(Game::Tetromino::blocs* tetrominos, int tetrominoIndex, int* max, int* height);
  bool collideCharacter(Game::Tetromino::blocs* tetrominos, int* max, Game::Character::position* position, bool* run);
  bool collideCharacterSide(Game::Tetromino::blocs* tetrominos, int* max, Game::Character::position* position, bool* run);
  bool collideCharacterFall(Game::Tetromino::blocs* tetrominos, int* max, Game::Character::position* position, bool* run);
  bool collideCharacterDead(Game::Tetromino::blocs* tetrominos, int* max, Game::Character::position* position, bool* run);
};
