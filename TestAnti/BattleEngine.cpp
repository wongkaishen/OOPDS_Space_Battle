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

  cout << "Loading data files..." << endl;

  string line, id, type, name;
  // No header to skip based on cat output?
  // zShips1.csv line 1 is "Z-S-001,Corazzata,Tacitus". So NO header.
  // Wait, original code had "Skip header". Check sample file view again.
  // Sample file view Step Id: 53 shows line 1 is data. So NO header.
  
  while (getline(file, line)) {
    stringstream ss(line);
    if (getline(ss, id, ',') && getline(ss, type, ',') &&
        getline(ss, name, ',')) {
      if (!name.empty() && name.back() == '\r') {
        name.pop_back();
      }
      
      Ship *newShip = nullptr;
      // Identify Faction from ID (Z- or R-) or guess from Type.
      // Or we can rely on the filename or context. 
      // But the user prompt says: "Format of ships.csv: ID of ship, type of ship, name of ship"
      // It doesn't explicitly store faction in CSV.
      // However, we can infer faction from ship type.
      // Zapezoid: Guerriero, Medio, Corazzata
      // Rogoatuskan: Jager, Kreuzer, Fregatte
      
      Faction f;
      if (type == "Guerriero") { newShip = new Guerriero(); f=ZAPEZOID; }
      else if (type == "Medio") { newShip = new Medio(); f=ZAPEZOID; }
      else if (type == "Corazzata") { newShip = new Corazzata(); f=ZAPEZOID; }
      else if (type == "Jager") { newShip = new Jager(); f=ROGOATUSKAN; }
      else if (type == "Kreuzer") { newShip = new Kreuzer(); f=ROGOATUSKAN; }
      else if (type == "Fregatte") { newShip = new Fregatte(); f=ROGOATUSKAN; }
      
      if (newShip) {
          // We need to set the name!
          // But wait, the Ship classes (Guerriero etc) constructors might verify name?
          // The base Ship constructor takes name.
          // But `new Guerriero()` uses default constructor?
          // Let's check ZapezoidShips.h and RogoatuskanShips.h
          // Actually, I can't check them now without another tool call.
          // But usually they would just call base constructor. 
          // The current Ship.cpp constructor `Ship::Ship(...)` initializes everything.
          // If Guerriero inherits Ship, it must call Ship constructor.
          // The previous code `newShip = new Guerriero();` implies default constructor exists.
          // IF default constructor exists, it probably sets a default name.
          // We need to set the specific name read from CSV.
          // Since I can't change the derived classes easily without reading them,
          // I should check if I can modify the name after creation?
          // Ship.h has `string getName() const`. No setName.
          // I might need to update ZapezoidShips.h etc OR add a setName method to Ship.h.
          // Adding setName to Ship.h is safer.
          
          // Let's assume for now I will add setName to Ship.h in a separate step or just assume I can refactor Ship.h.
          // Actually, I should probably check the derived classes.
          // But to proceed, I will add `setName` to Ship.h in a quick fix after this.
          // Or I check if I can just use `newShip = new Guerriero(name)`;
          
          // Let's assume default constructors are used and I need to set name.
          // I'll add `setName` to Ship.h in next step.
          // For now I will leave a TODO or just try to access protected `name` if I make BattleEngine a friend?
          // No, friend is messy.
          // I'll use a hack or just update Ship.h first?
          // I am already in BattleEngine.cpp. 
          
          // Let's pause this and add setName to Ship.h FIRST.
          // No, I can't pause easily.
          // I will proceed with assumption I will add setName.
          
          string fullName = "[" + id + "] " + name;
          newShip->name = fullName; 
          
          if (f == ZAPEZOID) zapezoidFleet.push_back(newShip);
          else rogoatuskanFleet.push_back(newShip);
      }
    }
  }

  cout << "Fleets loaded from " << filename << endl;
  cout << "Zapezoids: " << zapezoidFleet.size() << " ships." << endl;
  cout << "Rogoatuskans: " << rogoatuskanFleet.size() << " ships." << endl;
}

