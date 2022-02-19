#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size);
    virtual void doSomething() = 0;
    virtual void bonk() = 0;
    
    //getters and setters
    bool isAlive();
    void setAlive();
    void setDead();
    
    //behavior differentiators
    virtual bool canMove() const;
    virtual bool impedes() const;
    
private:
    bool m_alive;
};

class MobileActor : virtual public Actor{
public:
    virtual bool canMove() const override;
    virtual bool isImpeded(const Actor& other) = 0;
    
    //getters and setters
    bool getImpeded();
    void setImpeded(bool willCollide);
    
    bool collides(int x, int y, int l, int h, int x0, int y0, int l0, int h0);


private:
    bool m_impeded;
};

class Peach : public MobileActor{
public:
    Peach(int startX, int startY);
    virtual ~Peach();

    virtual void doSomething() override;
    virtual void bonk() override;
    
    virtual bool isImpeded(const Actor& other) override;
    void setKey(int key);
    void setKeyIsPressed(bool keyIsPressed);
    
private:
    int m_health;
    
    int m_key;
    bool m_keyIsPressed;
    
    int m_invTick;
    int m_tempInvTick;
    int m_fBTick;
};

class Obstacle : virtual public Actor{
public:
    virtual bool impedes() const override;
private:
};

class Block : public Obstacle{
public:
    Block(int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
private:
};

class Pipe : public Obstacle{
public:
    Pipe(int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
private:
};

//Mario or Flag
class Objective: public Actor{
public:
    
private:
};

class Mario: public Objective{
public:
    
private:
};

//Change later
class Flag : public Actor{
public:
    Flag(int startX, int startY);
    virtual void doSomething();
    virtual void bonk();
private:
};

class Enemy : public Actor{
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

class Weapon : public Actor{
public:
    
private:
};

class PeachWeapon : public Weapon{
public:
    
private:
};

class PiranhaFireball : public Weapon{
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

class MobileEnemy : public Enemy{
public:
    
private:
};

class Goomba : public MobileEnemy{
public:
    
private:
};

class Koopa : public MobileEnemy{
public:
    
private:
};

class Piranha : public Enemy{
public:
    
private:
};

#endif // ACTOR_H_
