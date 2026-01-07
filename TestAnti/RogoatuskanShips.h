#ifndef ROGOATUSKAN_SHIPS_H
#define ROGOATUSKAN_SHIPS_H

#include "Ship.h"

class Jager : public Ship {
public:
  Jager() : Ship("Jager", ROGOATUSKAN, 112, 1, 0, 0, 101, 0, 1, 0) {}
};

class Kreuzer : public Ship {
public:
  Kreuzer() : Ship("Kreuzer", ROGOATUSKAN, 212, 1, 2, 0, 132, 0, 2, 0) {}
};

class Fregatte : public Ship {
public:
  Fregatte() : Ship("Fregatte", ROGOATUSKAN, 1143, 2, 11, 5, 159, 282, 11, 5) {}
};

#endif