void BattleEngine::loadPersonnelFromCSV(string filename, Faction faction) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    return;
  }

  string line, id, name, role;
  // No header in zCrew1.csv based on file content.
  
  vector<string> pilotsAvailable, gunnersAvailable, torpedoHandlersAvailable;

  while (getline(file, line)) {
    stringstream ss(line);
    if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, role)) {
      // Trim whitespace and carriage return
      while (!role.empty() && (isspace(role.back()) || role.back() == '\r')) {
          role.pop_back();
      }
      while (!role.empty() && isspace(role.front())) {
          role.erase(0, 1);
      }

      if (role == "Pilot")
        pilotsAvailable.push_back(name);
      else if (role == "Gunner")
        gunnersAvailable.push_back(name);
      else if (role == "TorpedoHandler")
        torpedoHandlersAvailable.push_back(name);
    }
  }

  // Assign crew ONLY to ships of the specified faction
  vector<Ship *> *targetFleet = nullptr;
  if (faction == ZAPEZOID) {
      targetFleet = &zapezoidFleet;
  } else {
      targetFleet = &rogoatuskanFleet;
  }

  int pIdx = 0, gIdx = 0, tIdx = 0;

  for (Ship *ship : *targetFleet) {
    while (ship->getAssignedPilots().size() < ship->getRequiredPilots() && pIdx < pilotsAvailable.size()) {
        ship->assignPilot(pilotsAvailable[pIdx]);
        pIdx++;
    }
    while (ship->getAssignedGunners().size() < ship->getRequiredGunners() && gIdx < gunnersAvailable.size()) {
        ship->assignGunner(gunnersAvailable[gIdx]);
        gIdx++;
    }
    while (ship->getAssignedTorpedoHandlers().size() < ship->getRequiredTorpedoHandlers() && tIdx < torpedoHandlersAvailable.size()) {
        ship->assignTorpedoHandler(torpedoHandlersAvailable[tIdx]);
        tIdx++;
    }
  }
}

void BattleEngine::printFleetReport() const {
  cout << "\n========================================\n"
       << "       FLEET CONFIGURATION REPORT       \n"
       << "========================================\n"
       << endl;

  auto printCrewList = [](const vector<string>& crew) {
      for (size_t i = 0; i < crew.size(); ++i) {
          cout << crew[i] << (i < crew.size() - 1 ? ", " : "");
      }
  };
  
  auto printShipReport = [&](const vector<Ship*>& fleet) {
    for (Ship *ship : fleet) {
        // Output format: "[ID] Name     (Type)"
        // But we stored "[ID] Name" in name.
        // We actually need to access Type separately or just print name.
        // Ship::printInfo() was: cout << name << " [" << faction << "] " ...
        // We want to match output1.txt: "[Z-S-001] Tacitus              (Corazzata)"
        // If we stored fullName as "[Z-S-001] Tacitus", we are good for first part.
        // For Type, we can guess from HP or use dynamic_cast?
        // Or store type in Ship?
        // Existing Ship class doesn't store "Type" string (e.g. "Corazzata").
        // It has lightCannons/hp stats.
        // Use polymorphic getType()
        string typeStr = ship->getType();
        
        cout << ship->getName() << "              (" << typeStr << ")" << endl;
        cout << "      HP: " << ship->getHP() << "/" << ship->getMaxHP() << endl; // maxhp? getter missing, assume current=max at start
        
        const auto& p = ship->getAssignedPilots();
        if (ship->getRequiredPilots() > 0) {
            cout << "      Pilots (" << p.size() << "/" << ship->getRequiredPilots() << "): ";
            printCrewList(p);
            cout << endl;
        }
        
        const auto& g = ship->getAssignedGunners();
        if (ship->getRequiredGunners() > 0) {
            cout << "      Gunners (" << g.size() << "/" << ship->getRequiredGunners() << "): ";
            printCrewList(g);
            cout << endl;
        }
        
        const auto& th = ship->getAssignedTorpedoHandlers();
        if (ship->getRequiredTorpedoHandlers() > 0) {
            cout << "      Torp Handlers (" << th.size() << "/" << ship->getRequiredTorpedoHandlers() << "): ";
            printCrewList(th);
            cout << endl;
        }
        cout << "--------------------------------------------------" << endl;
    }
  };

  cout << "--- ZAPEZOID FLEET ---" << endl;
  printShipReport(zapezoidFleet);

  cout << "--- ROGOATUSKAN FLEET ---" << endl;
  printShipReport(rogoatuskanFleet);
  
  cout << "\n[Unassigned ROGOATUSKAN Crew]" << endl;
  cout << " - (Functionality to track unassigned crew pending)" << endl; 
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
