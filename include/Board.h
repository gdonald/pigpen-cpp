#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#include "Move.h"

#define SIZE 50
#define ROWS 11
#define COLS 6

#define P -1
#define EMPTY 0
#define C 1

class Board {
public:
  Board();

  Board(Board const &board);

  ~Board();

  bool legalMove(int col, int row);

  std::vector<Move> legalMoves();

  int getPC(int col, int row);

  void addMove(const Move &move, int xo);

  int getWinner();

  int moves[ROWS][COLS]{};

private:

};

#endif