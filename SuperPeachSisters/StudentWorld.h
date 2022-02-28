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

class StudentWorld : public GameWorld{
public:
    //constructor and destructor
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    
    //GameWorld-called functions
    virtual int init();
    virtual int move();
    virtual void cleanUp();
        
    //Actor-called functions
    void addActor(Actor* a);
    void endLevel(bool isGameWon);
    bool moveOrBonk(Actor *a, int destx, int desty) const;
    bool moveIfPossible(Actor* a, int destx, int desty) const;
    bool isMovePossible(Actor* a, int destx, int desty) const;
    bool bonkOverlappingPeach(Actor* bonker) const;
    void bonkOverlappingActor(Peach* bonker) const;
    bool damageOverlappingPeach(Actor* damager) const;
    bool damageOverlappingActor(Actor* damager) const;
    bool overlapsPeach(Actor* a) const;
    bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const;
    void grantInvincibility(int ticks) const;
    void grantShootPower() const;
    void grantJumpPower() const;
            
private:
    //helper functions
    bool collides(int x, int y, int x0, int y0) const;
    void updateGameText();
    int gridToCoord(int grid) const;
    
    //private member variables that store actors
    std::vector<Actor*> m_actors;
    Peach* m_peach;
    
    //private member variables that store state of game
    bool levelIsEnded;
    bool gameIsEnded;
};

#endif // STUDENTWORLD_H_
