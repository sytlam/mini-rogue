//
//  Game.cpp
//  Project 3
//
//  Created by Stephanie Lam on 5/12/15.
//  Copyright (c) 2015 Stephanie Lam. All rights reserved.
//

// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "Dungeon.h"
#include "Actors.h"
#include <iostream>
using namespace std;

Game::Game(int goblinSmellDistance)
{
    level = 0;
    smellDist = goblinSmellDistance;
    user = new Player(nullptr, randInt(17) + 1, randInt(69)); // place player in random spot
    board = new Dungeon(user, level, smellDist);
    user->setDungeon(board);
}

Game::~Game()
{
    delete user;
    delete board; 
}

void Game::play() // get a keyboard command from the player and then take appropriate action
{
    char action;
    bool finish = false;
    clearScreen();
    board->displayDungeon(); // print out the dungeon and the player's stats below it
    board->displayStatus();
    do
    {
        action = getCharacter(); // get keyboard command from player
        switch (action)
        {
                // movement or cheating
            case ARROW_LEFT: case ARROW_DOWN: case ARROW_RIGHT: case ARROW_UP: case 'c':
                board->move(action);
                break;
            case '>': // proceeding to the next level
                if (board->move(action) && level != 4)
                {
                    level++;
                    board = new Dungeon(user, level, smellDist); // make a new dungeon if it's not the last level
                    user->setDungeon(board);
                }
                break;
            case 'g': // picking up an item
                clearScreen();
                board->displayDungeon();
                board->displayStatus();
                board->printAttackMessages();
                if (board->move(action))
                {
                    if (level == 4 && board->getFound()) // picking up the golden idol
                    {
                        cout << "You pick up the golden idol" << endl;
                        cout << "Congratulations, you won!" << endl;
                        finish = true;
                    }
                    else // picking up a regular item
                    {
                        board->printAttackMessages(); // print the series of attack messages beneath the dungeon
                        continue;
                    }
                }
                break;
            case 'i': // displaying inventory, wielding a weapon, or reading a scroll
            case 'w':
            case 'r':
            {
                bool temp = board->move(action);
                if (temp)
                {
                    board->printAttackMessages();
                    continue;
                }
                break;
            }
            default: // character inputted is not a valid command
                board->moveMonsters(action); // don't move player, but move the board
                break; 
        }
        if (finish) // player has picked up golden idol and the game is done
        {
            cout << "Press q to exit game." << endl;
            do
            {
                action = getCharacter();
            } while (action != 'q');
        }
        else // game is not done
        {
            clearScreen();
            board->displayDungeon();
            board->displayStatus();
            board->printAttackMessages(); 
            if (user->getHP() <= 0) // user is dead
            {
                cout << "Press q to exit game." << endl;
                do
                {
                    action = getCharacter();
                } while (action != 'q');
            }
        }
    } while (action != 'q');
}

