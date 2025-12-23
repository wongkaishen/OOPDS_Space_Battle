#ifndef ship_h
#define ship_h
#include <iostream>
using namespace std;

class ship
{
};

class Zapezoids : public ship
{
};

class Rogatuskan : public ship
{
private:
    float hitPoints;
    int pilot;
    int gunner;
    int lightCannons;
    int chanceToBeHitByLightCannons;
    int chanceToBeHitByTorpedoes;
    int torpedoHandlers;
    int torpedoes;
};

class Jager : public Rogatuskan
{
private:
    float hitPoints;
    int pilot;
    int lightCannons;
    int chanceToBeHitByLightCannons;
    int chanceToBeHitByTorpedoes;
};

class Kreuzer : public Rogatuskan
{
private:
    float hitPoints;
    int pilot;
    int gunner;
    int lightCannons;
    int chanceToBeHitByLightCannons;
    int chanceToBeHitByTorpedoes;
};

class Fregatte : public Rogatuskan
{
private:
    float hitPoints;
    int pilot;
    int gunner;
    int lightCannons;
    int chanceToBeHitByLightCannons;
    int chanceToBeHitByTorpedoes;
    int torpedoHandlers;
    int torpedoes;
};
#endif