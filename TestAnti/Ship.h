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

  // Assigned Crew
  int assignedPilots;
  int assignedGunners;
  int assignedTorpedoHandlers;

public:
  Ship(string n, Faction f, int h, int p, int g, int th, int lc_pow, int t_pow,
       int lc_count, int t_count);
  virtual ~Ship() {}

  // Getters
  string getName() const { return name; }
  Faction getFaction() const { return faction; }
  int getHP() const { return hp; }
  bool isAlive() const { return hp > 0; }

  // Actions
  virtual int calculateTotalFirePower() const;
  void takeDamage(int damage);
  void assignPilot() { assignedPilots++; }
  void assignGunner() { assignedGunners++; }
  void assignTorpedoHandler() { assignedTorpedoHandlers++; }

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
