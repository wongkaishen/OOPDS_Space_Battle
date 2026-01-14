// Group Germany TT3L
// YAP HUI CHI
// WONG KAI SHEN
// SOH YI HERN

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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
  const vector<string> &getAssignedPilots() const { return assignedPilots; }
  const vector<string> &getAssignedGunners() const { return assignedGunners; }
  const vector<string> &getAssignedTorpedoHandlers() const {
    return assignedTorpedoHandlers;
  }

  // New Hit Chance Getters
  double getLightCannonHitChance() const;
  double getTorpedoHitChance() const;

  // New Accessors for Individual Fire
  int getLightCannonPower() const { return lightCannons; }
  int getTorpedoPower() const { return torpedoes; }
  int getEffectiveLightCannonCount() const;
  int getEffectiveTorpedoCount() const;

  // Actions
  virtual int calculateTotalFirePower() const;
  int getLightCannonDamage() const;
  int getTorpedoDamage() const;
  void takeDamage(int damage);
  void assignPilot(string name) { assignedPilots.push_back(name); }
  void assignGunner(string name) { assignedGunners.push_back(name); }
  void assignTorpedoHandler(string name) {
    assignedTorpedoHandlers.push_back(name);
  }

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
           int t_pow, int lc_count, int t_count, int lc_hit_chance,
           int t_hit_chance)
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
    torpedoEfficiency =
        (double)assignedTorpedoHandlers.size() / torpedoHandlers;
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
  // Penalty: If 2 pilots required but only 1 assigned, increase hit chance by
  // 25%
  if (pilots == 2 && assignedPilots.size() == 1) {
    chance *= 1.25;
  }
  return chance;
}

double Ship::getTorpedoHitChance() const {
  double chance = (double)torpedoHitChance;
  // Penalty: If 2 pilots required but only 1 assigned, increase hit chance by
  // 25%
  if (pilots == 2 && assignedPilots.size() == 1) {
    chance *= 1.25;
  }
  return chance;
}

int Ship::getEffectiveLightCannonCount() const {
  if (pilots > 0 && assignedPilots.empty())
    return 0;
  return min(lightCannonCount, (int)assignedGunners.size());
}

int Ship::getEffectiveTorpedoCount() const {
  if (pilots > 0 && assignedPilots.empty())
    return 0;
  return min(torpedoCount, (int)assignedTorpedoHandlers.size());
}

void Ship::takeDamage(int damage) {
  hp -= damage;
  if (hp < 0)
    hp = 0;
}

void Ship::printInfo() const {
  cout << name << " [" << (faction == ZAPEZOID ? "Zapezoid" : "Rogoatuskan")
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
  Corazzata()
      : Ship("Corazzata", ZAPEZOID, 1031, 2, 10, 4, 164, 293, 10, 4, 50, 25) {}
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
  Kreuzer()
      : Ship("Kreuzer", ROGOATUSKAN, 212, 1, 2, 0, 132, 0, 2, 0, 29, 10) {}
  string getType() const override { return "Kreuzer"; }
};

class Fregatte : public Ship {
public:
  Fregatte()
      : Ship("Fregatte", ROGOATUSKAN, 1143, 2, 11, 5, 159, 282, 11, 5, 60, 30) {
  }
  string getType() const override { return "Fregatte"; }
};

// ==========================================
// BattleEngine.h & BattleEngine.cpp
// ==========================================

class BattleEngine {
private:
  vector<Ship *> zapezoidShips;
  vector<Ship *> rogoatuskanShips;
  int turnCount;

  void cleanupDeadShips(); // Still declaring it even if not implemented, as in
                           // original.
  bool areShipsDefeated(const vector<Ship *> &ships) const;
  Ship *getRandomTarget(const vector<Ship *> &ships);

public:
  BattleEngine();
  ~BattleEngine();

  void init();
  void loadShipsFromCSV(string filename);
  void loadCrewFromCSV(string filename, Faction faction);
  void printShipsReport() const;
  void runBattle();
  void processTurn();
  void validateShips(); // Removes ships with 0 pilots
};

