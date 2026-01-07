#include "BattleEngine.h"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 5) {
      std::cerr << "Usage: " << argv[0] << " <zapezoid_ships.csv> <zapezoid_crew.csv> <rogoatuskan_ships.csv> <rogoatuskan_crew.csv>" << std::endl;
      return 1;
  }

  BattleEngine engine;
  
  // Load Ship Files (Arg 1 and 3)
  engine.loadFleetsFromCSV(argv[1]);
  engine.loadFleetsFromCSV(argv[3]);
  
  // Load Crew Files (Arg 2 and 4)
  // Note: loadPersonnelFromCSV loads ALL crew into one big pool in existing logic?
  // Let's check BattleEngine.cpp again.
  // Yes, it loads into vectors `pilotsAvailable`, etc.
  // Then `assignCrew` iterates through ALL ships (Z + R).
  // If we call it twice, we might get mixed results if we strictly want Z crew for Z ships?
  // The assignment instruction: "XyloTT9L zShips1.csv zCrew1.csv rShips1.csv rCrew1.csv"
  // It separates them.
  // Logic in BattleEngine.cpp:
  // `vector<Ship *> allShips = zapezoidFleet; allShips.insert(... rogoatuskanFleet ...)`
  // It mixes them for assignment!
  // And `loadPersonnelFromCSV` clears the available vectors locally?
  // No, `vector<string> pilotsAvailable...` are local variables in `loadPersonnelFromCSV`.
  // So if we call it twice:
  // Call 1 (zCrew): Loads Z crew. Assigns to (Z+R) fleet?
  //    Wait, `zapezoidFleet` is populated. `rogoatuskanFleet` might be empty if we call this interstitial.
  //    If we load Z ships, then Z crew. R fleet is empty. Z crew assigned to Z ships.
  //    Then load R ships.
  //    Then load R crew. Z ships are already assigned?
  //    `if (ship->getRequiredPilots() > 0)`... `assignPilot` increments assigned count.
  //    `Ship::assignPilot` pushes back.
  //    We need to check if ship is FULL.
  //    I updated `Ship.h` to have `getRequiredPilots`.
  //    In `BattleEngine.cpp`: `for (int i = 0; i < ship->getRequiredPilots(); ++i)`
  //    This loop always tries to assign N pilots.
  //    If ship already has pilots, we might over-assign!
  //    `ship->assignPilot` just pushes back.
  //    So, `Ship` needs a way to say "I am full". OR `BattleEngine` check `getAssignedPilots().size() < getRequiredPilots()`.
  //    My previous edit to `BattleEngine.cpp` DID NOT Add this check loop constraint change.
  //    It still does: `for (int i = 0; i < ship->getRequiredPilots(); ++i)` logic.
  //    This logic is flawed if called twice.
  //    Correct logic: `while (ship->getAssignedPilots().size() < ship->getRequiredPilots() && !pilotsAvailable.empty())`
  
  // I must fix BattleEngine.cpp loop logic FIRST.
  // But for main.cpp, the order is: load zShips, load zCrew, load rShips, load rCrew.
  // IF I load zShips, then zCrew: Z ships get Z crew. R fleet empty.
  // THEN load rShips.
  // THEN load rCrew. All ships (Z+R) processed. Z ships are full. R ships get R crew.
  // This logic "accidentally" works if I use the "while not full" logic.
  // But current logic "for i < required" will FORCE assignment even if full.
  
  // So I MUST update BattleEngine.cpp assignment loop.
  
  engine.loadPersonnelFromCSV(argv[2], ZAPEZOID);
  engine.loadPersonnelFromCSV(argv[4], ROGOATUSKAN);
  
  engine.printFleetReport();
  
  cout << "========================================" << endl;
  cout << "           BATTLE COMMENCING            " << endl;
  cout << "========================================" << endl;

  engine.runBattle();
  return 0;
}
