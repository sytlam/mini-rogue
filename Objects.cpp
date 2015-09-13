//
//  Objects.cpp
//  Project 3
//
//  Created by Stephanie Lam on 5/15/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#include "Objects.h"
#include "Actors.h"
#include "utilities.h"
#include <string>
#include <iostream>
using namespace std;

Objects::Objects(int r, int c, string n, Actors* p)
{
    row = r;
    col = c;
    name = n;
    user = p;
}

int Objects::getRow() const
{
    return row;
}

int Objects::getCol() const
{
    return col;
}

string Objects::getName() const
{
    return name;
}

Actors* Objects::getUser() const
{
    return user;
}

Weapons::Weapons(int r, int c, string n, Actors* p)
: Objects(r, c, n, p)
{
    setUpWeapon();
}

string Weapons::getAction(Actors* d, bool hit) const // Creates the action string that is displayed when an Actor attacks another Actor
{
    string action = getUser()->getName();
    if (getName() == "mace")
    {
        action += "swings mace at ";
    }
    else if (getName() == "short sword")
    {
        action += "slashes short sword at ";
    }
    else if (getName() == "long sword")
    {
        action += "swings long sword at ";
    }
    else if (getName() == "magic axe")
    {
        action += "chops magic axe at ";
    }
    else if (getName() == "magic fangs of sleep")
    {
        action += "strikes magic fangs at ";
    }
    action += d->getName();
    if (d->getHP() <= 0) // the hit killed the actor
        action += "dealing a final blow.";
    else if (sleep) // put the actor to sleep
    {
        action += "putting ";
        action += d->getName();
        action += "to sleep.";
    }
    else
    {
        if (hit)
            action += "and hits.";
        else
            action += "and misses.";
    }
    return action;
}

char Weapons::getSymbol() const
{
    return ')';
}

void Weapons::setUpWeapon() // Set up the stats for each of the weapons
{
    if (getName() == "mace")
    {
        dexBonus = 0;
        damage = 2;
    }
    else if (getName() == "short sword")
    {
        dexBonus = 0;
        damage = 2;
    }
    else if (getName() == "long sword")
    {
        dexBonus = 2;
        damage = 4;
    }
    else if (getName() == "magic axe")
    {
        dexBonus = 5;
        damage = 5;
    }
    else if (getName() == "magic fangs of sleep")
    {
        dexBonus = 3;
        damage = 2;
    }
}

int Weapons::getDexBonus() const
{
    return dexBonus;
}

int Weapons::getDamage() const
{
    return damage;
}

bool Weapons::hit(Actors* a, Actors* d) // Determines whether or not Actor a successfully hit Actor d
{
    sleep = false;
    int attackerPoints = a->getDex() + dexBonus;
    int defenderPoints = d->getDex() + d->getArmor();
    if (randInt(attackerPoints) >= randInt(defenderPoints))
        return true;
    return false;
}

int Weapons::calculateDamage(Actors* a, Actors* d) // Calculates the damage that a hit from Actor a does to Actor d
{
    if (a->getWeapon()->getName() == "magic fangs of sleep") // attacking with the magic fangs of sleep
    {
        int random = randInt(5);
        if (random == 0) // put the player to sleep
        {
            int numSleep = randInt(5) + 2; // sleep counter set to a # in range from 2 to 6
            if (d->getSleep() > 0) // set sleep counter to maximum of 2 sleep values
            {
                if (numSleep > d->getSleep())
                    d->setSleep(numSleep);
            }
            else
            {
                d->setSleep(numSleep); 
            }
            sleep = true;
        }
    }
    return randInt(a->getStrength() + damage); // return the damage amount
}

Scroll::Scroll(int r, int c, string n, Actors* p)
: Objects(r, c, n, p)
{}

char Scroll::getSymbol() const
{
    return '?'; 
}

string Scroll::getName() const
{
    return "scroll of " + Objects::getName();
}

string Scroll::getReadMessage() const // Create the message displayed after reading a scroll
{
    string n = Objects::getName();
    string msg = "";
    if (n == "teleportation")
        msg = "You feel your body wrenched in space and time.";
    else if (n == "strength")
        msg = "Your muscles bulge.";
    else if (n == "enhance armor")
        msg = "Your armor glows blue.";
    else if (n == "enhance health")
        msg = "You feel your heart beating stronger.";
    else if (n == "enhance dexterity")
        msg = "You feel like less of a klutz.";
    return msg;
}

void Scroll::applyEffect() // Applies the effect of reading the scroll on the player
{
    string n = Objects::getName();
    if (n == "teleportation")
    {
        getUser()->getDungeon()->moveToRandom(); // move the player to a random blank space on the grid
    }
    else if (n == "strength")
    {
        int random = randInt(3) + 1; // range from 1 to 3
        getUser()->setStrength(getUser()->getStrength() + random); // Increment user's strength by random #
    }
    else if (n == "enhance armor")
    {
        int random = randInt(3) + 1; // range from 1 to 3
        getUser()->setArmor(getUser()->getArmor() + random); // Increment user's armor by random #
    }
    else if (n == "enhance health")
    {
        int random = randInt(6) + 3; // range from 3 to 8
        getUser()->setMaxHP(getUser()->getMaxHP() + random); // Increment user's max HP by random #
    }
    else if (n == "enhance dexterity")
    {
        getUser()->setDex(getUser()->getDex() + 1); // Increment user's dex by 1 
    }
}
