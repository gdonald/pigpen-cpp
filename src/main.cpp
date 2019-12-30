#include "Game.h"

auto main() -> int {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine e(seed);

  auto game = std::make_unique<Game>("PigPen", &e);

  game->render();

  while (game->isRunning()) {
    game->handleEvents();
  }

  exit(EXIT_SUCCESS);
}
