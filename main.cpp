#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Ship
{
    protected:

    string name;
    string type_ship_name;

    int hitPoints;
    int Maxhitpoints;

    int pilotCount;
    int gunnerCount;
    int torpedoHandlersCount;

    int lightCannonscount;
    int torpedoesCount;

    float chanceHitByLightCannon;
    float chanceHitByLightTorpedo;

    int lightCannonPower;
    int torpedoPower;

    public:

    Ship(string n , string tsn , int hp , float cHitLightCanon, float cHitLightTorpedo)
    {
        name=n;
        type_ship_name=tsn;
        hitPoints=hp;
        chanceHitByLightCannon=cHitLightCanon;
        chanceHitByLightTorpedo=cHitLightTorpedo;
        string type_ship_name=tsn;
    }
    
    string getName()
    {
        return name;
    }

    void takeDamage(int damage)
    {
        hitPoints -= damage;
        if(hitPoints <= 0)
        {
            hitPoints=0;
        }

    bool isDestroyed()
    {
        return hitPoints<=0;
    }
};

class Zapezoids : public Ship
{
    public:
    Zapezoids(string n ,string tsn , int hp , float cHitLightCanon, float cHitLightTorpedo) : Ship(n , tsn , hp , cHitLightCanon , cHitLightTorpedo){}

};

class Guerriero : public Zapezoids
{

    public:
    Guerriero(string n) : Zapezoids(n , "Guerriero" , 123 , 0.26 , 0.06){}

};

class Medio  : public Zapezoids
{
   public:
   Medio(string n) : Zapezoids(n , "Medio" , 214 , 0.31 , 0.11){}

};

class Corazzata : public Zapezoids
{
    public:
    Corazzata(string n) : Zapezoids(n , "Corazzata" , 1031 , 0.31 , 0.11){} 

};

