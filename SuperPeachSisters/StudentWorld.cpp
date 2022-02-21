#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
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

GameWorld* createStudentWorld(string assetPath){
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath){}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init(){
    //use level file to create Actor objects
    Level lev(assetPath());
    ostringstream oss;
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel();
    oss << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found){
        cerr << "Could not find " << level_file << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format){
        cerr << level_file << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        
        //run through level object grid
        for(int i = 0; i < GRID_WIDTH; i++){
            for(int j = 0; j < GRID_HEIGHT; j++){
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j);
                switch (ge){
                    case Level::empty:
                        break;
                    case Level::peach:
                        m_peach = new Peach(i, j, this);
                        break;
                    case Level::block:
                        m_actors.push_back(new Block(i, j, this));
                        break;
                    case Level::pipe:
                        m_actors.push_back(new Pipe(i, j, this));
                        break;
                    case Level::flag:
                        m_actors.push_back(new Flag(i, j, this));
                        break;
                    case Level::mario:
                        break;
                    case Level::flower_goodie_block:
                        break;
                    case Level::mushroom_goodie_block:
                        break;
                    case Level::star_goodie_block:
                        break;
                    case Level::goomba:
                        break;
                    case Level::koopa:
                        break;
                    case Level::piranha:
                        break;
                }
            }
        }
    }
            
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // The term "actors" refers to all actors, e.g., Peach, goodies,
    // enemies, flags, blocks, pipes, fireballs, etc.
    // Give each actor a chance to do something, incl. Peach
   
    //other Actor actions
    for(int i = 0; i < m_actors.size(); i++){
        Actor* actor = m_actors[i];
        if (actor->isAlive()){
            // tell that actor to do something (e.g. move)
            m_actors[i]->doSomething();
            
            if(!m_peach->isAlive()){
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
//            if (Peach reached Mario) {
//                play game over sound
//                return GWSTATUS_WON_GAME;
//            }
//            if (Peach competed the current level) {
//                play completed level sound
//                return GWSTATUS_FINISHED_LEVEL;
//            }
        }
    }
    
    m_peach->doSomething();

    // Remove newly-dead actors after each tick
    for(int i = 0; i < m_actors.size(); i++){
        if(!m_actors[i]->isAlive()){
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
    }
//    // Update the game status line
//    update display text // update the score/lives/level text at screen top
//    // the player hasn’t completed the current level and hasn’t died, so
//    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
    
//    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    decLives();
    delete m_peach;
    for(int i = 0; i < m_actors.size(); i++){
        delete m_actors[i];
    }
    m_actors.clear();
}

//returns true ifthe object impedes
bool StudentWorld::checkCollision(int x, int y, Actor* actor){
    for(int i = 0; i < m_actors.size(); i++){
        Actor* other = m_actors[i];
        if(actor->collides(x, y, other->getX(), other->getY())){
            other->bonk();
            if(other->impedes())
                return true;
        }
    }
    return false;
}
