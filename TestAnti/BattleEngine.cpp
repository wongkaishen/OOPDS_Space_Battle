#include "BattleEngine.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

BattleEngine::BattleEngine() : turnCount(0) { srand(time(nullptr)); }

BattleEngine::~BattleEngine() {
  for (Ship *s : zapezoidFleet)
    delete s;
  for (Ship *s : rogoatuskanFleet)
    delete s;
}

void BattleEngine::loadFleetsFromCSV(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    return;
  }

  string line, faction, type, countStr;
  // Skip header
  getline(file, line);

  while (getline(file, line)) {
    stringstream ss(line);
    if (getline(ss, faction, ',') && getline(ss, type, ',') &&
        getline(ss, countStr, ',')) {
      // Remove any carriage return characters if present (handle Windows line
      // endings)
      if (!countStr.empty() && countStr.back() == '\r') {
        countStr.pop_back();
      }
      int count = 0;
      try {
        count = stoi(countStr);
      } catch (...) {
        continue;
      }

      for (int i = 0; i < count; ++i) {
        Ship *newShip = nullptr;
        if (faction == "Zapezoid") {
          if (type == "Guerriero")
            newShip = new Guerriero();
          else if (type == "Medio")
            newShip = new Medio();
          else if (type == "Corazzata")
            newShip = new Corazzata();
        } else if (faction == "Rogoatuskan") {
          if (type == "Jager")
            newShip = new Jager();
          else if (type == "Kreuzer")
            newShip = new Kreuzer();
          else if (type == "Fregatte")
            newShip = new Fregatte();
        }

        if (newShip) {
          if (faction == "Zapezoid")
            zapezoidFleet.push_back(newShip);
          else
            rogoatuskanFleet.push_back(newShip);
        }
      }
    }
  }

  cout << "Fleets loaded from " << filename << endl;
  cout << "Zapezoids: " << zapezoidFleet.size() << " ships." << endl;
  cout << "Rogoatuskans: " << rogoatuskanFleet.size() << " ships." << endl;
}

void BattleEngine::loadPersonnelFromCSV(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    return;
  }

  string line, name, role;
  getline(file, line); // Skip header

  vector<string> pilotsAvailable, gunnersAvailable, torpedoHandlersAvailable;

  while (getline(file, line)) {
    stringstream ss(line);
    if (getline(ss, name, ',') && getline(ss, role, ',')) {
      if (!role.empty() && role.back() == '\r')
        role.pop_back(); // Handle Windows EOL

      if (role == "Pilot")
        pilotsAvailable.push_back(name);
      else if (role == "Gunner")
        gunnersAvailable.push_back(name);
      else if (role == "TorpedoHandler")
        torpedoHandlersAvailable.push_back(name);
    }
  }

  // Helper lambda to assign crew
  auto assignCrew = [&](vector<Ship *> &fleet) {
    for (Ship *ship : fleet) {
      // Assign Pilots
      while (ship->getRequiredPilots() > 0 && !pilotsAvailable.empty()) {
        ship->assignPilot();
        pilotsAvailable.pop_back();
        // Check if ship is fully staffed for this role?
        // Currently Ship doesn't track assigned count vs required getter, but
        // we can infer Actually Ship::assignPilot just increments a counter. We
        // should check if we have reached the ship's requirement. But Ship
        // doesn't expose "getAssignedPilots". We can assume we shouldn't
        // over-assign. A simpler way is: We loop N times where N is required
        // count. Oops, the loop "while (ship->getRequiredPilots() > 0 ..."
        // implies getRequired decrements? No, getRequiredPilots returns the
        // total requirement. We need to track how many we assigned. Let's
        // refactor this loop.
      }
    }
  };

  // Better allocation logic
  // We iterate through fleets and assign available crew.
  // Concatenate fleets for easier processing
  vector<Ship *> allShips = zapezoidFleet;
  allShips.insert(allShips.end(), rogoatuskanFleet.begin(),
                  rogoatuskanFleet.end());

  int pIdx = 0, gIdx = 0, tIdx = 0;

  for (Ship *ship : allShips) {
    for (int i = 0; i < ship->getRequiredPilots(); ++i) {
      if (pIdx < pilotsAvailable.size()) {
        ship->assignPilot();
        pIdx++;
      }
    }
    for (int i = 0; i < ship->getRequiredGunners(); ++i) {
      if (gIdx < gunnersAvailable.size()) {
        ship->assignGunner();
        gIdx++;
      }
    }
    for (int i = 0; i < ship->getRequiredTorpedoHandlers(); ++i) {
      if (tIdx < torpedoHandlersAvailable.size()) {
        ship->assignTorpedoHandler();
        tIdx++;
      }
    }
  }

  cout << "Personnel loaded and assigned." << endl;
  cout << "Assigned Pilots: " << pIdx << "/" << pilotsAvailable.size() << endl;
  cout << "Assigned Gunners: " << gIdx << "/" << gunnersAvailable.size()
       << endl;
  cout << "Assigned TorpedoHandlers: " << tIdx << "/"
       << torpedoHandlersAvailable.size() << endl;
}

