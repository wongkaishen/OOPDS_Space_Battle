#ifndef ship_h
#define ship_h
#include <iostream>
#include <string>
using namespace std;

class Ship
{
protected:
    // check the ship name
    string name;

    // hit points
    int hitPoints;    // current hp
    int maxHitPoints; // max hp(not change during the game)

    // crew members
    int pilotCount;
    int gunnerCount;
    int torpedoHandlersCount;

    // weapons
    int lightCannons;
    int torpedoes;

    // probability to be hit
    int chanceHitByCannon;
    int chanceHitByTorpedo;

public:
    string getName()
    {
        return name;
    }

    // action function
    void takeDamage(int damage)
    {
        hitPoints -= damage;
        if (hitPoints < 0)
        {
            hitPoints = 0;
        }
    }

    // check if the ship is destroyed
    bool isDestroyed()
    {
        return hitPoints <= 0;
    }
};

class Rogatuskan : public Ship
{
};

class Jager : public Rogatuskan
{
};

class Kreuzer : public Rogatuskan
{
};

class Fregatte : public Rogatuskan
{
};
#endif
