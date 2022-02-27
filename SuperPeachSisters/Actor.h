#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//global constants
const int DEFAULT_DEPTH = 0;
const int LEVEL_CLEARANCE_SCORE = 1000;

const int FALL_DISTANCE = 4;
const int JUMP_DISTANCE = 4;
const int MOVEMENT_DISTANCE = 4;
const int JUMP_HEIGHT_CHECK = 1;
const int FIREBALL_SPAWN_DISTANCE = 4;

const int FLOWER_SCORE = 50;
const int MUSHROOM_SCORE = 75;
const int STAR_SCORE = 100;

const int GOODIE_FALL_DISTANCE = 2;
const int GOODIE_MOVEMENT_DISTANCE = 2;

const int PROJECTILE_FALL_DISTANCE = 2;
const int PROJECTILE_MOVEMENT_DISTANCE = 2;

const int STAR_TICKS = 150;
const int FB_RECHARGE_TICKS = 8;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    //constructor and destructors
    Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir = right, int depth = DEFAULT_DEPTH);
    virtual ~Actor(){}
    
    //base actions
    virtual void doSomething();
    virtual void getBonked(bool bonkerIsInvinciblePeach);
    virtual void bonk();
    
    //getters and setters
    StudentWorld* sWP() const;
    bool isAlive() const;
    void setDead();
    
    //behavior differentiators
    virtual bool impedes() const;
    
    //doActionIfPossible
    virtual void sufferDamageIfDamageable();
    void fallIfPossible(int dist);
    
    //helper functions
    bool collides(int x, int y, int x0, int y0);
    void reverseDirection();
    void converDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const;
    
private:
    virtual void doSomethingAux() = 0;
    StudentWorld* m_sWP;
    bool m_alive;
};

class Peach : public Actor{
public:
    //constructor and destructor
    Peach(int startX, int startY, StudentWorld* sWP);
    virtual ~Peach(){}
    
    //standard actions
    virtual void getBonked(bool bonkerIsInvinciblePeach) override;
    virtual void sufferDamageIfDamageable() override;
    virtual void bonk() override;
    
    //Peach actions
    void setHP(int hp);
    int getHP() const;
    void gainInvincibility(int ticks);
    void gainShootPower();
    void gainJumpPower();
    bool isInvincible() const;
    bool hasShootPower() const;
    bool hasJumpPower() const;
    
    
private:
    virtual void doSomethingAux() override;
    
    //private member variables
    int m_hp;
    int m_invTick;
    int m_tempInvTick;
    int m_fBTick;
    int m_remainingJumpDistance;
    bool m_hasShootPower;
    bool m_hasJumpPower;
};

class Obstacle : public Actor{
public:
    Obstacle(int imageID, int startX, int startY, StudentWorld* sWP);
    virtual ~Obstacle(){}
    
    virtual bool impedes() const override;
    virtual void doSomethingAux() override;
private:
};

class Block : public Obstacle{
public:
    //define type for block
    enum GoodieType {none, flower, mushroom, star};
    
    //constructor and destructor
    Block(int startX, int startY, StudentWorld* sWP, GoodieType g = none);
    virtual ~Block(){}

    virtual void bonk();
    
private:
    GoodieType m_g;
    bool m_wasBonked;
};

class Pipe : public Obstacle{
public:
    //constructor and destructor
    Pipe(int startX, int startY, StudentWorld* sWP);
    virtual ~Pipe(){}
    
    virtual void bonk();
private:
};

//Mario or Flag
class Objective: virtual public Actor{
public:
    //constructor and destructor
    Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder);
    virtual ~Objective(){}
    
    virtual void doSomethingAux();
private:
    bool m_isGameEnder;
};

class Goodie : public Actor{
public:
    //constructor and destructor
    Goodie(int imageID, int startX, int startY, StudentWorld* sWP);
    virtual ~Goodie(){}
    
    virtual void doSomethingAux();
    
private:
    virtual void doSomethingGoodieAux() = 0;
};

class Flower : public Goodie{
public:
    //constructor and destructor
    Flower(int startX, int startY, StudentWorld* sWP);
    virtual ~Flower(){}
    
    virtual void doSomethingGoodieAux();

private:
};

class Mushroom : public Goodie{
public:
    //constructor and destructor
    Mushroom(int startX, int startY, StudentWorld* sWP);
    virtual ~Mushroom(){}
    
    virtual void doSomethingGoodieAux();
    
private:
};

class Star : public Goodie{
public:
    //constructor and destructor
    Star(int startX, int startY, StudentWorld* sWP);
    virtual ~Star(){}
    
    virtual void doSomethingGoodieAux();
private:
};

class Projectile : public Actor{
public:
    //constructor and destructor
    Projectile(int imageID, int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~Projectile(){}
    
    virtual void doSomethingAux();
private:
    virtual void doSomethingProjectileAux() = 0;
};

class PiranhaFireball : public Projectile{
public:
    
private:
};

class PeachFireball : public Projectile{
public:
    //constructor and destructor
    PeachFireball(int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~PeachFireball(){}
    
    virtual void doSomethingProjectileAux() override;

private:
};

class Shell : public Projectile{
public:
    
private:
};

class Enemy : public Actor{
public:
    
private:
};

class Goomba : public Enemy{
public:
    
private:
};

class Koopa : public Enemy{
public:
    
private:
};

class Piranha : public Enemy{
public:
    
private:
};

#endif // ACTOR_H_
