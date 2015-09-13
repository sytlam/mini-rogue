//
//  Dungeon.cpp
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#include "Dungeon.h"
#include "utilities.h"
#include "Actors.h"
#include "Objects.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std; 

Dungeon::Dungeon(Player* p, int lvl, int dist)
{
    user = p;
    level = lvl;
    smellDist = dist; 
    found = inventoryOpen = false; // not found the exit/golden idol
    
    // No open space in the dungeon
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            dungeon[i][j] = '#';
        }
    }
    createRooms();
}

Dungeon::~Dungeon()
{
    for (int i = 0; i < items.size(); i++)
    {
        delete items[i];
    }
    for (int i = 0; i < pieces.size(); i++)
    {
        delete pieces[i]; 
    }
}

void Dungeon::displayDungeon() // Places items and monsters on the grid, and then prints out the grid
{
    int count = 0;
    for (vector<Actors*>::iterator i = pieces.begin(); i != pieces.end(); i++) // Erase all dead monsters from the grid
    {
        if ((*i)->getHP() <= 0)
        {
            pieces.erase(pieces.begin() + count);
            break;
        }
        count++;
    }
    for (vector<Objects*>::iterator i = items.begin(); i != items.end(); i++) // Place all items in the grid
    {
        dungeon[(*i)->getRow()][(*i)->getCol()] = (*i)->getSymbol();
    }
    dungeon[user->getRow()][user->getCol()] = '@'; // Place user in the grid

    for(vector<Actors*>::iterator i = pieces.begin(); i != pieces.end(); i++) // Place all monsters in the grid
    {
        dungeon[(*i)->getRow()][(*i)->getCol()] = (*i)->getPiece();
    }
    
    // Print out the dungeon
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            cout << dungeon[i][j];
        }
        cout << endl;
    }
}

void Dungeon::createRooms() // Create rooms to be placed in the dungeon
{
    int startRow, startCol;
    int prevCenterRow, prevCenterCol;
    int numRooms = 0;
    
    for (int i = 0; i < 30; i++) // 30 attempts to make rooms
    {
        if (i == 0) // make sure the first room fits in the dungeon
        {
            startRow = randInt(7) + 1;
            startCol = randInt(20) + 1;
        }
        else
        {
            startRow = randInt(10) + 1;
            startCol = randInt(68) + 1;
        }
    
        int length = rand() % 8 + 4; // random length in range from 4 to 11
        int width = rand() % 8 + 6; // random width in range from 6 to 13

        // Get the coordinates for the center of the room
        int centerRow = (((startRow+length)-startRow)/2) + startRow;
        int centerCol = (((startCol+width)-startCol)/2) + startCol;

        if (verifyEmpty(startRow, startCol, length, width)) // nothing in the way of creating the room
        {
            fillRoom(startRow, startCol, length, width); // Fill the room with empty space
            if (numRooms > 0) // Create a corridor that links the room with the previous room made
                createCorridors(centerRow, centerCol, prevCenterRow, prevCenterCol);
            prevCenterRow = centerRow;
            prevCenterCol = centerCol;
            numRooms++;
        }
    }
    setUpDungeon(); // Determine how many monsters and items to place in the grid
}

void Dungeon::fillRoom(int r, int c, int l, int w) // Fills a room of given dimensions with empty space
{
    for (int i = r; i < r + l; i++)
    {
        for (int j = c; j < w + c; j++)
        {
            if (i == r || i == r + l || j == c || j == c + w) // walls around the borders of the room
                dungeon[i][j] = '#';
            else
                dungeon[i][j] = ' '; // fill the interior of the room with white space
        }
    }
}

bool Dungeon::verifyEmpty(int r, int c, int l, int w) // Make sure that there's nothing in the way of creating the room
{
    for (int i = r; i < r + l; i++)
    {
        for (int j = c; j < w + c; j++)
        {
            if (dungeon[i][j] == ' ') // already a room in the place of where the new room is to be created
                return false;
            if (j >= 69) // out of bounds
                return false;
        }
        if (i >= 17) // out of bounds
            return false;
    }
    return true;
}

