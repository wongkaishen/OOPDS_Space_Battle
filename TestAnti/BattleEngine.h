#ifndef BATTLE_ENGINE_H
#define BATTLE_ENGINE_H

#include "RogoatuskanShips.h"
#include "Ship.h"
#include "ZapezoidShips.h"
#include <memory>
#include <string>
#include <vector>

class BattleEngine {
private:
  vector<Ship *> zapezoidFleet;
  vector<Ship *> rogoatuskanFleet;
  int turnCount;

  void cleanupDeadShips();
  bool isFleetDefeated(const vector<Ship *> &fleet) const;
  Ship *getRandomTarget(const vector<Ship *> &fleet);

public:
  BattleEngine();
  ~BattleEngine();

  void init();
  void loadFleetsFromCSV(std::string filename);
  void loadPersonnelFromCSV(std::string filename, Faction faction);
  void printFleetReport() const;
  void runBattle();
  void processTurn();
};

#endif
