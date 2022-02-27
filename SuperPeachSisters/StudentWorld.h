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
    
    bool collides(int x, int y, int x0, int y0) const;
    
    void addActor(Actor* a);
    void endLevel(bool isGameWon);
    bool moveOrBonk(Actor *a, int destx, int desty) const;
    bool moveIfPossible(Actor* a, int destx, int desty) const;
    bool isMovePossible(Actor* a, int destx, int desty) const;
    bool bonkOverlappingPeach(Actor* bonker) const;
    bool bonkOverlappingActor(Peach* bonker) const;
    bool damageOverlappingPeach(Actor* damager) const;
    bool damageOverlappingActor(Actor* damager) const;
    bool overlapsPeach(Actor* a) const;
    bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const;
    void setPeachHP(int hp) const;
    void grantInvincibility(int ticks) const;
    void grantShootPower() const;
    void grantJumpPower() const;
            
private:
    int gridToCoord(int grid) const;
    
    std::vector<Actor*> m_actors;
    Peach* m_peach;
    
    bool levelIsEnded;
    bool gameIsEnded;
};

#endif // STUDENTWORLD_H_
