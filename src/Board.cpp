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

Board::Board(Board const &board) {
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      moves[row][col] = board.moves[row][col];
}

bool Board::legalMove(int col, int row) {
  if (row < 0 || col < 0) { return false; }
  if ((row % 2 == 0) && col > 4) { return false; }

  return moves[row][col] == EMPTY;
}

void Board::addMove(const Move &move, int pc) {
  if (moves[move.row][move.col] != EMPTY) {
    std::cout << "Cannot add move to occupied: col: " << move.col << ", row: " << move.row << std::endl;
    throw;
  }

  moves[move.row][move.col] = pc;
}

std::vector<Move> Board::legalMoves() {
  std::vector<Move> v;

  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      if (legalMove(c, r))
        v.emplace_back(c, r);

  return v;
}

int Board::getWinner() {
  // TODO

  return EMPTY;
}