// Creates corridors that links the new room created to the previous room
void Dungeon::createCorridors(int centerRow, int centerCol, int prevCenterRow, int prevCenterCol)
{
    if (centerCol < prevCenterCol) // new room is to the left of the previous room
    {
        for (int i = centerCol; i <= prevCenterCol; i++) // draw a horizontal corridor to connect the 2 rooms
        {
            dungeon[centerRow][i] = ' ';
        }
        if (prevCenterRow < centerRow) // draw a vertical corridor upwards to connect the 2 rooms
        {
            for (int j = centerRow; j >= prevCenterRow; j--)
                dungeon[j][prevCenterCol] = ' ';
        }
        else // draw a vertical corridor downwards to connect the 2 rooms
        {
            for (int j = centerRow; j <= prevCenterRow; j++)
                dungeon[j][prevCenterCol] = ' ';
        }
    }
    else // new room is to the right of the previous room
    {
        for (int i = prevCenterCol; i <= centerCol; i++) // draw a horizontal corridor to connect the 2 rooms
        {
            dungeon[prevCenterRow][i] = ' ';
        }
        if (prevCenterRow < centerRow) // draw a vertical corridor upwards to connect the 2 rooms
        {
            for (int j = prevCenterRow; j <= centerRow; j++)
                dungeon[j][centerCol] = ' ';
        }
        else // draw a vertical corridor downwards to connect the 2 rooms
        {
            for (int j = prevCenterRow; j >= centerRow; j--)
                dungeon[j][centerCol] = ' '; 
        }
    }
}

