#include <algorithm>
#include "Game.h"

std::chrono::duration<long long int> Game::wait = std::chrono::seconds(1);

Game::~Game() {
  TTF_CloseFont(boardFont);
  TTF_CloseFont(font21);
  TTF_Quit();

  SDL_DestroyTexture(btnTextures[BtnYes]);
  SDL_DestroyTexture(btnTextures[BtnNo]);

  SDL_DestroyTexture(bgTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

Game::Game(const char *title, std::default_random_engine *eng) {
  engine = eng;

  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);

  if (!window) {
    printf("Could not create window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetWindowMinimumSize(window, SCREEN_W, SCREEN_H);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer) {
    printf("Count not get renderer! SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  bgSurface = SDL_LoadBMP("res/img/bg.bmp");
  if (bgSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/bg.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  SDL_FreeSurface(bgSurface);

  btnYesSurface = SDL_LoadBMP("res/img/btn_yes.bmp");
  btnNoSurface = SDL_LoadBMP("res/img/btn_no.bmp");

  if (btnYesSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_yes.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (btnNoSurface == nullptr) {
    printf("Unable to load image %s! SDL Error: %s\n", "res/img/btn_no.bmp", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  btnTextures[BtnYes] = SDL_CreateTextureFromSurface(renderer, btnYesSurface);
  btnTextures[BtnNo] = SDL_CreateTextureFromSurface(renderer, btnNoSurface);

  SDL_FreeSurface(btnYesSurface);
  SDL_FreeSurface(btnNoSurface);

  if (TTF_Init() == -1) {
    printf("TTF_Init failed: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  boardFont = TTF_OpenFont(FONT, 100);
  if (boardFont == nullptr) {
    printf("Failed to load boardFont! Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  font21 = TTF_OpenFont(FONT, 21);
  if (font21 == nullptr) {
    printf("Failed to load font21! Error: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  newGame();

  running = true;
}

void Game::render() {
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);

  drawMoves();
  drawDots();
  drawPCs();
  drawMenu();

  SDL_RenderPresent(renderer);
}

void Game::newGame() {
  board = new Board();
  turn = P;
}

bool Game::isRunning() {
  return running;
}

void Game::handleEvents() {
  SDL_Event event;

  if (SDL_WaitEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&mouseX, &mouseY);
        handleClick(&event.button);
        break;
    }
  }
}

void Game::drawDots() {
  int xx, yy;

  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 6; x++) {
      xx = x * SIZE * 2 + SIZE;
      yy = y * SIZE * 2 + SIZE;
      filledCircleRGBA(renderer, xx, yy - 1, 4, 0x00, 0x00, 0xff, 0xaa);
      filledCircleRGBA(renderer, xx, yy + 1, 4, 0x00, 0x00, 0xff, 0xaa);
    }
  }
}

void Game::drawMoves() {
  // vertical
  for (int y = 1; y < ROWS; y += 2) {
    for (int x = 0; x < COLS; x++) {
      if (board->moves[y][x] == EMPTY) { continue; }

      Sint16 x0 = (x * 2 * SIZE) + SIZE - 3;
      Sint16 x1 = (x * 2 * SIZE) + 4 + SIZE;
      Sint16 y0 = y * SIZE;
      Sint16 y1 = (y * SIZE) + (SIZE * 2);

      const Sint16 vx[4] = {x0, x1, x1, x0};
      const Sint16 vy[4] = {y0, y0, y1, y1};

      drawMove(board->moves[y][x], vx, vy);
    }
  }

  // horizontal
  for (int y = 0; y < ROWS; y += 2) {
    for (int x = 0; x < COLS; x++) {
      if (board->moves[y][x] == EMPTY) { continue; }

      Sint16 x0 = (x * 2 * SIZE) + SIZE;
      Sint16 x1 = (x * 2 * SIZE) + (SIZE * 2) + SIZE;
      Sint16 y0 = (y * SIZE) + SIZE - 4;
      Sint16 y1 = (y * SIZE) + SIZE + 2;

      const Sint16 vx[4] = {x0, x1, x1, x0};
      const Sint16 vy[4] = {y0, y0, y1, y1};

      drawMove(board->moves[y][x], vx, vy);
    }
  }
}

void Game::drawMove(int color, const Sint16 vx[4], const Sint16 vy[4]) {
  int rgb = color == P ? 0xff : 0x00;
  filledPolygonRGBA(renderer, vx, vy, 4, rgb, rgb, rgb, 0x99);
}

void Game::drawPCs() {
  for (Sint16 r = 0; r < PC_ROWS; r++) {
    for (Sint16 c = 0; c < PC_COLS; c++) {
      int pc = board->getPC(c, r);
      if (pc == P) drawP(c, r);
      else if (pc == C) drawC(c, r);
    }
  }
}

void Game::drawP(Sint16 col, Sint16 row) {
  Sint16 x = (SIZE * 2 * col) + SIZE + 15;
  Sint16 y = (SIZE * 2 * row) + SIZE + 13;

  SDL_Color color = {255, 255, 255, 190};
  writeText("P", x, y, boardFont, color);
}

void Game::drawC(Sint16 col, Sint16 row) {
  Sint16 x = (SIZE * 2 * col) + SIZE + 12;
  Sint16 y = (SIZE * 2 * row) + SIZE + 13;

  SDL_Color color = {0, 0, 0, 190};
  writeText("C", x, y, boardFont, color);
}

void Game::handleClick(SDL_MouseButtonEvent *event) {
  switch (currentMenu) {
    case MenuGameOver:
      if (insideRect(btnRects[BtnNo], mouseX, mouseY)) {
        currentMenu = MenuNone;
        render();
        return;
      }
      if (insideRect(btnRects[BtnYes], mouseX, mouseY)) {
        currentMenu = MenuNone;
        newGame();
        render();
        return;
      }
      break;
  }

  if (gameOver(board)) {
    currentMenu = MenuGameOver;
    return;
  }

  if (!isPlayerTurn() || event->button != SDL_BUTTON_LEFT) { return; }

  int row = -1;
  int col = -1;

  if (mouseY > SIZE * 1 - 4 && mouseY < SIZE * 1 + 2) { // horizontal
    row = 0;
  } else if (mouseY > SIZE * 1 + 2 && mouseY < SIZE * 3 - 4) { // vertical
    row = 1;
  } else if (mouseY > SIZE * 3 - 4 && mouseY < SIZE * 3 + 2) { // horizontal
    row = 2;
  } else if (mouseY > SIZE * 3 + 2 && mouseY < SIZE * 5 - 4) { // vertical
    row = 3;
  } else if (mouseY > SIZE * 5 - 4 && mouseY < SIZE * 5 + 2) { // horizontal
    row = 4;
  } else if (mouseY > SIZE * 5 + 2 && mouseY < SIZE * 7 - 4) { // vertical
    row = 5;
  } else if (mouseY > SIZE * 7 - 4 && mouseY < SIZE * 7 + 2) { // horizontal
    row = 6;
  } else if (mouseY > SIZE * 7 + 2 && mouseY < SIZE * 9 - 4) { // vertical
    row = 7;
  } else if (mouseY > SIZE * 9 - 4 && mouseY < SIZE * 9 + 2) { // horizontal
    row = 8;
  } else if (mouseY > SIZE * 9 + 2 && mouseY < SIZE * 11 - 4) { // vertical
    row = 9;
  } else if (mouseY > SIZE * 11 - 4 && mouseY < SIZE * 11 + 2) { // horizontal
    row = 10;
  } else if (mouseY > SIZE * 11 + 2 && mouseY < SIZE * 13 - 4) { // vertical
    row = 11;
  }

  if (0 == row % 2) { // horizontal
    if (mouseX > SIZE * 1 + 4 && mouseX < SIZE * 3 - 3) {
      col = 0;
    } else if (mouseX > SIZE * 3 + 4 && mouseX < SIZE * 5 - 3) {
      col = 1;
    } else if (mouseX > SIZE * 5 + 4 && mouseX < SIZE * 7 - 3) {
      col = 2;
    } else if (mouseX > SIZE * 7 + 4 && mouseX < SIZE * 9 - 3) {
      col = 3;
    } else if (mouseX > SIZE * 9 + 4 && mouseX < SIZE * 11 - 3) {
      col = 4;
    }
  } else { // vertical
    if (mouseX > SIZE * 1 - 3 && mouseX < SIZE * 1 + 4) {
      col = 0;
    } else if (mouseX > SIZE * 3 - 3 && mouseX < SIZE * 3 + 4) {
      col = 1;
    } else if (mouseX > SIZE * 5 - 3 && mouseX < SIZE * 5 + 4) {
      col = 2;
    } else if (mouseX > SIZE * 7 - 3 && mouseX < SIZE * 7 + 4) {
      col = 3;
    } else if (mouseX > SIZE * 9 - 3 && mouseX < SIZE * 9 + 4) {
      col = 4;
    } else if (mouseX > SIZE * 11 - 3 && mouseX < SIZE * 11 + 4) {
      col = 5;
    }
  }

  if (board->legalMove(col, row)) {
    board->addMove(Move(col, row), P);

    if (board->canGoAgain) {
      render();
      return;
    }

    switchTurn();
    render();

    if (gameOver(board)) {
      currentMenu = MenuGameOver;
      return;
    }

    std::thread t{aiThread, this};
    t.join();

    render();
  }
}

void Game::switchTurn() {
  if (!gameOver(board)) {
    turn = isPlayerTurn() ? C : P;
  } else {
    currentMenu = MenuGameOver;
  }
}

bool Game::gameOver(Board *board) {
  return board->legalMoves().empty();
}

bool Game::isPlayerTurn() {
  return turn == P;
}

void Game::aiThread(Game *game) {
  do {
    std::this_thread::sleep_for(wait);
    game->aiTurn();
    game->render();
  } while (game->board->canGoAgain && !gameOver(game->board));

  game->switchTurn();
}

void Game::aiTurn() {
  if (isPlayerTurn())
    return;

  Move m = getAiMove();

  if (m.col > -1 && m.row > -1)
    board->addMove(m, C);
}

Move Game::getAiMove() {
  auto moves = board->legalMoves();
  std::shuffle(moves.begin(), moves.end(), *engine);

  int eval;
  int maxEval = std::numeric_limits<int>::min();
  Move bestMove = Move(-1, -1);

  for (auto &move : moves) {
    auto childBoard = Board(*board);
    childBoard.addMove(move, C);
    eval = minimax(&childBoard, DEPTH, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
    if (eval > maxEval) {
      maxEval = eval;
      bestMove = move;
    }
  }

  return bestMove;
}

int Game::minimax(Board *board, int depth, int alpha, int beta, bool maximizingPlayer) {
  if (depth == 0 || gameOver(board)) {
    return evaluate(board, maximizingPlayer ? P : C);
  }

  int eval;

  if (maximizingPlayer) {
    int maxEval = std::numeric_limits<int>::min();

    for (auto &move : board->legalMoves()) {
      auto childBoard = Board(*board);
      childBoard.addMove(move, C);
      eval = minimax(&childBoard, depth - 1, alpha, beta, false);
      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);
      if (beta <= alpha)
        break;
    }

    return maxEval;

  } else {
    int minEval = std::numeric_limits<int>::max();

    for (auto &move : board->legalMoves()) {
      auto childBoard = Board(*board);
      childBoard.addMove(move, P);
      eval = minimax(&childBoard, depth - 1, alpha, beta, true);
      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);
      if (beta <= alpha)
        break;
    }

    return minEval;
  }
}

int Game::evaluate(Board *board, int pc) {
  int other = getOther(pc);

  int pScore = board->getScore(pc);
  int cScore = board->getScore(other);
  int score = pScore - cScore;

  return pc == P ? -score : score;
}

void Game::writeText(const char *text, int x, int y, TTF_Font *font, SDL_Color color) {
  int w, h;

  SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  TTF_SizeText(font, text, &w, &h);
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

  SDL_RenderCopy(renderer, texture, nullptr, &rect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

int Game::getOther(int pc) {
  return pc == P ? C : P;
}

void Game::drawMenu() {
  switch (currentMenu) {
    case MenuGameOver:
      drawGameOverMenu();
      break;
  }
}

void Game::drawGameOverMenu() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xcc);
  SDL_Rect rect;
  rect.x = SCREEN_W / 2 - 185;
  rect.y = SCREEN_W / 2 - 95;
  rect.h = 190;
  rect.w = 370;
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
  SDL_RenderDrawRect(renderer, &rect);

  std::ostringstream result;
  result << "Game Over:  ";

  int winner = board->getWinner();

  if (winner == P) {
    result << "You won!";
  } else if (winner == C) {
    result << "You lost!";
  } else {
    result << "Tie";
  }

  SDL_Color color = {255, 255, 255, 0};
  writeText(result.str().c_str(), 172, 244, font21, color);
  writeText("Play Again?", 172, 285, font21, color);

  SDL_Rect clip[BtnCount];
  clip[BtnNo].x = 0;
  clip[BtnNo].y = BtnUp;
  clip[BtnNo].w = BTN_W;
  clip[BtnNo].h = BTN_H;

  clip[BtnYes].x = 0;
  clip[BtnYes].y = BtnUp;
  clip[BtnYes].w = BTN_W;
  clip[BtnYes].h = BTN_H;

  btnRects[BtnNo].x = (SCREEN_W / 2) - BTN_W - (BTN_SPACE / 2);
  btnRects[BtnNo].y = (int) 330;
  btnRects[BtnNo].w = BTN_W;
  btnRects[BtnNo].h = BTN_H;

  btnRects[BtnYes].x = (SCREEN_W / 2) + (BTN_SPACE / 2);
  btnRects[BtnYes].y = (int) 330;
  btnRects[BtnYes].w = BTN_W;
  btnRects[BtnYes].h = BTN_H;

  SDL_RenderCopy(renderer, btnTextures[BtnNo], &clip[BtnNo], &btnRects[BtnNo]);
  SDL_RenderCopy(renderer, btnTextures[BtnYes], &clip[BtnYes], &btnRects[BtnYes]);
}

bool Game::insideRect(SDL_Rect rect, int x, int y) {
  return x > rect.x &&
         x < rect.x + rect.w &&
         y > rect.y &&
         y < rect.y + rect.h;
}