// ==========================================
// Helper Functions
// ==========================================
string trim(const string &str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (string::npos == first)
    return "";
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

BattleEngine::BattleEngine() : turnCount(0) { srand(time(nullptr)); }

BattleEngine::~BattleEngine() {
  for (Ship *s : zapezoidShips)
    delete s;
  for (Ship *s : rogoatuskanShips)
    delete s;
}

void BattleEngine::loadShipsFromCSV(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    exit(1);
  }

  // Check for empty file
  if (file.peek() == ifstream::traits_type::eof()) {
    cerr << "Error: File " << filename << " is empty." << endl;
    exit(1);
  }

  cout << "Loading data files..." << endl;

  string line, id, type, name;
  int lineNum = 0;

  while (getline(file, line)) {
    lineNum++;
    if (trim(line).empty())
      continue; // Skip empty lines

    stringstream ss(line);
    if (getline(ss, id, ',') && getline(ss, type, ',') &&
        getline(ss, name, ',')) {

      // Clean Data
      id = trim(id);
      type = trim(type);
      name = trim(name);

      string lowerType = type;
      transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
                [](unsigned char c) { return tolower(c); });

      Ship *newShip = nullptr;
      Faction f;
      if (lowerType == "guerriero") {
        newShip = new Guerriero();
        f = ZAPEZOID;
      } else if (lowerType == "medio") {
        newShip = new Medio();
        f = ZAPEZOID;
      } else if (lowerType == "corazzata") {
        newShip = new Corazzata();
        f = ZAPEZOID;
      } else if (lowerType == "jager") {
        newShip = new Jager();
        f = ROGOATUSKAN;
      } else if (lowerType == "kreuzer") {
        newShip = new Kreuzer();
        f = ROGOATUSKAN;
      } else if (lowerType == "fregatte") {
        newShip = new Fregatte();
        f = ROGOATUSKAN;
      } else {
        cerr << "Warning: Line " << lineNum << " has unknown ship type '"
             << type << "'. Skipping." << endl;
        continue;
      }

      if (newShip) {
        string fullName = "[" + id + "] " + name;
        newShip->name = fullName;

        if (f == ZAPEZOID)
          zapezoidShips.push_back(newShip);
        else
          rogoatuskanShips.push_back(newShip);
      }
    } else {
      cerr << "Warning: Line " << lineNum << " is malformed (missing columns)."
           << endl;
      if (line.find(';') != string::npos) {
        cerr << "  Hint: Found semicolon ';'. Ensure CSV uses commas ',' as "
                "delimiters."
             << endl;
      }
    }
  }

  cout << "Ships loaded from " << filename << endl;
  cout << "Zapezoids: " << zapezoidShips.size() << " ships." << endl;
  cout << "Rogoatuskans: " << rogoatuskanShips.size() << " ships." << endl;

  if (zapezoidShips.empty() && rogoatuskanShips.empty()) {
    cerr << "Error: No valid ships loaded from " << filename << endl;
    exit(1);
  }
}

void BattleEngine::loadCrewFromCSV(string filename, Faction faction) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << filename << endl;
    exit(1);
  }

  // Check for empty file
  if (file.peek() == ifstream::traits_type::eof()) {
    cerr << "Error: File " << filename << " is empty." << endl;
    exit(1);
  }

  string line, id, name, role;
  int lineNum = 0;

  vector<string> pilotsAvailable, gunnersAvailable, torpedoHandlersAvailable;

  while (getline(file, line)) {
    lineNum++;
    if (trim(line).empty())
      continue;

    stringstream ss(line);
    // Note: Assuming format ID, Name, Role based on earlier reading.
    // Sometimes CSVs vary, ensuring robustness.
    if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, role)) {
      id = trim(id);
      name = trim(name);
      role = trim(role);

      string lowerRole = role;
      transform(lowerRole.begin(), lowerRole.end(), lowerRole.begin(),
                [](unsigned char c) { return tolower(c); });

      if (lowerRole == "pilot" || lowerRole == "pilots")
        pilotsAvailable.push_back(name);
      else if (lowerRole == "gunner" || lowerRole == "gunners")
        gunnersAvailable.push_back(name);
      else if (lowerRole == "torpedohandler" || lowerRole == "torpedohandlers")
        torpedoHandlersAvailable.push_back(name);
      else {
        cerr << "Warning: Line " << lineNum << " has unknown role '" << role
             << "'. Skipping." << endl;
      }
    } else {
      cerr << "Warning: Line " << lineNum << " is malformed (missing columns)."
           << endl;
      if (line.find(';') != string::npos) {
        cerr << "  Hint: Found semicolon ';'. Ensure CSV uses commas ',' as "
                "delimiters."
             << endl;
      }
    }
  }

  // Assign crew ONLY to ships of the specified faction
  vector<Ship *> *targetShips = nullptr;
  if (faction == ZAPEZOID) {
    targetShips = &zapezoidShips;
  } else {
    targetShips = &rogoatuskanShips;
  }

  int pIdx = 0, gIdx = 0, tIdx = 0;

  // Round Robin Assignment for Pilots
  bool assignedAny = true;
  while (assignedAny && pIdx < pilotsAvailable.size()) {
    assignedAny = false;
    for (Ship *ship : *targetShips) {
      if (pIdx >= pilotsAvailable.size())
        break;
      // If ship needs more pilots, assign one
      if (ship->getAssignedPilots().size() < ship->getRequiredPilots()) {
        ship->assignPilot(pilotsAvailable[pIdx]);
        pIdx++;
        assignedAny = true;
      }
    }
  }

  // Round Robin Assignment for Gunners
  assignedAny = true;
  while (assignedAny && gIdx < gunnersAvailable.size()) {
    assignedAny = false;
    for (Ship *ship : *targetShips) {
      if (gIdx >= gunnersAvailable.size())
        break;
      if (ship->getAssignedGunners().size() < ship->getRequiredGunners()) {
        ship->assignGunner(gunnersAvailable[gIdx]);
        gIdx++;
        assignedAny = true;
      }
    }
  }

  // Round Robin Assignment for Torpedo Handlers
  assignedAny = true;
  while (assignedAny && tIdx < torpedoHandlersAvailable.size()) {
    assignedAny = false;
    for (Ship *ship : *targetShips) {
      if (tIdx >= torpedoHandlersAvailable.size())
        break;
      if (ship->getAssignedTorpedoHandlers().size() <
          ship->getRequiredTorpedoHandlers()) {
        ship->assignTorpedoHandler(torpedoHandlersAvailable[tIdx]);
        tIdx++;
        assignedAny = true;
      }
    }
  }
}

