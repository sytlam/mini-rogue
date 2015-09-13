//
//  Actors.cpp
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#include "Actors.h"
#include "utilities.h"
#include "Objects.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Actors::Actors(Dungeon* d, int r, int c, int health, int arm, int str, int dext)
{
    field = d;
    row = r;
    col = c;
    hp = health;
    armor = arm;
    strength = str;
    dex = dext;
    sleep = 0;
    current = nullptr; 
}

int Actors::getRow() const
{
    return row;
}

int Actors::getCol() const
{
    return col;
}

int Actors::getHP() const
{
    return hp;
}

int Actors::getArmor() const
{
    return armor;
}

int Actors::getStrength() const
{
    return strength;
}

int Actors::getDex() const
{
    return dex;
}

int Actors::getSleep() const
{
    return sleep;
}

Dungeon* Actors::getDungeon() const
{
    return field;
}

Weapons* Actors::getWeapon() const
{
    return current;
}

int Actors::getMaxHP() const
{
    return maxHP;
}

bool Actors::isDead() const
{
    if (getHP() <= 0)
        return true;
    return false;
}

void Actors::setRow(int r)
{
    row = r;
}

void Actors::setCol(int c)
{
    col = c;
}

void Actors::setHP(int health)
{
    if (health > 99)
        hp = 99;
    else
        hp = health;
}

void Actors::setArmor(int arm)
{
    if (arm > 99)
        armor = 99;
    else
        armor = arm;
}

void Actors::setStrength(int str)
{
    if (str > 99)
        strength = 99;
    else
        strength = str;
}

void Actors::setDex(int dext)
{
    if (dex > 99)
        dex = 99;
    else
        dex = dext;
}

void Actors::setSleep(int slp)
{
    if (sleep > 9)
        sleep = 9;
    else
        sleep = slp;
}

void Actors::setDungeon(Dungeon* d)
{
    field = d;
}

void Actors::setWeapon(Weapons* w)
{
    current = w;
}

void Actors::setMaxHP(int h)
{
    maxHP = h;
}

bool Actors::attack(Actors* a, Actors* d) // attacks Actor d
{
    if (current->hit(a, d)) // if the attacker successfully hit the defender
    {
        int damage = current->calculateDamage(a, d); // calculate how much damage the attacker did
        d->setHP(d->getHP() - damage); // subtract damage from the defender's HP
        getDungeon()->addAttackMessages((current->getAction(d, true))); // add attack to the attack message log
        return true;
    }
    else // the attacker missed the defender
        getDungeon()->addAttackMessages((current->getAction(d, false)));
        return false; 
}

bool Actors::drop() // Actors don't drop anything
{
    return false;
}

string Actors::getDrop() const
{
    return ""; 
}

Player::Player(Dungeon* d, int r, int c)
: Actors(d, r, c, 20, 2, 2, 2)
{
    Weapons* w = new Weapons(r, c, "short sword", this);
    addToInventory(w);
    setWeapon(w);
    setMaxHP(20);
}

char Player::getPiece() const
{
    return '@'; 
}

string Player::getName() const
{
    return "Player ";
}

bool Player::movePiece(char action, const char(&dungeon)[18][70]) // moves the player in appropriate direction
{
    if (action == ARROW_LEFT && getDungeon()->checkMove(getRow(), getCol()-1))
    {
        setCol(getCol()-1);
        return true;
    }
    else if (action == ARROW_RIGHT && getDungeon()->checkMove(getRow(), getCol()+1))
    {
        setCol(getCol()+1);
        return true; 
    }
    else if (action == ARROW_UP && getDungeon()->checkMove(getRow()-1, getCol()))
    {
        setRow(getRow()-1);
        return true;
    }
    else if (action == ARROW_DOWN && getDungeon()->checkMove(getRow()+1, getCol()))
    {
        setRow(getRow()+1);
        return true;
    }
    return false;
}

vector<Objects*> Player::getInventory() const
{
    return inventory;
}

void Player::addToInventory(Objects* o)
{
    inventory.push_back(o);
}

void Player::removeFromInventory(int index)
{
    inventory.erase(inventory.begin() + index);
}

