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

  drawDots();

//  drawGrid();
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
        //handleClick(&event.button);
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
      filledCircleRGBA(renderer, xx, yy - 1, 4, 0x00, 0x00, 0xff, 0x66);
      filledCircleRGBA(renderer, xx, yy + 1, 4, 0x00, 0x00, 0xff, 0x66);
    }
  }
}
