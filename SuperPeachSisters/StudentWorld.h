#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool checkCollision(int x, int y, Actor* actor);
    bool checkCollisionFalling(int x, int y, Actor* actor);

    
private:
    std::vector<Actor*> m_actors;
    Peach* m_peach;
};

#endif // STUDENTWORLD_H_