void Dungeon::setUpDungeon() // Determine how many monsters and items to put in the dungeon
{
    // get all the blank spaces in dungeon
    vector<int> blankRows;
    vector<int> blankCols;
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            if (dungeon[i][j] == ' ')
            {
                blankRows.push_back(i);
                blankCols.push_back(j);
            }
        }
    }
    
    // choose a random blank space to put either the staircase or the golden idol
    int random = randInt(blankRows.size());
    endRow = blankRows.at(random);
    endCol = blankCols.at(random);
    if (level != 4)
        dungeon[endRow][endCol] = '>';
    else
        dungeon[endRow][endCol] = '&';
    
    blankRows.erase(blankRows.begin() + random);
    blankCols.erase(blankCols.begin() + random);
    
    // generate random amounts of weapons and scrolls (2-3 maximum)
    int max = randInt(2) + 2; // generate random max amount of weapons/scrolls (between 2 and 3)
    int numWeapons = randInt(max+1);
    int numScrolls = max - numWeapons;
    
    for (int i = 0; i < numWeapons; i++) // Place the determined # of weapons in the dungeon
    {
        int weaponNo = randInt(3); // random # from 0-2 to represent the different weapons
        random = randInt(blankRows.size());
        if (weaponNo == 0)
            items.push_back(new Weapons(blankRows.at(random), blankCols.at(random), "short sword", user));
        else if (weaponNo == 1)
            items.push_back(new Weapons(blankRows.at(random), blankCols.at(random), "long sword", user));
        else
            items.push_back(new Weapons(blankRows.at(random), blankCols.at(random), "mace", user));
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
    
    for (int i = 0; i < numScrolls; i++) // Place the determined # of scrolls in the dungeon
    {
        int scrollNo = randInt(4); // random # from 0-3 to represent the different scrolls
        random = randInt(blankRows.size());
        if (scrollNo == 0)
            items.push_back(new Scroll(blankRows.at(random), blankCols.at(random), "enhance armor", user));
        else if (scrollNo == 1)
            items.push_back(new Scroll(blankRows.at(random), blankCols.at(random), "strength", user));
        else if (scrollNo == 2)
            items.push_back(new Scroll(blankRows.at(random), blankCols.at(random), "enhance health", user));
        else
            items.push_back(new Scroll(blankRows.at(random), blankCols.at(random), "enhance dexterity", user));
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
    
    // choose a random space to put the player
    random = randInt(blankRows.size());
    user->setRow(blankRows.at(random));
    user->setCol(blankCols.at(random));
    blankRows.erase(blankRows.begin() + random);
    blankCols.erase(blankCols.begin() + random);
    
    // Determine random numbers of each monster
    int numMonsters = 2 + randInt(5*(level+1));
    int numSnakes = randInt(numMonsters);
    int numBogey = 0, numDragon = 0;
    int numGoblin = numMonsters-numSnakes;
    if (level >= 2)
    {
        numBogey = randInt(numMonsters - numSnakes);
        if (level >= 3)
            numDragon = randInt(numMonsters-numSnakes-numBogey);
        numGoblin = numMonsters-numSnakes-numBogey-numDragon;
    }
    
    // Place the determined # of the appropriate monster in the grid
    for (int i = 0; i < numSnakes; i++)
    {
        random = randInt(blankRows.size());
        Snakewomen* s = new Snakewomen(this, blankRows.at(random), blankCols.at(random), randInt(4) + 3, 3, 2, 3, user);
        pieces.push_back(s);
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
    for (int i = 0; i < numBogey; i++)
    {
        random = randInt(blankRows.size());
        Bogeyman* b = new Bogeyman(this, blankRows.at(random), blankCols.at(random), randInt(6) + 5, 2, randInt(2) + 2, randInt(2)+2, user);
        pieces.push_back(b);
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
    for (int i = 0; i < numDragon; i++)
    {
        random = randInt(blankRows.size());
        Dragon* d = new Dragon(this, blankRows.at(random), blankCols.at(random), randInt(6) + 20, 4, 4, 4, user);
        pieces.push_back(d);
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
    for (int i = 0; i < numGoblin; i++)
    {
        random = randInt(blankRows.size());
        Goblin* g = new Goblin(this, blankRows.at(random), blankCols.at(random), randInt(6) + 15, 1, 3, 1, user, smellDist);
        pieces.push_back(g);
        blankRows.erase(blankRows.begin() + random);
        blankCols.erase(blankCols.begin() + random);
    }
}

void Dungeon::displayStatus() // Displays the player's stats
{
    cout << "Dungeon Level: " << level << ", Hit points: " << user->getHP() << ", Armor: " << user->getArmor() << ", Strength: " << user->getStrength() << ", Dexterity: " << user->getDex() << endl << endl;
}

bool Dungeon::move(char action) // Moves the player
{
    monster = nullptr; // player isn't attacking any monster
    int rand = randInt(10); // random integer from 0 to 9
    if (rand == 0) // Player regains HP
    {
        if (user->getHP() < user->getMaxHP())
            user->setHP(user->getHP() + 1);
    }
    found = false;
    if (user->getSleep() == 0) // Player is awake
    {
        if (action == '>') // advancing to next level
        {
            if (checkFinish(user->getRow(), user->getCol()))
            {
                found = true;
                return true;
            }
        }
        else if (action == 'g') // picking up item
        {
            if (level == 4) // picking up the golden idol
            {
                if (checkFinish(user->getRow(), user->getCol()))
                {
                    found = true;
                    moveMonsters(action);
                    return true;
                }
            }
            if (checkItem(user->getRow(), user->getCol())) // picking up a regular item
            {
                removeItem(user->getRow(), user->getCol());
                moveMonsters(action);
                return true;
            }
            moveMonsters(action);
            return false;
        }
        else if (action == 'i') // displays the inventory
        {
            if (user->getSleep() == 0)
            {
                if (!inventoryOpen) // opens inventory if it's not already open
                {
                    clearScreen();
                    user->displayInventory();
                    inventoryOpen = true;
                    return true;
                }
                inventoryOpen = false; // closes inventory if it was already open
            }
            moveMonsters(action);
            return false;
        }
        else if (action == 'w') // wields a weapon from the player's inventory
        {
            if (user->getSleep() == 0)
            {
                clearScreen();
                user->displayInventory();
                char choice = getCharacter(); // get what weapon the player wants to wield
                moveMonsters(action);
                user->wieldWeapon(choice); // wield the weapon
                return true;
            }
            moveMonsters(action);
            return false;
        }
        else if (action == 'r') // reads a scroll from the player's inventory
        {
            if (user->getSleep() == 0)
            {
                clearScreen();
                user->displayInventory();
                char choice = getCharacter(); // get what scroll the player wants to read
                moveMonsters(action);
                user->readScroll(choice); // read the scroll
                return true;
            }
            moveMonsters(action);
            return false;
        }
        else if (action == 'c') // allows player to cheat by setting HP to 50 and strength to 9
        {
            user->setMaxHP(50);
            user->setHP(50);
            user->setStrength(9);
        }
        else // moving the player
        {
            int origRow = user->getRow();
            int origCol = user->getCol();
            if (user->movePiece(action, dungeon)) // if the player can move, move to the appropriate place
                dungeon[origRow][origCol] = ' ';
            else if (monster != nullptr) // if the player is next to a monster, attack the monster
                user->attack(user, monster);
            if (level != 4)
                dungeon[endRow][endCol] = '>';
            else
                dungeon[endRow][endCol] = '&';
            for (vector<Objects*>::iterator i = items.begin(); i != items.end(); i++)
            {
                if (user->getRow() != (*i)->getRow() && user->getCol() != (*i)->getCol())
                    dungeon[(*i)->getRow()][(*i)->getCol()] = (*i)->getSymbol();
            }
            moveMonsters(action);
            return true;
        }
    }
    else // player is asleep, so decrement the sleep counter by 1
        user->setSleep(user->getSleep()-1);
    moveMonsters(action);
    return false;
}

bool Dungeon::checkMove(int row, int col) // checks that a valid move in the dungeon can be made
{
    if (row >= 0 && row <= 18 && col >= 0 && col <= 70 && dungeon[row][col] != '#') // move is within bounds and not blocked by a wall
    {
        for (vector<Actors*>::iterator i = pieces.begin(); i != pieces.end(); i++)
        {
            if ((*i)->getRow() == row && (*i)->getCol() == col)
            {
                monster = *i; // monster occupying the space that player wants to move to
                return false; // move blocked by a monster
            }
        }
        return true;
    }
    return false;
}

bool Dungeon::checkFinish(int row, int col) // determines whether the player is standing on the golden idol
{
    if (row == endRow && col == endCol)
        return true;
    return false;
}

bool Dungeon::checkItem(int row, int col) // determines whether the player is standing on an item
{
    for (vector<Objects*>::iterator i = items.begin(); i != items.end(); i++)
    {
        if (row == (*i)->getRow() && col == (*i)->getCol())
            return true;
    }
    return false;
}

void Dungeon::removeItem(int row, int col) // removes an item from the grid after the player has picked it up
{
    if (items.size() <= 25) // still have space in the inventory
    {
        for (vector<Objects*>::iterator i = items.begin(); i != items.end(); i++)
        {
            if (row == (*i)->getRow() && col == (*i)->getCol())
            {
                cout << "You pick up ";
                if ((*i)->getSymbol() == ')')
                    cout << (*i)->getName() << endl;
                else if ((*i)->getSymbol() == '?')
                    cout << "a scroll called " << (*i)->getName() << endl;
                user->addToInventory(*i);
                dungeon[(*i)->getRow()][(*i)->getCol()] = ' ';
                items.erase(i);
                break;
            }
        }
    }
    else
        cout << "Your knapsack is full; you can't pick that up." << endl;
}

bool Dungeon::getFound() const // user has found the golden idol
{
    return found;
}

void Dungeon::moveToRandom() // moves player to a random spot for the teleportation scroll
{
    // gets all the blank spaces in the dungeon
    vector<int> blankRows;
    vector<int> blankCols;
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            if (dungeon[i][j] == ' ')
            {
                blankRows.push_back(i);
                blankCols.push_back(j);
            }
        }
    }
    
    // moves player to a random blank space
    int random = randInt(blankRows.size());
    user->setRow(blankRows.at(random));
    user->setCol(blankCols.at(random));
}

void Dungeon::moveMonsters(char action) // moves the monsters in the grid
{
    for (vector<Actors*>::iterator i = pieces.begin(); i != pieces.end(); i++)
    {
        int origRow = (*i)->getRow();
        int origCol = (*i)->getCol();
        if ((*i)->movePiece(action, dungeon)) // monster makes its move by either moving to another space or attacking the player
            dungeon[origRow][origCol] = ' ';
        else if ((*i)->isDead()) // if the monster has been killed
        {
            dungeon[origRow][origCol] = ' ';
            if ((*i)->drop() && origRow != endRow && origCol != endCol) // determine if monster should drop something
            {
                Dragon* d = dynamic_cast<Dragon*>(*i);
                if (d != nullptr) // the monster killed is a dragon
                    items.push_back(new Scroll(origRow, origCol, (*i)->getDrop(), user));
                else
                    items.push_back(new Weapons(origRow, origCol, (*i)->getDrop(), user));
            }
        }
    }
    if (level != 4)
        dungeon[endRow][endCol] = '>';
    else
        dungeon[endRow][endCol] = '&';
    for (vector<Objects*>::iterator i = items.begin(); i != items.end(); i++)
    {
        if (user->getRow() != (*i)->getRow() && user->getCol() != (*i)->getCol())
            dungeon[(*i)->getRow()][(*i)->getCol()] = (*i)->getSymbol();
    }
}

void Dungeon::printAttackMessages() // Prints the series of attack messages underneath the player's stats
{
    for (vector<string>::iterator i = attackMessages.begin(); i != attackMessages.end(); i++)
    {
        cout << (*i) << endl;
    }
    attackMessages.clear(); 
}

void Dungeon::addAttackMessages(string msg)
{
    attackMessages.push_back(msg);
}

vector<Actors*> Dungeon::getMonsters() const
{
    return pieces;
}
