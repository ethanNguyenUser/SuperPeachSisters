#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    //constructor and destructors
    Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir = right, int depth = 0);
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
    virtual bool canMove() const;
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
    virtual bool canMove() const override;
    
    //Peach actions
    void setHP(int hp);
    void gainInvincibility(int ticks);
    void gainShootPower();
    void gainJumpPower();
    bool isInvincible() const;
    bool hasShootPower() const;
    bool hasJumpPower() const;
    
    
private:
    virtual void doSomethingAux() override;
    
    //private member variables
    int m_health;
    int m_invTick;
    int m_tempInvTick;
    int m_fBTick;
    int m_remainingJumpDistance;
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
    Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder);
    virtual ~Objective(){}

    //    virtual void bonk();
    
    virtual void doSomethingAux();
private:
    bool m_isGameEnder;
};

class Goodie : public Actor{
public:
    virtual ~Goodie(){}
    
private:
};

class Flower : public Goodie{
public:
    virtual ~Flower(){}

private:
};

class Mushroom : public Goodie{
public:
    virtual ~Mushroom(){}

private:
};

class Star : public Goodie{
public:
    virtual ~Star(){}
    
private:
};

class Projectile : public Actor{
public:
    
private:
};

class PiranhaFireball : public Projectile{
public:
    
private:
};

class PeachFireball : public Projectile{
public:
    
private:
    virtual void doSomethingAux();
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
