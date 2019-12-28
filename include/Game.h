#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <thread>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "Board.h"

#define DEPTH 3

#define SCREEN_W  600
#define SCREEN_H 600

#define BTN_W 120
#define BTN_H 40
#define BTN_SPACE 20

#define FONT "res/font/WickedGrit.ttf"

enum Buttons {
  BtnYes, BtnNo,
  BtnCount
};

enum ButtonStates {
  BtnUp = 0
};

enum Menus {
  MenuNone, MenuGameOver
};

class Game {
public:
  ~Game();

  explicit Game(const char *title);

  bool isRunning();

  void handleEvents();

  void render();

  void newGame();

  void drawDots();

  void drawMoves();

  void drawMove(int color, const Sint16 vx[4], const Sint16 vy[4]);

  void handleClick(SDL_MouseButtonEvent *event);

  void switchTurn();

  static bool gameOver(Board *board);

  bool isPlayerTurn();

  static void aiThread(Game *game);

  void aiTurn();

  Move getAiMove();

  static int minimax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer);

  static int evaluate(Board *board, int xo);

  void drawPCs();

  void drawP(Sint16 col, Sint16 row);

  void drawC(Sint16 col, Sint16 row);

  void writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color);

  static int getOther(int pc);

  void drawMenu();

  void drawGameOverMenu();

  static bool insideRect(SDL_Rect rect, int x, int y);

private:
  bool running = false;
  int currentMenu = MenuNone;

  SDL_Window *window{};
  SDL_Renderer *renderer{};
  SDL_Surface *bgSurface;
  SDL_Texture *bgTexture;
  Board *board{};

  int mouseX{};
  int mouseY{};

  int turn{};

  SDL_Texture *btnTextures[BtnCount]{};
  SDL_Rect btnRects[BtnCount]{};

  SDL_Surface *btnYesSurface;
  SDL_Surface *btnNoSurface;

  TTF_Font *boardFont;
  TTF_Font *font21;
};

#endif