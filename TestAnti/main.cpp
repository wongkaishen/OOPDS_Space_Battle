#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>

using namespace std;

// ==========================================
// Ship.h & Ship.cpp
// ==========================================

enum Faction { ZAPEZOID, ROGOATUSKAN };

class Ship {
protected:
  string name;
  Faction faction;
  int hp;
  int maxHp;
  int pilots;
  int gunners;
  int torpedoHandlers;
  int lightCannons; // Power per cannon
  int torpedoes;    // Power per torpedo
  int lightCannonCount;
  int torpedoCount;

  // Hit Chances
  int lightCannonHitChance;
  int torpedoHitChance;

  // Assigned Crew
  vector<string> assignedPilots;
  vector<string> assignedGunners;
  vector<string> assignedTorpedoHandlers;

  friend class BattleEngine; 

public:
  Ship(string n, Faction f, int h, int p, int g, int th, int lc_pow, int t_pow,
       int lc_count, int t_count, int lc_hit_chance, int t_hit_chance);
  virtual ~Ship() {}

  // Getters
  string getName() const { return name; }
  virtual string getType() const { return "Ship"; }
  Faction getFaction() const { return faction; }
  int getHP() const { return hp; }
  int getMaxHP() const { return maxHp; }
  bool isAlive() const { return hp > 0; }
  const vector<string>& getAssignedPilots() const { return assignedPilots; }
  const vector<string>& getAssignedGunners() const { return assignedGunners; }
  const vector<string>& getAssignedTorpedoHandlers() const { return assignedTorpedoHandlers; }

  // New Hit Chance Getters
  double getLightCannonHitChance() const;
  double getTorpedoHitChance() const;

  // Actions
  virtual int calculateTotalFirePower() const;
  int getLightCannonDamage() const;
  int getTorpedoDamage() const;
  void takeDamage(int damage);
  void assignPilot(string name) { assignedPilots.push_back(name); }
  void assignGunner(string name) { assignedGunners.push_back(name); }
  void assignTorpedoHandler(string name) { assignedTorpedoHandlers.push_back(name); }

  // Requirements
  int getRequiredPilots() const { return pilots; }
  int getRequiredGunners() const { return gunners; }
  int getRequiredTorpedoHandlers() const { return torpedoHandlers; }

  // Display
  virtual void printInfo() const;

  // Operator Overloading
  friend ostream &operator<<(ostream &os, const Ship &ship);
};

Ship::Ship(string n, Faction f, int h, int p, int g, int th, int lc_pow,
           int t_pow, int lc_count, int t_count, int lc_hit_chance, int t_hit_chance)
    : name(n), faction(f), maxHp(h), hp(h), pilots(p), gunners(g),
      torpedoHandlers(th), lightCannons(lc_pow), torpedoes(t_pow),
      lightCannonCount(lc_count), torpedoCount(t_count), 
      lightCannonHitChance(lc_hit_chance), torpedoHitChance(t_hit_chance) {}

int Ship::getLightCannonDamage() const {
  if (pilots > 0 && assignedPilots.empty())
    return 0; // No power if no pilot or needed

  double cannonEfficiency = 1.0;
  if (gunners > 0) {
    cannonEfficiency = (double)assignedGunners.size() / gunners;
    if (cannonEfficiency > 1.0)
      cannonEfficiency = 1.0;
  }
  return (lightCannons * lightCannonCount) * cannonEfficiency;
}

int Ship::getTorpedoDamage() const {
  if (pilots > 0 && assignedPilots.empty())
    return 0; 

  double torpedoEfficiency = 1.0;
  if (torpedoHandlers > 0) {
    torpedoEfficiency = (double)assignedTorpedoHandlers.size() / torpedoHandlers;
    if (torpedoEfficiency > 1.0)
      torpedoEfficiency = 1.0;
  }
  return (torpedoes * torpedoCount) * torpedoEfficiency;
}

int Ship::calculateTotalFirePower() const {
  return getLightCannonDamage() + getTorpedoDamage();
}

double Ship::getLightCannonHitChance() const {
  double chance = (double)lightCannonHitChance;
  // Penalty: If 2 pilots required but only 1 assigned, increase hit chance by 25%
  if (pilots == 2 && assignedPilots.size() == 1) {
      chance *= 1.25; 
  }
  return chance;
}

