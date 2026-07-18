#include "Game.h"

int main(int argc, char *argv[]) {
  srand(time(nullptr));
  Game g("config.txt");
  g.run();
}
