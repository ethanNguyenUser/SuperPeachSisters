#include "Actor.h"
#include "StudentWorld.h"

//GraphObject functions
//GraphObject(int imageID, int startX, int startY,
//int startDirection = 0, int depth = 0, double size = 1.0);
//double getX() const; // in pixels (0-255)
//double getY() const; // in pixels (0-255)
//void moveTo(double x, double y); // in pixels (0-255)
//int getDirection() const; // in degrees (0-359)
//void setDirection(int d); // in degrees (0-359)
//void increaseAnimationNumber(); // forces a sprite to animate frames

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////////////////////////////////////////////////////////////////////////
///Actor Implementation
//////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir, int depth, double size) : GraphObject(imageID, startX * VIEW_WIDTH / GRID_WIDTH, startY * VIEW_HEIGHT / GRID_HEIGHT, dir, depth, size){
    m_sWP = sWP;
    m_alive = true;
}

inline
StudentWorld* Actor::sWP() const{
    return m_sWP;
}

inline
bool Actor::isAlive() const{
    return m_alive;
}

inline
void Actor::setAlive(){
    m_alive = true;
}

inline
void Actor::setDead(){
    m_alive = false;
}

inline
bool Actor::canMove() const{
    return false;
}

inline
bool Actor::impedes() const{
    return false;
}

inline
bool Actor::damageable() const{
    return false;
}

bool Actor::collides(int x, int y, int x0, int y0){
    int X = x + SPRITE_WIDTH - 1;
    int Y = y + SPRITE_HEIGHT - 1;
    int X0 = x0 + SPRITE_WIDTH - 1;
    int Y0 = y0 + SPRITE_HEIGHT - 1;
//
//    return ((x <= x0 && X >= x0) || (X0 >= x && X0 <= X)) &&
//    ((y <= y0 && Y >= y0) || (Y0 >= y && Y0 <= Y));
    
    return (x <= X0 && X >= x0 && y <= Y0 && Y >= y0);
    
}

//////////////////////////////////////////////////////////////////////////////
///DamageableActor Implementation
//////////////////////////////////////////////////////////////////////////////

bool DamageableActor::damageable() const{
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///MobileActor Implementation
//////////////////////////////////////////////////////////////////////////////

bool MobileActor::canMove() const{
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY, StudentWorld* sWP) : Actor(IID_PEACH, startX, startY, sWP, 4, 0){
    m_health = 1;
}

Peach::~Peach(){
    
}

void Peach::doSomething(){
    //check alive
    if(!isAlive())
        return;
    
    //check if invincible
    if(m_invTick > 0)
        m_invTick--;
    
    //check if temporarily invincible
    if(m_tempInvTick > 0)
        m_tempInvTick--;
    
    //check if in recharge mode
    if(m_fBTick > 0)
        m_fBTick--;
    
    //check collision
    sWP()->checkCollision(getX(), getY(), this);

    int x = getX();
    int y = getY();
    
    //check jump
    if(m_remainingJumpDistance > 0){
        x = getX();
        y = getY() + 4;
        if(sWP()->checkCollision(x, y, this))
            m_remainingJumpDistance = 0;
        else{
            moveTo(x, y);
            m_remainingJumpDistance--;
        }
    }
    
    //check falling
    else if(m_remainingJumpDistance == 0){
        x = getX();
        y = getY() - 4;
        if(!sWP()->checkCollisionFalling(x, y, this))
            moveTo(x, y);
    }

    
    //check keystroke
    int key;
    if(sWP()->getKey(key)){
        x = getX();
        y = getY();
        bool isMoved = false;
        switch(key){
            case KEY_PRESS_LEFT:
                setDirection(180);
                x -= 4;
                isMoved = true;
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                isMoved = true;
                x += 4;
                break;
//            case KEY_PRESS_UP:
//                y += 4;
//                break;
//            case KEY_PRESS_SPACE:
//                y -= 4;
//                break;
            case KEY_PRESS_UP:
                m_remainingJumpDistance = 8;
                isMoved = true;
                break;
            case KEY_PRESS_SPACE:
                y -= 4;
                isMoved = true;
                break;
        }
        bool isImpeded = sWP()->checkCollision(x, y, this);
        if(!isImpeded && isMoved)
            moveTo(x, y);
    }
    
}

void Peach::bonk(){
    if(true)
        true;
}

//////////////////////////////////////////////////////////////////////////////
///Obstacle Implementation
//////////////////////////////////////////////////////////////////////////////

void Obstacle::doSomething(){}

bool Obstacle::impedes() const{
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///Block Implementation
//////////////////////////////////////////////////////////////////////////////

Block::Block(int startX, int startY, StudentWorld* sWP) : Actor(IID_BLOCK, startX, startY, sWP, 0, 2){
    m_wasBonked = false;
}

void Block::bonk(){
    sWP()->playSound(SOUND_PLAYER_BONK);
    std::cerr << "bonk" << std::endl;
    if(!m_wasBonked){
        m_wasBonked = true;
    }
}
    

//////////////////////////////////////////////////////////////////////////////
///Pipe Implementation
//////////////////////////////////////////////////////////////////////////////


Pipe::Pipe(int startX, int startY, StudentWorld* sWP) : Actor(IID_PIPE, startX, startY, sWP, 0, 2){}

void Pipe::bonk(){
}


//////////////////////////////////////////////////////////////////////////////
///Flag Implementation
//////////////////////////////////////////////////////////////////////////////

Flag::Flag(int startX, int startY, StudentWorld* sWP) : Actor(IID_FLAG, startX, startY, sWP, 0, 2){}

void Flag::doSomething(){
    
}

void Flag::bonk(){
    
}

//////////////////////////////////////////////////////////////////////////////
///Mario Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Flower Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Mushroom Implementation
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
///Star Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Piranha Fireball Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Peach Fireball Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Shell Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Goomba Implementation
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
///Koopa mplementation
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
///Piranha Implementation
//////////////////////////////////////////////////////////////////////////////



