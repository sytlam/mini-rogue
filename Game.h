//
//  Game.h
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.
class Dungeon;
class Player;

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game(); 
    void play();
private:
    int level; 
    Dungeon* board;
    Player* user;
    int smellDist; 
};

#endif // GAME_INCLUDED

