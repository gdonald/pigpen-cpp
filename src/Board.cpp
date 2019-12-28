#include "Board.h"

Board::~Board() {}

Board::Board() {
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      moves[row][col] = EMPTY;

  for (int row = 0; row < PC_ROWS; row++)
    for (int col = 0; col < PC_COLS; col++)
      pcs[row][col] = EMPTY;

  // some verticals
//  moves[1][0] = P;
//  moves[1][1] = C;
//  moves[3][1] = P;
//  moves[3][2] = C;

  // some horizontals
//  moves[0][0] = P;
//  moves[2][0] = C;
//  moves[2][1] = P;
//  moves[4][1] = C;

//  pcs[0][0] = P;
//  pcs[1][1] = C;

//  moves[3][1] = P;
//  moves[3][2] = P;
//  moves[4][1] = P;

}

Board::Board(Board const &board) {
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      moves[row][col] = board.moves[row][col];

  for (int row = 0; row < PC_ROWS; row++)
    for (int col = 0; col < PC_COLS; col++)
      pcs[row][col] = board.pcs[row][col];
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
  assignSquares(move, pc);
}

void Board::assignSquares(const Move &move, int pc) {
  if (move.row % 2 == 0) { // horizontal
    std::cout << "horizontal" << std::endl;

    if (move.row < 10) { // below
      std::cout << "below" << std::endl;
      if (moves[move.row + 1][move.col + 1] != EMPTY && // right
          moves[move.row + 2][move.col] != EMPTY &&     // bottom
          moves[move.row + 1][move.col] != EMPTY) {     // left
        pcs[move.row == 0 ? 0 : (move.row / 2)][move.col] = pc;
      }
    }

    if(move.row > 0) { // above
      std::cout << "above" << std::endl;
      if (moves[move.row - 1][move.col + 1] != EMPTY && // right
          moves[move.row - 2][move.col] != EMPTY &&     // top
          moves[move.row - 1][move.col] != EMPTY) {     // left
        pcs[(move.row / 2) - 1][move.col] = pc;
      }
    }
  } else { // vertical
    std::cout << "vertical" << std::endl;

    if (move.col < 5) { // right
      std::cout << "right" << std::endl;
      if (moves[move.row - 1][move.col] != EMPTY && // top
          moves[move.row][move.col + 1] != EMPTY && // right
          moves[move.row + 1][move.col] != EMPTY) { // bottom
        pcs[move.row / 2][move.col] = pc;
      }
    }

    if(move.col > 0) { // left
      std::cout << "left" << std::endl;
      if (moves[move.row - 1][move.col - 1] != EMPTY && // top
          moves[move.row][move.col - 1] != EMPTY &&     // left
          moves[move.row + 1][move.col - 1] != EMPTY) { // bottom
        pcs[move.row / 2][move.col - 1] = pc;
      }
    }
  }
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

int Board::getPC(int col, int row) {
  return pcs[row][col];
}
