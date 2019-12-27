#include "Board.h"

Board::~Board() {}

Board::Board() {
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      moves[row][col] = EMPTY;

  // some verticals
//  moves[1][0] = P;
//  moves[3][1] = C;
//  moves[5][2] = P;
//  moves[7][3] = C;
//  moves[9][4] = P;
//  moves[7][5] = C;

  // some horizontals
//  moves[0][0] = P;
//  moves[2][1] = C;
//  moves[4][2] = P;
//  moves[6][3] = C;
//  moves[8][4] = P;
//  moves[10][3] = C;
}
