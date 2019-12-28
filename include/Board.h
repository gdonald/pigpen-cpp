#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#include "Move.h"

#define SIZE 50
#define ROWS 11
#define COLS 6
#define PC_ROWS 5
#define PC_COLS 5

#define P -1
#define EMPTY 0
#define C 1

class Board {
public:
  Board();

  Board(Board const &board);

  ~Board();

  bool legalMove(int col, int row);

  void addMove(const Move &move, int pc);

  void assignSquares(const Move &move, int pc);

  std::vector<Move> legalMoves();

  int getWinner();

  int getPC(int col, int row);

  int getScore(int pc);

  int moves[ROWS][COLS]{};

  int pcs[PC_ROWS][PC_COLS]{};

private:

};

#endif