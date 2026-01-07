#ifndef ZAPEZOID_SHIPS_H
#define ZAPEZOID_SHIPS_H

#include "Ship.h"

class Guerriero : public Ship {
public:
  Guerriero() : Ship("Guerriero", ZAPEZOID, 123, 1, 1, 0, 96, 0, 1, 0) {}
  string getType() const override { return "Guerriero"; }
};

class Medio : public Ship {
public:
  Medio() : Ship("Medio", ZAPEZOID, 214, 1, 2, 0, 134, 0, 2, 0) {}
  string getType() const override { return "Medio"; }
};

class Corazzata : public Ship {
public:
  Corazzata() : Ship("Corazzata", ZAPEZOID, 1031, 2, 10, 4, 164, 293, 10, 4) {}
  string getType() const override { return "Corazzata"; }
};

#endif
