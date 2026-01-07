#include "Ship.h"

Ship::Ship(string n, Faction f, int h, int p, int g, int th, int lc_pow,
           int t_pow, int lc_count, int t_count)
    : name(n), faction(f), maxHp(h), hp(h), pilots(p), gunners(g),
      torpedoHandlers(th), lightCannons(lc_pow), torpedoes(t_pow),
      lightCannonCount(lc_count), torpedoCount(t_count), assignedPilots(0),
      assignedGunners(0), assignedTorpedoHandlers(0) {}

int Ship::calculateTotalFirePower() const {
  if (pilots > 0 && assignedPilots == 0)
    return 0; // No power if no pilot

  double cannonEfficiency = 1.0;
  if (gunners > 0) {
    cannonEfficiency = (double)assignedGunners / gunners;
    if (cannonEfficiency > 1.0)
      cannonEfficiency = 1.0;
  }

  double torpedoEfficiency = 1.0;
  if (torpedoHandlers > 0) {
    torpedoEfficiency = (double)assignedTorpedoHandlers / torpedoHandlers;
    if (torpedoEfficiency > 1.0)
      torpedoEfficiency = 1.0;
  }

  int totalPower = 0;

  // Calculate Cannon Power
  totalPower += (lightCannons * lightCannonCount) * cannonEfficiency;
  totalPower += (torpedoes * torpedoCount) * torpedoEfficiency;

  return totalPower;
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
