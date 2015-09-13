//
//  Dungeon.h
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include <string>
using namespace std;

class Player;
class Actors;
class Objects; 

class Dungeon
{
public:
    Dungeon(Player* p, int lvl, int dist);
    ~Dungeon(); 
    void displayDungeon();
    void createRooms();
    void fillRoom(int r, int c, int l, int w);
    bool verifyEmpty(int r, int c, int l, int w);
    void createCorridors(int centerRow, int centerCol, int prevCenterRow, int prevCenterCol);
    void setUpDungeon(); 
    void displayStatus();
    bool move(char action);
    bool checkMove(int row, int col);
    bool checkFinish(int row, int col);
    bool checkItem(int row, int col);
    void removeItem(int row, int col);
    bool getFound() const;
    void moveToRandom(); // moves player to a random blank spot
    void moveMonsters(char action);
    void printAttackMessages();
    void addAttackMessages(string msg);
    vector<Actors*> getMonsters() const; 
private:
    int level;
    int endRow, endCol; // row and column of the staircase/golden idol
    char dungeon[18][70];
    Player* user;
    bool found, inventoryOpen;
    vector<Actors*> pieces;
    vector<Objects*> items;
    vector<string> attackMessages;
    Actors* monster; // monster that player wants to attack
    int smellDist; // goblin smell distance 
};

#endif // DUNGEON_H