void BattleEngine::init() {
  // Add sample ships to Zapezoid Fleet
  zapezoidFleet.push_back(new Guerriero());
  zapezoidFleet.push_back(new Medio());
  zapezoidFleet.push_back(new Corazzata());

  // Add sample ships to Rogoatuskan Fleet
  rogoatuskanFleet.push_back(new Jager());
  rogoatuskanFleet.push_back(new Kreuzer());
  rogoatuskanFleet.push_back(new Fregatte());

  cout << "Battle Initialized!" << endl;
  cout << "Zapezoids: " << zapezoidFleet.size() << " ships." << endl;
  cout << "Rogoatuskans: " << rogoatuskanFleet.size() << " ships." << endl;
}

bool BattleEngine::isFleetDefeated(const vector<Ship *> &fleet) const {
  for (Ship *s : fleet) {
    if (s->isAlive())
      return false;
  }
  return true;
}

Ship *BattleEngine::getRandomTarget(const vector<Ship *> &fleet) {
  vector<Ship *> livingShips;
  for (Ship *s : fleet) {
    if (s->isAlive())
      livingShips.push_back(s);
  }

  if (livingShips.empty())
    return nullptr;
  return livingShips[rand() % livingShips.size()];
}

void BattleEngine::processTurn() {
  turnCount++;
  cout << "\n--- Turn " << turnCount << " ---" << endl;

  map<Ship *, int> pendingDamage;

  // Zapezoids fire
  for (Ship *shooter : zapezoidFleet) {
    if (!shooter->isAlive())
      continue;
    Ship *target = getRandomTarget(rogoatuskanFleet);
    if (target) {
      int damage = shooter->calculateTotalFirePower();
      // Simple hit chance logic could go here (e.g., 50% chance to hit)
      // For this basic version, we assume 100% accuracy or simplified model
      pendingDamage[target] += damage;
      cout << *shooter << " fires at " << *target << " for " << damage
           << " damage." << endl;
    }
  }

  // Rogoatuskans fire
  for (Ship *shooter : rogoatuskanFleet) {
    if (!shooter->isAlive())
      continue;
    Ship *target = getRandomTarget(zapezoidFleet);
    if (target) {
      int damage = shooter->calculateTotalFirePower();
      pendingDamage[target] += damage;
      cout << *shooter << " fires at " << *target << " for " << damage
           << " damage." << endl;
    }
  }

  // Apply Damage
  cout << "\nResolving Damage..." << endl;
  for (auto const &[ship, damage] : pendingDamage) {
    ship->takeDamage(damage);
    if (!ship->isAlive()) {
      cout << ship->getName() << " has been destroyed!" << endl;
    }
  }
}

void BattleEngine::runBattle() {
  while (!isFleetDefeated(zapezoidFleet) &&
         !isFleetDefeated(rogoatuskanFleet)) {
    processTurn();
  }

  cout << "\n--- Battle Over ---" << endl;
  if (isFleetDefeated(zapezoidFleet)) {
    cout << "Rogoatuskans Win!" << endl;
  } else {
    cout << "Zapezoids Win!" << endl;
  }
}