void BattleEngine::printShipsReport() const {
  cout << "\n========================================\n"
       << "       SHIPS CONFIGURATION REPORT       \n"
       << "========================================\n"
       << endl;

  auto printCrewList = [](const vector<string> &crew) {
    for (size_t i = 0; i < crew.size(); ++i) {
      cout << crew[i] << (i < crew.size() - 1 ? ", " : "");
    }
  };

  auto printShipReport = [&](const vector<Ship *> &ships) {
    for (Ship *ship : ships) {
      string typeStr = ship->getType();

      cout << ship->getName() << "              (" << typeStr << ")" << endl;
      cout << "      HP: " << ship->getHP() << "/" << ship->getMaxHP() << endl;

      const auto &p = ship->getAssignedPilots();
      if (ship->getRequiredPilots() > 0) {
        cout << "      Pilots (" << p.size() << "/" << ship->getRequiredPilots()
             << "): ";
        printCrewList(p);
        cout << endl;
      }

      const auto &g = ship->getAssignedGunners();
      if (ship->getRequiredGunners() > 0) {
        cout << "      Gunners (" << g.size() << "/"
             << ship->getRequiredGunners() << "): ";
        printCrewList(g);
        cout << endl;
      }

      const auto &th = ship->getAssignedTorpedoHandlers();
      if (ship->getRequiredTorpedoHandlers() > 0) {
        cout << "      Torp Handlers (" << th.size() << "/"
             << ship->getRequiredTorpedoHandlers() << "): ";
        printCrewList(th);
        cout << endl;
      }
      cout << "--------------------------------------------------" << endl;
    }
  };

  cout << "--- ZAPEZOID SHIPS ---" << endl;
  printShipReport(zapezoidShips);

  cout << "--- ROGOATUSKAN SHIPS ---" << endl;
  printShipReport(rogoatuskanShips);
}

void BattleEngine::init() {
  // Add sample ships to Zapezoid Ships
  zapezoidShips.push_back(new Guerriero());
  zapezoidShips.push_back(new Medio());
  zapezoidShips.push_back(new Corazzata());

  // Add sample ships to Rogoatuskan Ships
  rogoatuskanShips.push_back(new Jager());
  rogoatuskanShips.push_back(new Kreuzer());
  rogoatuskanShips.push_back(new Fregatte());

  cout << "Battle Initialized!" << endl;
  cout << "Zapezoids: " << zapezoidShips.size() << " ships." << endl;
  cout << "Rogoatuskans: " << rogoatuskanShips.size() << " ships." << endl;
}

bool BattleEngine::areShipsDefeated(const vector<Ship *> &ships) const {
  for (Ship *s : ships) {
    if (s->isAlive())
      return false;
  }
  return true;
}