void Player::displayInventory()
{
    char letter = 'a';
    cout << "Inventory: " << endl;
    for (vector<Objects*>::iterator i = inventory.begin(); i != inventory.end(); i++)
    {
        cout << " " << letter << ". " << (*i)->getName() << endl;
        letter++; 
    }
}

void Player::wieldWeapon(char choice) // Changes the weapon that the player is wielding
{
    clearScreen();
    getDungeon()->displayDungeon();
    getDungeon()->displayStatus();
    char letter = 'a';
    for (vector<Objects*>::iterator i = inventory.begin(); i != inventory.end(); i++)
    {
        if (letter == choice)
        {
            Weapons* temp = dynamic_cast<Weapons*>(*i); // see if the object of choice is a weapon
            if (temp != nullptr) // chosen object is a weapon
            {
                cout << "You are wielding " << temp->getName() << endl;
                setWeapon(temp);
            }
            else // chosen object isn't a weapon
            {
                cout << "You can't wield " << (*i)->getName() << endl;
            }
        }
        letter++;
    }
}

void Player::readScroll(char choice) // Allows player to read a scroll
{
    char letter = 'a';
    int count = 0; // to keep track of what position to erase, if any
    for (vector<Objects*>::iterator i = inventory.begin(); i != inventory.end(); i++)
    {
        if (letter == choice)
        {
            Scroll* temp = dynamic_cast<Scroll*>(*i); // see if the object of choice is a scroll
            if (temp != nullptr) // chosen object is a scroll
            {
                temp->applyEffect();
                clearScreen();
                getDungeon()->displayDungeon();
                getDungeon()->displayStatus();
                cout << "You read the scroll called " << temp->getName() << endl;
                cout << temp->getReadMessage() << endl;
                inventory.erase(inventory.begin() + count);
                return;
            }
            else
            {
                clearScreen();
                getDungeon()->displayDungeon();
                getDungeon()->displayStatus();
                cout << "You can't read a " << (*i)->getName() << endl;
                return;
            }
        }
        letter++;
        count++;
    }
    
    // to make sure that the dungeon is redisplayed if the user chose an invalid option
    clearScreen();
    getDungeon()->displayDungeon();
    getDungeon()->displayStatus();
}

Snakewomen::Snakewomen(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p)
: Actors(d, r, c, health, arm, str, dext)
{
    setWeapon(new Weapons(r, c, "magic fangs of sleep", this));
    player = p;
}

string Snakewomen::getName() const
{
    return "the Snakewoman ";
}

bool Snakewomen::smell() // Determine if the player is within the range that a Snakewoman can smell
{
    if ((abs(getRow() - player->getRow()) + abs(getCol() - player->getCol()) <= 3)) // Snakewoman within range of smelling the player
        return true;
    return false;
}

bool Snakewomen::movePiece(char action, const char(&dungeon)[18][70]) // move the Snakewoman
{
    if (smell() && !isDead() && getSleep() == 0)
    {
        bool moved = false;
        
        // Move the Snakewoman closer to the player's row/column and attack if the Snakewoman is next to the player
        if (!moved && getCol() - player->getCol() < 0)
        {
            if (getRow() == player->getRow() && getCol()+1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()+1))
            {
                setCol(getCol()+1);
                moved = true;
            }
        }
        if (!moved && getCol() - player->getCol() > 0)
        {
            if (getRow() == player->getRow() && getCol()-1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()-1))
            {
                setCol(getCol()-1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() < 0)
        {
            if (getRow()+1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()+1, getCol()))
            {
                setRow(getRow()+1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() > 0)
        {
            if (getRow()-1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()-1, getCol()))
            {
                setRow(getRow()-1);
                moved = true;
            }
        }
        return true;
    }
    else if (getSleep() > 0)
        setSleep(getSleep()-1); 
    return false;
}

char Snakewomen::getPiece() const
{
    return 'S';
}

bool Snakewomen::drop() // Determine whether or not the Snakewoman drops an item after it dies
{
    int random = randInt(3);
    if (random == 0) // 1 in 3 chance of dropping an item
        return true;
    return false;
}

string Snakewomen::getDrop() const
{
    return "magic fangs of sleep"; 
}

