#include "Game.h"

auto main() -> int {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);

  Game *game = new Game("PigPen", &e);

  game->render();

  while (game->isRunning()) {
    game->handleEvents();
  }

  delete game;
  exit(EXIT_SUCCESS);
}
