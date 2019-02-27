#pragma once
#include "tetromino.h"

bool xCollide(int x1Actual, int x2Actual, int x1All, int x2All);
bool yCollide(int y1Actual, int y2Actual, int y1All, int y2All);
bool borderTopRightCollide(int x2Actual, int x1All, int y1Actual, int y2All);
bool borderTopLeftCollide(int x1Actual, int x2All, int y1Actual, int y2All);
bool borderBottomRightCollide(int x2Actual, int x1All, int y2Actual, int y1All);
bool borderBottomLeftCollide(int x1Actual, int x2All, int y2Actual, int y1All);
bool collide(struct blocs *tetrominos, int tetrominoIndex);