double Ship::getTorpedoHitChance() const {
  double chance = (double)torpedoHitChance;
  // Penalty: If 2 pilots required but only 1 assigned, increase hit chance by 25%
  if (pilots == 2 && assignedPilots.size() == 1) {
      chance *= 1.25; 
  }
  return chance;
}

void Ship::takeDamage(int damage) {
  hp -= damage;
  if (hp < 0)
    hp = 0;
}

void Ship::printInfo() const {
  cout << name << " [" << (faction == ZAPEZOID ? "Zapezvoid" : "Rogoatuskan")
       << "] "
       << "HP: " << hp << "/" << maxHp;
}

ostream &operator<<(ostream &os, const Ship &ship) {
  os << ship.name << " (" << ship.hp << "/" << ship.maxHp << ")";
  return os;
}

// ==========================================
// ZapezoidShips.h
// ==========================================

class Guerriero : public Ship {
public:
  Guerriero() : Ship("Guerriero", ZAPEZOID, 123, 1, 1, 0, 96, 0, 1, 0, 26, 6) {}
  string getType() const override { return "Guerriero"; }
};

class Medio : public Ship {
public:
  Medio() : Ship("Medio", ZAPEZOID, 214, 1, 2, 0, 134, 0, 2, 0, 31, 11) {}
  string getType() const override { return "Medio"; }
};

class Corazzata : public Ship {
public:
  Corazzata() : Ship("Corazzata", ZAPEZOID, 1031, 2, 10, 4, 164, 293, 10, 4, 50, 25) {}
  string getType() const override { return "Corazzata"; }
};

// ==========================================
// RogoatuskanShips.h
// ==========================================

class Jager : public Ship {
public:
  Jager() : Ship("Jager", ROGOATUSKAN, 112, 1, 1, 0, 101, 0, 1, 0, 24, 5) {}
  string getType() const override { return "Jager"; }
};

class Kreuzer : public Ship {
public:
  Kreuzer() : Ship("Kreuzer", ROGOATUSKAN, 212, 1, 2, 0, 132, 0, 2, 0, 29, 10) {}
  string getType() const override { return "Kreuzer"; }
};

class Fregatte : public Ship {
public:
  Fregatte() : Ship("Fregatte", ROGOATUSKAN, 1143, 2, 11, 5, 159, 282, 11, 5, 60, 30) {}
  string getType() const override { return "Fregatte"; }
};

// ==========================================
// BattleEngine.h & BattleEngine.cpp
// ==========================================

class BattleEngine {
private:
  vector<Ship *> zapezoidFleet;
  vector<Ship *> rogoatuskanFleet;
  int turnCount;

  void cleanupDeadShips(); // Still declaring it even if not implemented, as in original.
  bool isFleetDefeated(const vector<Ship *> &fleet) const;
  Ship *getRandomTarget(const vector<Ship *> &fleet);

public:
  BattleEngine();
  ~BattleEngine();

