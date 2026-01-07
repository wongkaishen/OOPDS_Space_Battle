#ifndef ROGOATUSKAN_SHIPS_H
#define ROGOATUSKAN_SHIPS_H

#include "Ship.h"

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

#endif