Bogeyman::Bogeyman(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p)
: Actors(d, r, c, health, arm, str, dext)
{
    setWeapon(new Weapons(r, c, "short sword", this));
    player = p;
}

bool Bogeyman::smell() // Determines if the player is within range of the Bogeyman smelling it
{
    if ((abs(getRow() - player->getRow()) + abs(getCol() - player->getCol()) <= 5)) // Bogeyman within range of smelling the player
        return true;
    return false;
}

bool Bogeyman::movePiece(char action, const char(&dungeon)[18][70]) // moves the Bogeyman
{
    if (smell() && !isDead() && getSleep() == 0)
    {
        bool moved = false;
        
        // Moves Bogeyman so that it gets closer to the player's row/column and attacks if the player is next to it
        if (!moved && getCol() - player->getCol() < 0)
        {
            if (getRow() == player->getRow() && getCol()+1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()+1))
            {
                setCol(getCol()+1);
                moved = true;
            }
        }
        if (!moved && getCol() - player->getCol() > 0)
        {
            if (getRow() == player->getRow() && getCol()-1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()-1))
            {
                setCol(getCol()-1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() < 0)
        {
            if (getRow()+1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()+1, getCol()))
            {
                setRow(getRow()+1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() > 0)
        {
            if (getRow()-1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()-1, getCol()))
            {
                setRow(getRow()-1);
                moved = true;
            }
        }
        return true;
    }
    else if (getSleep() > 0) // Bogeyman is asleep so decrement sleep counter
        setSleep(getSleep()-1);
    return false;
}

char Bogeyman::getPiece() const
{
    return 'B';
}

bool Bogeyman::drop() // determine if Bogeyman is supposed to drop an item
{
    int random = randInt(10);
    if (random == 0) // 1 in 10 chance of dropping an item
        return true;
    return false;
}

string Bogeyman::getDrop() const
{
    return "magic axe";
}

string Bogeyman::getName() const
{
    return "the Bogeyman ";
}

Dragon::Dragon(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p)
: Actors(d, r, c, health, arm, str, dext)
{
    setWeapon(new Weapons(r, c, "long sword", this));
    player = p;
}

