#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

  friend class BattleEngine; // To allow setting name from CSV logic
  
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

#endif
