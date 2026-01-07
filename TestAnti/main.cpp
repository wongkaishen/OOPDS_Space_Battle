#include "BattleEngine.h"
#include <iostream>

int main() {
  BattleEngine engine;
  engine.loadFleetsFromCSV("ship.csv");
  engine.loadPersonnelFromCSV("crew.csv");
  engine.runBattle();
  return 0;
}
