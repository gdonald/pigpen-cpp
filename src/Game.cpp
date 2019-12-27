#include "Game.h"

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

Game::Game(const char *title) {
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

  boardFont = TTF_OpenFont(FONT, 212);
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

  //  drawPCs();
//  drawMenu();

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

void Game::handleClick(SDL_MouseButtonEvent *event) {
  int row = -1;
  int col = -1;

         if (mouseY > SIZE *  1 - 4 && mouseY < SIZE *  1 + 2) { // horizontal
    row = 0;
  } else if (mouseY > SIZE *  1 + 2 && mouseY < SIZE *  3 - 4) { // vertical
    row = 1;
  } else if (mouseY > SIZE *  3 - 4 && mouseY < SIZE *  3 + 2) { // horizontal
    row = 2;
  } else if (mouseY > SIZE *  3 + 2 && mouseY < SIZE *  5 - 4) { // vertical
    row = 3;
  } else if (mouseY > SIZE *  5 - 4 && mouseY < SIZE *  5 + 2) { // horizontal
    row = 4;
  } else if (mouseY > SIZE *  5 + 2 && mouseY < SIZE *  7 - 4) { // vertical
    row = 5;
  } else if (mouseY > SIZE *  7 - 4 && mouseY < SIZE *  7 + 2) { // horizontal
    row = 6;
  } else if (mouseY > SIZE *  7 + 2 && mouseY < SIZE *  9 - 4) { // vertical
    row = 7;
  } else if (mouseY > SIZE *  9 - 4 && mouseY < SIZE *  9 + 2) { // horizontal
    row = 8;
  } else if (mouseY > SIZE *  9 + 2 && mouseY < SIZE * 11 - 4) { // vertical
    row = 9;
  } else if (mouseY > SIZE * 11 - 4 && mouseY < SIZE * 11 + 2) { // horizontal
    row = 10;
  } else if (mouseY > SIZE * 11 + 2 && mouseY < SIZE * 13 - 4) { // vertical
    row = 11;
  }

         if (mouseX > SIZE *  1 - 3 && mouseX < SIZE *  1 + 4) { // horizontal
    col = 0;
  } else if (mouseX > SIZE *  3 - 3 && mouseX < SIZE *  3 + 4) { // horizontal
    col = 1;
  } else if (mouseX > SIZE *  5 - 3 && mouseX < SIZE *  5 + 4) { // horizontal
    col = 2;
  } else if (mouseX > SIZE *  7 - 3 && mouseX < SIZE *  5 + 4) { // horizontal
    col = 3;
  } else if (mouseX > SIZE *  9 - 3 && mouseX < SIZE *  9 + 4) { // horizontal
    col = 4;
  } else if (mouseX > SIZE * 11 - 3 && mouseX < SIZE * 11 + 4) { // horizontal
    col = 5;
  }

  std::cout << "mouseY: " << mouseY << std::endl;
  std::cout << "mouseX: " << mouseX << std::endl;

  std::cout << "row: " << row << std::endl;
  std::cout << "col: " << col << std::endl;

  if (row == -1 || col == -1) { return; }

  board->moves[row][col] = C;
  render();
}