  void init();
  void loadFleetsFromCSV(string filename);
  void loadPersonnelFromCSV(string filename, Faction faction);
  void printFleetReport() const;
  void runBattle();
  void processTurn();
};

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
  
  while (getline(file, line)) {
    stringstream ss(line);
    if (getline(ss, id, ',') && getline(ss, type, ',') &&
        getline(ss, name, ',')) {
      if (!name.empty() && name.back() == '\r') {
        name.pop_back();
      }
      
      Ship *newShip = nullptr;
      Faction f;
      if (type == "Guerriero") { newShip = new Guerriero(); f=ZAPEZOID; }
      else if (type == "Medio") { newShip = new Medio(); f=ZAPEZOID; }
      else if (type == "Corazzata") { newShip = new Corazzata(); f=ZAPEZOID; }
      else if (type == "Jager") { newShip = new Jager(); f=ROGOATUSKAN; }
      else if (type == "Kreuzer") { newShip = new Kreuzer(); f=ROGOATUSKAN; }
      else if (type == "Fregatte") { newShip = new Fregatte(); f=ROGOATUSKAN; }
      
      if (newShip) {
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

      string lowerRole = role;
      transform(lowerRole.begin(), lowerRole.end(), lowerRole.begin(),
                [](unsigned char c){ return tolower(c); });

      if (lowerRole == "pilot" || lowerRole == "pilots")
        pilotsAvailable.push_back(name);
      else if (lowerRole == "gunner" || lowerRole == "gunners")
        gunnersAvailable.push_back(name);
      else if (lowerRole == "torpedohandler" || lowerRole == "torpedohandlers")
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
        string typeStr = ship->getType();
        
        cout << ship->getName() << "              (" << typeStr << ")" << endl;
        cout << "      HP: " << ship->getHP() << "/" << ship->getMaxHP() << endl; 
        
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
      // Light Cannon Attack
      int lcDamage = shooter->getLightCannonDamage();
      if (lcDamage > 0) {
        double hitRoll = (rand() % 10000) / 100.0;
        if (hitRoll < target->getLightCannonHitChance()) {
            pendingDamage[target] += lcDamage;
            cout << *shooter << " fires Light Cannons at " << *target << " for " << lcDamage
                 << " damage. [HIT] (Roll: " << hitRoll << " vs " << target->getLightCannonHitChance() << "%)" << endl;
        } else {
             cout << *shooter << " fires Light Cannons at " << *target << " but MISSES! (Roll: " << hitRoll << " vs " << target->getLightCannonHitChance() << "%)" << endl;
        }
      }

      // Torpedo Attack
      int tDamage = shooter->getTorpedoDamage();
      if (tDamage > 0) {
        double hitRoll = (rand() % 10000) / 100.0;
        if (hitRoll < target->getTorpedoHitChance()) {
            pendingDamage[target] += tDamage;
            cout << *shooter << " fires Torpedoes at " << *target << " for " << tDamage
                 << " damage. [HIT] (Roll: " << hitRoll << " vs " << target->getTorpedoHitChance() << "%)" << endl;
        } else {
             cout << *shooter << " fires Torpedoes at " << *target << " but MISSES! (Roll: " << hitRoll << " vs " << target->getTorpedoHitChance() << "%)" << endl;
        }
      }
    }
  }

  // Rogoatuskans fire
  for (Ship *shooter : rogoatuskanFleet) {
    if (!shooter->isAlive())
      continue;
    Ship *target = getRandomTarget(zapezoidFleet);
    if (target) {
      // Light Cannon Attack
      int lcDamage = shooter->getLightCannonDamage();
      if (lcDamage > 0) {
        double hitRoll = (rand() % 10000) / 100.0;
        if (hitRoll < target->getLightCannonHitChance()) {
            pendingDamage[target] += lcDamage;
            cout << *shooter << " fires Light Cannons at " << *target << " for " << lcDamage
                 << " damage. [HIT] (Roll: " << hitRoll << " vs " << target->getLightCannonHitChance() << "%)" << endl;
        } else {
             cout << *shooter << " fires Light Cannons at " << *target << " but MISSES! (Roll: " << hitRoll << " vs " << target->getLightCannonHitChance() << "%)" << endl;
        }
      }

      // Torpedo Attack
      int tDamage = shooter->getTorpedoDamage();
      if (tDamage > 0) {
        double hitRoll = (rand() % 10000) / 100.0;
        if (hitRoll < target->getTorpedoHitChance()) {
            pendingDamage[target] += tDamage;
            cout << *shooter << " fires Torpedoes at " << *target << " for " << tDamage
                 << " damage. [HIT] (Roll: " << hitRoll << " vs " << target->getTorpedoHitChance() << "%)" << endl;
        } else {
             cout << *shooter << " fires Torpedoes at " << *target << " but MISSES! (Roll: " << hitRoll << " vs " << target->getTorpedoHitChance() << "%)" << endl;
        }
      }
    }
  }
  // Apply Damage
  if (pendingDamage.empty()) {
    cout << "\nAll misses!" << endl;
  } else {
    cout << "\nResolving Damage..." << endl;
    for (auto const &[ship, damage] : pendingDamage) {
      ship->takeDamage(damage);
      if (!ship->isAlive()) {
        cout << ship->getName() << " has been destroyed!" << endl;
      }
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

// ==========================================
// Main Function
// ==========================================

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
  engine.loadPersonnelFromCSV(argv[2], ZAPEZOID);
  engine.loadPersonnelFromCSV(argv[4], ROGOATUSKAN);
  
  engine.printFleetReport();
  
  cout << "========================================" << endl;
  cout << "           BATTLE COMMENCING            " << endl;
  cout << "========================================" << endl;

  engine.runBattle();
  return 0;
}
