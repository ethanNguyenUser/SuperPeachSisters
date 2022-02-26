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
    
    bool checkCollision(int x, int y, Actor* actor, bool isFalling);

    
    // Add an actor to the world.
  void addActor(Actor* a);

    // Record that a level was finished.  The parameter is true if it was
    // the last level.
  void endLevel(bool isGameWon);

    // If a can move to (destx,desty), move it and return true; otherwise
    // bonk the object that's blockig it and return false.
  bool moveOrBonk(Actor *a, int destx, int desty) const;

    // If a can move to (destx,desty), move it and return true; otherwise,
    // return false.
  bool moveIfPossible(Actor* a, int destx, int desty) const;

    // If a can move to (destx,destx), return true (but don't move it);
    // otherwise (it would be blocked), return false.
  bool isMovePossible(Actor* a, int destx, int desty) const;

    // If Peach overlaps bonker, bonk 'er and return true; otherwise,
    // return false.
  bool bonkOverlappingPeach(Actor* bonker) const;

    // If a non-Peach actor overlaps bonker, bonk that non-Peach actor and
    // return true; otherwise, return false.  (The bonker will be Peach.)
  void bonkOverlappingActor(Actor* bonker) const;

    // If Peach overlaps damager, damage her and return true; otherwise,
    // return false.
  bool damageOverlappingPeach(Actor* damager) const;

    // If a non-Peach actor overlaps damager, damage that non-Peach actor
    // and return true; otherwise, return false.
  bool damageOverlappingActor(Actor* damager) const;

    // Return true if a overlaps Peach; otherwise, return false.
  bool overlapsPeach(Actor* a) const;

    // If the y cooodinates of a and Peach are at least yDeltaLimit apart,
    // return false; otherwise, set xDeltaFromActor to the difference
    // between Peach's and a's x coordinates (positive means Peach is to
    // the right of a) and return true.
  bool getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const;

    // Set Peach's hit points to hp.
  void setPeachHP(int hp) const;

    // Grant Peach invincibility for this number of ticks.
  void grantInvincibility(int ticks) const;

    // Grant Peach Shoot Power.
  void grantShootPower() const;

    // Grant Peach Jump Power.
  void grantJumpPower() const;
    
private:
    std::vector<Actor*> m_actors;
    Peach* m_peach;
};

#endif // STUDENTWORLD_H_
