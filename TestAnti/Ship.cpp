#include "Ship.h"

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
