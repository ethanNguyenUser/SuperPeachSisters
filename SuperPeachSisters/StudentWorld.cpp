#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

//GameWorld functions
//int getLevel() const;
//int getLives() const;
//void decLives();
//void incLives();
//int getScore() const;
//void increaseScore(int howMuch);
//void setGameStatText(string text);
//bool getKey(int& value);
//void playSound(int soundID);

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    //initialize data structures of actors
    
    //allocate/insert Peach, no powers in right location
    
    //allocate/insert all other actors
    
    //loadlevel of actors
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for(int i = 0; i < GRID_HEIGHT - 1; i++){
            for(int j = 0; j < GRID_WIDTH; j++){
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j); // x=5, y=10
                switch (ge){
                case Level::empty:
                    break;
                case Level::koopa:
                        
                    break;
                case Level::goomba:
                    cout << "Location 5,10 starts with a goomba" << endl;
                    break;
                case Level::peach:
                    cout << "Location 5,10 is where Peach starts" << endl;
                    break;
                case Level::flag:
                    cout << "Location 5,10 is where a flag is" << endl;
                    break;
                case Level::block:
                    cout << "Location 5,10 holds a regular block" << endl;
                    break;
                case Level::star_goodie_block:
                    cout << "Location 5,10 has a star goodie block" << endl;
                    break;
                    // etc…
                }
            }
        }
    }
    
    //initialize number of actors to destroy
    
    //return GWSTATUS_LEVEL_ERROR if no level data file exists or file is improperly formatted
        
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
//    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
//    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//    decLives();
//
//    //doSomething() for all actors
//    //  if doSomething() for actor kills Peach
//    //      play SOUND_PLAYER_DIE using playSound()
//    //      return GWSTATUS_PLAYER_DIED
//
//    //if Peach reaches flag
//    //  play SOUND_FINISHED_LEVEL using playSound()
//    //  return GWSTATUS_PLAYER_DIED
//
//    //if Peach reaches Mario
//    //  play SOUND_GAME_OVER using playSound()
//    //  return GWSTATUS_PLAYER_WON
//
//    //delete dead actors
//
//    //update statusText using setGameStatText()
//
//    //continue on if nothing else has happened
//    return GWSTATUS_CONTINUE_GAME;
    
    
    
//    // The term "actors" refers to all actors, e.g., Peach, goodies,
//    // enemies, flags, blocks, pipes, fireballs, etc.
//    // Give each actor a chance to do something, incl. Peach
//    for each of the actors in the game world
//    {
//        if (that actor is still active/alive)
//        {
//            // tell that actor to do something (e.g. move)
//            that actor -> doSomething();
//            if (Peach died during this tick) {
//                play dying sound
//                return GWSTATUS_PLAYER_DIED;
//            }
//            if (Peach reached Mario) {
//                play game over sound
//                return GWSTATUS_WON_GAME;
//            }
//            if (Peach competed the current level) {
//                play completed level sound
//                return GWSTATUS_FINISHED_LEVEL;
//            }
//        }
//    }
//    // Remove newly-dead actors after each tick
//    remove dead game objects
//    // Update the game status line
//    update display text // update the score/lives/level text at screen top
//    // the player hasn’t completed the current level and hasn’t died, so
//    // continue playing the current level
//    return GWSTATUS_CONTINUE_GAME;
    
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    decLives();
    //delete all actors
}