bool Dragon::movePiece(char action, const char(&dungeon)[18][70]) // Attacks the player if the player is next to a Dragon
{
    int rand = randInt(10); // random integer from 0 to 9
    if (rand == 0) // 1 in 10 chance of regaining HP 
    {
        if (getHP() < getMaxHP())
            setHP(getHP() + 1);
    }
    if (!isDead() && getSleep() == 0) // Attack player if the player is next to the Dragon
    {
        bool moved = false;
        if (!moved && getCol() - player->getCol() < 0)
        {
            if (getRow() == player->getRow() && getCol()+1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
        }
        if (!moved && getCol() - player->getCol() > 0)
        {
            if (getRow() == player->getRow() && getCol()-1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() < 0)
        {
            if (getRow()+1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() > 0)
        {
            if (getRow()-1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
        }
        return true;
    }
    else if (getSleep() > 0) // Dragon is asleep so decrement sleep counter
        setSleep(getSleep()-1);
    return false;
}

char Dragon::getPiece() const
{
    return 'D';
}

bool Dragon::drop()
{
    return true; // 100% chance of dropping an item
}

string Dragon::getDrop() const // drops 1 scroll randomly
{
    int rand = randInt(5);
    if (rand == 0)
        return "teleportation";
    else if (rand == 1)
        return "enhance armor";
    else if (rand == 2)
        return "strength";
    else if (rand == 3)
        return "enhance health";
    else
        return "enhance dexterity";
}

string Dragon::getName() const
{
    return "the Dragon ";
}

Goblin::Goblin(Dungeon* d, int r, int c, int health, int arm, int str, int dext, Player* p, int dist)
: Actors(d, r, c, health, arm, str, dext)
{
    setWeapon(new Weapons(r, c, "short sword", this));
    player = p;
    smellDist = dist; 
}

bool Goblin::smell(const char(&dungeon)[18][70]) // Determine whether Goblin is within range of smelling player
{
    pathRows.clear();
    pathCols.clear();
    findPath(getRow(), getCol(), dungeon);
    
    if (pathRows.size() > 0 && pathRows.size() <= smellDist)
        return true;
    return false;
}

bool Goblin::findPath(int row, int col, const char (&dungeon)[18][70]) // Find the optimal path for the Goblin to move
{
    // Make a copy of the current dungeon
    char copy[18][70];
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            copy[i][j] = dungeon[i][j];
        }
    }
    vector<Actors*> monsters = getDungeon()->getMonsters();
    for (vector<Actors*>::iterator i = monsters.begin(); i != monsters.end(); i++)
    {
        copy[(*i)->getRow()][(*i)->getCol()] = (*i)->getPiece();
    }
    copy[player->getRow()][player->getCol()] = player->getPiece();
    
    if (copy[row][col] == '@')
        return true;
    copy[row][col] = 'z'; // mark current spot as visited
    
    if (col - player->getCol() < 0 && getDungeon()->checkMove(row, col+1) && copy[row][col+1] != 'z') // Move east towards the player
    {
        if (findPath(row, col+1, copy)) // Found a path to the player
        {
            pathRows.push_back(row);
            pathCols.push_back(col+1);
            return true;
        }
    }
    if (row - player->getRow() < 0 && getDungeon()->checkMove(row+1, col) && copy[row+1][col] != 'z') // move south towards the player
    {
        if (findPath(row+1, col, copy)) // Found a path to the player
        {
            pathRows.push_back(row+1);
            pathCols.push_back(col);
            return true;
        }
    }
    if (col - player->getCol() > 0 && getDungeon()->checkMove(row, col-1) && copy[row][col-1] != 'z') // move west towards the player
    {
        if (findPath(row, col-1, copy)) // Found a path to the player
        {
            pathRows.push_back(row);
            pathCols.push_back(col-1);
            return true;
        }
    }
    if (row - player->getRow() > 0 && getDungeon()->checkMove(row-1, col) && copy[row-1][col] != 'z') // move north towards the player
    {
        if (findPath(row-1, col, copy)) // Found a path to the player
        {
            pathRows.push_back(row-1);
            pathCols.push_back(col);
            return true;
        }
    }
    return false;
}

bool Goblin::movePiece(char action, const char(&dungeon)[18][70]) // Moves the Goblin closer toward the player
{
    if (smell(dungeon) && !isDead() && getSleep() == 0)
    {
        bool moved = false;
        // Move the Goblin closer to the player's row and column and attack the player if next to it
        // BUG: Goblin moves like the Snakewoman/Bogeyman, not by finding the optimal path
        if (!moved && getCol() - player->getCol() < 0)
        {
            if (getRow() == player->getRow() && getCol()+1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()+1))
            {
                setCol(getCol()+1);
                moved = true;
            }
        }
        if (!moved && getCol() - player->getCol() > 0)
        {
            if (getRow() == player->getRow() && getCol()-1 == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow(), getCol()-1))
            {
                setCol(getCol()-1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() < 0)
        {
            if (getRow()+1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()+1, getCol()))
            {
                setRow(getRow()+1);
                moved = true;
            }
        }
        if (!moved && getRow() - player->getRow() > 0)
        {
            if (getRow()-1 == player->getRow() && getCol() == player->getCol())
            {
                attack(this, player);
                moved = true;
            }
            else if (getDungeon()->checkMove(getRow()-1, getCol()))
            {
                setRow(getRow()-1);
                moved = true;
            }
        }
        return true;
    }
    else if (getSleep() > 0) // Goblin is asleep so decrement sleep counter by 1
        setSleep(getSleep()-1);
    return false;
}

char Goblin::getPiece() const
{
    return 'G';
}

bool Goblin::drop() // Determine is Goblin drops item when it dies
{
    int random = randInt(3);
    if (random == 0) // 1 in 3 chance of dropping an item
        return true;
    return false;
}

string Goblin::getDrop() const // Determines what Goblin should drop 
{
    int rand = randInt(2);
    if (rand == 0)
        return "magic axe";
    else
        return "magic fangs of sleep";
}

string Goblin::getName() const
{
    return "the Goblin ";
}