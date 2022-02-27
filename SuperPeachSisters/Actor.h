#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//global constants
const int LEVEL_CLEARANCE_SCORE = 1000;

//Peach constants
const int FALL_DISTANCE = 4;
const int JUMP_DISTANCE = 4;
const int NORMAL_JUMP_DISTANCE = 8;
const int MUSHROOM_JUMP_DISTANCE = 12;
const int MOVEMENT_DISTANCE = 4;
const int JUMP_HEIGHT_CHECK = 1;
const int FIREBALL_SPAWN_DISTANCE = 4;
const int STAR_TICKS = 150;
const int FB_RECHARGE_TICKS = 8;
const int TEMP_INV_TICKS = 10;

//Goodie constants
const int FLOWER_SCORE = 50;
const int MUSHROOM_SCORE = 75;
const int STAR_SCORE = 100;
const int GOODIE_FALL_DISTANCE = 2;
const int GOODIE_MOVEMENT_DISTANCE = 2;

//Projectile constants
const int PROJECTILE_FALL_DISTANCE = 2;
const int PROJECTILE_MOVEMENT_DISTANCE = 2;

//Enemy constants
const int ENEMY_MOVEMENT_DISTANCE = 1;
const int ENEMY_SCORE = 100;
const int PIRANHA_DETECTION_DISTANCE = 1.5 * SPRITE_WIDTH;
const int PIRANHA_RANGE = 8 * SPRITE_WIDTH;
const int PIRANHA_COOLDOWN = 40;

class StudentWorld;

class Actor : public GraphObject{
public:
    //constructor and destructors
    Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir = right, int depth = 0);
    virtual ~Actor();
    
    //base actions
    virtual void doSomething();
    virtual void getBonked(bool bonkerIsInvinciblePeach);
    
    //getters and setters
    StudentWorld* sWP() const;
    bool isAlive() const;
    void setDead();
    
    //behavior differentiators
    virtual bool impedes() const;
    virtual bool projectileCanPassThrough() const;
    
    //doActionIfPossible
    virtual void sufferDamageIfDamageable();
    void fallIfPossible(int dist);
    
    //helper functions
    void reverseDirection();
    void convertDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const;
    
private:
    virtual void doSomethingAux() = 0;
    StudentWorld* m_sWP;
    bool m_alive;
};

class Peach : public Actor{
public:
    //constructor and destructor
    Peach(int startX, int startY, StudentWorld* sWP);
    virtual ~Peach();
    
    //standard actions
    virtual void getBonked(bool bonkerIsInvinciblePeach) override;
    virtual void sufferDamageIfDamageable() override;
    
    //Peach actions
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
    //constructor and destructor
    Obstacle(int imageID, int startX, int startY, StudentWorld* sWP);
    virtual ~Obstacle();
    
    //behavior differentiation
    virtual bool impedes() const override;
    
private:
    virtual void doSomethingAux() override;
};

class Block : public Obstacle{
public:
    //define type for block
    enum GoodieType {none, flower, mushroom, star};
    
    //constructor and destructor
    Block(int startX, int startY, StudentWorld* sWP, GoodieType g = none);
    virtual ~Block();

    virtual void getBonked(bool bonkerIsInvinciblePeach);
    
private:
    GoodieType m_g;
    bool m_wasBonked;
};

class Pipe : public Obstacle{
public:
    //constructor and destructor
    Pipe(int startX, int startY, StudentWorld* sWP);
    virtual ~Pipe();
    
private:
};

//Mario or Flag
class Objective: virtual public Actor{
public:
    //constructor and destructor
    Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder);
    virtual ~Objective();
    
private:
    virtual void doSomethingAux() override;
    
    //private member variable
    bool m_isGameEnder;
};

class Goodie : public Actor{
public:
    //constructor and destructor
    Goodie(int imageID, int startX, int startY, StudentWorld* sWP);
    virtual ~Goodie();
        
private:
    virtual void doSomethingAux() override;
    virtual void doSomethingGoodieAux() = 0;
};

class Flower : public Goodie{
public:
    //constructor and destructor
    Flower(int startX, int startY, StudentWorld* sWP);
    virtual ~Flower();
    
private:
    virtual void doSomethingGoodieAux() override;
};

class Mushroom : public Goodie{
public:
    //constructor and destructor
    Mushroom(int startX, int startY, StudentWorld* sWP);
    virtual ~Mushroom();
        
private:
    virtual void doSomethingGoodieAux() override;
};

class Star : public Goodie{
public:
    //constructor and destructor
    Star(int startX, int startY, StudentWorld* sWP);
    virtual ~Star();
    
private:
    virtual void doSomethingGoodieAux() override;

};

class Projectile : public Actor{
public:
    //constructor and destructor
    Projectile(int imageID, int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~Projectile();
    
    bool projectileCanPassThrough() const override;
private:
    virtual void doSomethingAux() override;
    virtual void doSomethingProjectileAux() = 0;
};

class PiranhaFireball : public Projectile{
public:
    //constructor and destructor
    PiranhaFireball(int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~PiranhaFireball();

private:
    virtual void doSomethingProjectileAux() override;
};

class PeachFireball : public Projectile{
public:
    //constructor and destructor
    PeachFireball(int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~PeachFireball();
    
private:
    virtual void doSomethingProjectileAux() override;
};

class Shell : public Projectile{
public:
    //constructor and destructor
    Shell(int startX, int startY, StudentWorld* sWP, int dir);
    virtual ~Shell();
    
private:
    virtual void doSomethingProjectileAux() override;
};

class Enemy : public Actor{
public:
    //constructor and destructor
    Enemy(int imageID, int startX, int startY, StudentWorld* sWP, bool isMobile);
    virtual ~Enemy();
    
    void getBonked(bool bonkerIsInvinciblePeach) override;
    virtual void sufferDamageIfDamageable() override;
    
private:
    virtual void doSomethingAux() override;
    virtual void doSomethingEnemyDeathAux();
    virtual void doSomethingEnemyAux();
    
    bool m_isMobile;
};

class Goomba : public Enemy{
public:
    //constructor and destructor
    Goomba(int startX, int startY, StudentWorld* sWP);
    virtual ~Goomba();
    
private:
};

class Koopa : public Enemy{
public:
    //constructor and destructor
    Koopa(int startX, int startY, StudentWorld* sWP);
    virtual ~Koopa();
    
private:
    virtual void doSomethingEnemyDeathAux() override;
};

class Piranha : public Enemy{
public:
    //constructor and destructor
    Piranha(int startX, int startY, StudentWorld* sWP);
    virtual ~Piranha(){}
        
private:
    virtual void doSomethingEnemyAux() override;

    //private member variable
    int m_firingDelay;
};

#endif // ACTOR_H_
