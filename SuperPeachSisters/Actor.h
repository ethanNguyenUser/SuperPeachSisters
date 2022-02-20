#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir, int depth, double size = 1);
    virtual void doSomething() = 0;
    virtual void bonk() = 0;
    
    //getters and setters
    StudentWorld* sWP() const;
    bool isAlive() const;
    void setAlive();
    void setDead();
    
    //behavior differentiators
    virtual bool canMove() const;
    virtual bool impedes() const;
    
    bool collides(int x, int y, int x0, int y0);
    
private:
    StudentWorld* m_sWP;
    bool m_alive;
};

class MobileActor : virtual public Actor{
public:
    virtual bool canMove() const override;
};

class Peach : public MobileActor{
public:
    Peach(int startX, int startY, StudentWorld* sWP);
    virtual ~Peach();

    virtual void doSomething() override;
    virtual void bonk() override;
    
private:
    int m_health;
    
    int m_invTick;
    int m_tempInvTick;
    int m_fBTick;
};

class Obstacle : virtual public Actor{
public:
    virtual bool impedes() const override;
    virtual void doSomething() override;
private:
};

class Block : public Obstacle{
public:
    Block(int startX, int startY, StudentWorld* sWP);
    virtual void bonk();
private:
    bool m_wasBonked;
};

class Pipe : public Obstacle{
public:
    Pipe(int startX, int startY, StudentWorld* sWP);
    virtual void bonk();
private:
};

//Mario or Flag
class Objective: virtual public Actor{
public:
//    virtual void bonk();
private:
};

class Flag : public Objective{
public:
    Flag(int startX, int startY, StudentWorld* sWP);
    virtual void doSomething();
    virtual void bonk();
private:
};

class Mario: public Objective{
public:
    
private:
};

class Goodie : public Actor{
public:
    
private:
};

class Flower : public Goodie{
public:
    
private:
};

class Mushroom : public Goodie{
public:
    
private:
};

class Star : public Goodie{
public:
    
private:
};

class PiranhaFireball : public MobileActor{
public:
    
private:
};

class PeachWeapon : public MobileActor{
public:
    
private:
};

class PeachFireball : public PeachWeapon{
public:
    
private:
};

class Shell : public PeachWeapon{
public:
    
private:
};

class Enemy : public Actor{
public:
    
private:
};

class Goomba : virtual public MobileActor, virtual public Enemy{
public:
    
private:
};

class Koopa : virtual public MobileActor, virtual public Enemy{
public:
    
private:
};

class Piranha : public Enemy{
public:
    
private:
};

#endif // ACTOR_H_
