//
//  Actors.h
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#ifndef ACTORS_H
#define ACTORS_H

#include "Dungeon.h"
#include <string>
#include <vector>
class Weapons;

class Actors
{
public:
    Actors(Dungeon* d, int r, int c, int health, int arm, int str, int dext);
    virtual ~Actors() {}
    int getRow() const;
    int getCol() const;
    int getHP() const;
    int getArmor() const;
    int getStrength() const;
    int getDex() const;
    int getSleep() const;
    Dungeon* getDungeon() const;
    Weapons* getWeapon() const;
    int getMaxHP() const;
    
    void setRow(int r);
    void setCol(int c);
    void setHP(int health);
    void setArmor(int arm);
    void setStrength(int str);
    void setDex(int dext);
    void setSleep(int slp);
    void setDungeon(Dungeon* d);
    void setWeapon(Weapons* w);
    void setMaxHP(int h);
    
    virtual char getPiece() const = 0;
    virtual bool movePiece(char action, const char(&dungeon)[18][70]) = 0;
    virtual string getName() const = 0;
    bool isDead() const; 
    virtual bool attack(Actors* a, Actors* d);
    virtual bool drop();
    virtual string getDrop() const;
private:
    Dungeon* field;
    Weapons* current;
    int row, col;
    int hp;
    int armor;
    int strength;
    int dex;
    int sleep;
    int maxHP;
};

class Player : public Actors
{
public:
    Player(Dungeon* d, int r, int c);
    virtual ~Player() {}
    vector<Objects*> getInventory() const;
    void addToInventory(Objects* o);
    void removeFromInventory(int index);
    void displayInventory();
    void wieldWeapon(char choice);
    void readScroll(char choice);
    
    virtual char getPiece() const;
    virtual bool movePiece(char action, const char(&dungeon)[18][70]);
    virtual string getName() const;
private:
    vector<Objects*> inventory;
};

class Snakewomen : public Actors
{
public:
    Snakewomen(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p);
    virtual ~Snakewomen() {}
    bool smell();
    virtual bool movePiece(char action, const char(&dungeon)[18][70]);
    virtual char getPiece() const;
    virtual bool drop();
    virtual string getName() const;
    virtual string getDrop() const;
private:
    Player* player;
};

class Bogeyman : public Actors
{
public:
    Bogeyman(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p);
    virtual ~Bogeyman() {}
    bool smell();
    virtual bool movePiece(char action, const char(&dungeon)[18][70]);
    virtual char getPiece() const;
    virtual bool drop();
    virtual string getName() const;
    virtual string getDrop() const;
private:
    Player* player;
};

class Dragon : public Actors
{
public:
    Dragon(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p);
    virtual ~Dragon() {}
    virtual bool movePiece(char action, const char(&dungeon)[18][70]);
    virtual char getPiece() const;
    virtual string getName() const;
    virtual bool drop();
    virtual string getDrop() const;
private:
    Player* player;
};

class Goblin : public Actors
{
public:
    Goblin(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p, int dist);
    virtual ~Goblin() {} 
    bool smell(const char(&dungeon)[18][70]);
    bool findPath(int row, int col, const char (&dungeon)[18][70]);
    virtual bool movePiece(char action, const char(&dungeon)[18][70]);
    virtual char getPiece() const;
    virtual bool drop();
    virtual string getName() const;
    virtual string getDrop() const;
private:
    Player* player;
    int smellDist;
    vector<int> pathRows;
    vector<int> pathCols; 
};

#endif // ACTORS_H