Ship *BattleEngine::getRandomTarget(const vector<Ship *> &ships) {
  vector<Ship *> livingShips;
  for (Ship *s : ships) {
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
  for (Ship *shooter : zapezoidShips) {
    if (!shooter->isAlive())
      continue;

    // Light Cannon Attacks
    int activeCannons = shooter->getEffectiveLightCannonCount();
    for (int i = 0; i < activeCannons; ++i) {
      Ship *target = getRandomTarget(rogoatuskanShips);
      if (!target)
        break;

      double hitRoll = (rand() % 10000) / 100.0;
      if (hitRoll < target->getLightCannonHitChance()) {
        int damage = shooter->getLightCannonPower();
        pendingDamage[target] += damage;
        cout << *shooter << " fires Light Cannon " << (i + 1) << " at "
             << *target << " [HIT] (" << damage << " dmg)" << endl;
      } else {
        cout << *shooter << " fires Light Cannon " << (i + 1) << " at "
             << *target << " [MISS]" << endl;
      }
    }

    // Torpedo Attacks
    int activeTorpedoes = shooter->getEffectiveTorpedoCount();
    for (int i = 0; i < activeTorpedoes; ++i) {
      Ship *target = getRandomTarget(rogoatuskanShips);
      if (!target)
        break;

      double hitRoll = (rand() % 10000) / 100.0;
      if (hitRoll < target->getTorpedoHitChance()) {
        int damage = shooter->getTorpedoPower();
        pendingDamage[target] += damage;
        cout << *shooter << " fires Torpedo " << (i + 1) << " at " << *target
             << " [HIT] (" << damage << " dmg)" << endl;
      } else {
        cout << *shooter << " fires Torpedo " << (i + 1) << " at " << *target
             << " [MISS]" << endl;
      }
    }
  }

  // Rogoatuskans fire
  for (Ship *shooter : rogoatuskanShips) {
    if (!shooter->isAlive())
      continue;

    // Light Cannon Attacks
    int activeCannons = shooter->getEffectiveLightCannonCount();
    for (int i = 0; i < activeCannons; ++i) {
      Ship *target = getRandomTarget(zapezoidShips);
      if (!target)
        break;

      double hitRoll = (rand() % 10000) / 100.0;
      if (hitRoll < target->getLightCannonHitChance()) {
        int damage = shooter->getLightCannonPower();
        pendingDamage[target] += damage;
        cout << *shooter << " fires Light Cannon " << (i + 1) << " at "
             << *target << " [HIT] (" << damage << " dmg)" << endl;
      } else {
        cout << *shooter << " fires Light Cannon " << (i + 1) << " at "
             << *target << " [MISS]" << endl;
      }
    }

    // Torpedo Attacks
    int activeTorpedoes = shooter->getEffectiveTorpedoCount();
    for (int i = 0; i < activeTorpedoes; ++i) {
      Ship *target = getRandomTarget(zapezoidShips);
      if (!target)
        break;

      double hitRoll = (rand() % 10000) / 100.0;
      if (hitRoll < target->getTorpedoHitChance()) {
        int damage = shooter->getTorpedoPower();
        pendingDamage[target] += damage;
        cout << *shooter << " fires Torpedo " << (i + 1) << " at " << *target
             << " [HIT] (" << damage << " dmg)" << endl;
      } else {
        cout << *shooter << " fires Torpedo " << (i + 1) << " at " << *target
             << " [MISS]" << endl;
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
  while (!areShipsDefeated(zapezoidShips) &&
         !areShipsDefeated(rogoatuskanShips)) {
    if (turnCount >= 50) {
      cout << "\n--- BATTLE STOPPED (Stalemate / Max Turns Reached) ---"
           << endl;
      break;
    }
    processTurn();
  }

  cout << "\n--- Battle Over ---" << endl;
  if (areShipsDefeated(zapezoidShips)) {
    cout << "Rogoatuskans Win!" << endl;
  } else {
    cout << "Zapezoids Win!" << endl;
  }
}

// ==========================================
// Main Function
// ==========================================

void BattleEngine::validateShips() {
  auto validateShipList = [](vector<Ship *> &ships, string factionName) {
    auto it = remove_if(ships.begin(), ships.end(), [&](Ship *s) {
      if (s->getAssignedPilots().empty()) {
        cout << "[Warning] Ship " << s->getName() << " (" << factionName
             << ") removed: No Pilot Assigned." << endl;
        delete s; // Clean up memory
        return true;
      }
      return false;
    });
    ships.erase(it, ships.end());
  };

  validateShipList(zapezoidShips, "Zapezoid");
  validateShipList(rogoatuskanShips, "Rogoatuskan");
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0]
              << " <zapezoid_ships.csv> <zapezoid_crew.csv> "
                 "<rogoatuskan_ships.csv> <rogoatuskan_crew.csv>"
              << std::endl;
    return 1;
  }

  BattleEngine engine;

  // Load Ship Files (Arg 1 and 3)
  engine.loadShipsFromCSV(argv[1]);
  engine.loadShipsFromCSV(argv[3]);

  // Load Crew Files (Arg 2 and 4)
  engine.loadCrewFromCSV(argv[2], ZAPEZOID);
  engine.loadCrewFromCSV(argv[4], ROGOATUSKAN);

  engine.validateShips(); // Remove ships that cannot fly

  engine.printShipsReport();

  cout << "========================================" << endl;
  cout << "           BATTLE COMMENCING            " << endl;
  cout << "========================================" << endl;

  engine.runBattle();
  return 0;
}
