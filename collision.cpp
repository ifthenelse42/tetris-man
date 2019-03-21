#include "console.h"
#include "tetromino.h"
#include <iostream>

bool xCollide(int x1Actual, int x2Actual, int x1All, int x2All)
{
  return x2Actual > x1All && x1Actual < x2All;
}

bool yCollide(int y1Actual, int y2Actual, int y1All, int y2All)
{
  return y1Actual <= y2All && y2Actual >= y1All;
}

bool bottomBorderCollide(int y2)
{
  return y2 >= 700;
}

// On renvoi si la position donnée touche un autre bloc dans la mémoire
void collide(SDL_Renderer* renderer, blocs2* tetrominos, int tetrominoIndex)
{
  int lastIndex = lastTetrominosIndex(tetrominos);
}
