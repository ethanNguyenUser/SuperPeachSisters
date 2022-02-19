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

Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size) : GraphObject(imageID, startX * VIEW_WIDTH / GRID_WIDTH, startY * VIEW_HEIGHT / GRID_HEIGHT, dir, depth, size){
    m_alive = true;
}

inline
bool Actor::isAlive(){
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

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY) : Actor(IID_PEACH, startX, startY, 0, 0, 1.0){
    m_health = 1;
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
    
    //check jump
    
    //check falling
    
    //check keystroke
    
}

void Peach::bonk(){
    
}

//////////////////////////////////////////////////////////////////////////////
///Block Implementation
//////////////////////////////////////////////////////////////////////////////

Block::Block(int startX, int startY) : Actor(IID_BLOCK, startX, startY, 0, 2, 1.0){}

void Block::doSomething(){
    
}

void Block::bonk(){
    
}


//////////////////////////////////////////////////////////////////////////////
///Pipe Implementation
//////////////////////////////////////////////////////////////////////////////


Pipe::Pipe(int startX, int startY) : Actor(IID_PIPE, startX, startY, 0, 2, 1.0){}

void Pipe::doSomething(){
    
}

void Pipe::bonk(){
    
}


//////////////////////////////////////////////////////////////////////////////
///Flag Implementation
//////////////////////////////////////////////////////////////////////////////

Flag::Flag(int startX, int startY) : Actor(IID_FLAG, startX, startY, 0, 2, 1.0){}

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



