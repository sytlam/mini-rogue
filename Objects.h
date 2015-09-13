//
//  Objects.h
//  Project 3
//
//  Created by Stephanie Lam on 5/15/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
using namespace std;

class Actors;

class Objects
{
public:
    Objects(int r, int c, string n, Actors* p);
    virtual ~Objects() {}
    int getRow() const;
    int getCol() const;
    Actors* getUser() const;
    virtual string getName() const;
    virtual char getSymbol() const = 0; 
private:
    int row, col;
    string name;
    Actors* user;
};

class Weapons : public Objects
{
public:
    Weapons(int r, int c, string n, Actors* p);
    virtual ~Weapons() {}
    string getAction(Actors* d, bool hit) const;
    void setUpWeapon();
    int getDexBonus() const;
    int getDamage() const;
    bool hit(Actors* a, Actors* d);
    int calculateDamage(Actors* a, Actors* d);
    virtual char getSymbol() const;
private:
    string action;
    int dexBonus;
    int damage;
    bool sleep; 
};

class Scroll : public Objects
{
public:
    Scroll(int r, int c, string n, Actors* p);
    virtual ~Scroll() {}
    virtual char getSymbol() const;
    virtual string getName() const;
    string getReadMessage() const;
    void applyEffect(); 
private:
};

#endif // OBJECTS_H
