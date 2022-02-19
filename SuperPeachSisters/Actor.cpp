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

inline
bool Actor::canMove() const{
    return false;
}

inline
bool Actor::impedes() const{
    return false;
}

//////////////////////////////////////////////////////////////////////////////
///MobileActor Implementation
//////////////////////////////////////////////////////////////////////////////


bool MobileActor::canMove() const{
    return true;
}

inline
bool MobileActor::getImpeded(){
    return m_impeded;
}

void MobileActor::setImpeded(bool impeded){
    m_impeded = impeded;
}

bool MobileActor::collides(int x, int y, int l, int h, int x0, int y0, int l0, int h0){
    int X = x + l - 1;
    int Y = y + h - 1;
    int X0 = x0 + l0 - 1;
    int Y0 = y0 + h0 - 1;
        
    return ((x <= x0 && X >= x0) || (X0 >= x && X0 <= X)) &&
    ((y <= y0 && Y >= y0) || (Y0 >= y && Y0 <= Y));
}

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY) : Actor(IID_PEACH, startX, startY, 0, 0, 1.0){
    m_health = 1;
    m_keyIsPressed = false;
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
    
    //check jump
    
    //check falling
    
    //check keystroke
    if(m_keyIsPressed && !getImpeded()){
        int x = getX();
        int y = getY();
        switch(m_key){
            case KEY_PRESS_LEFT:
                setDirection(180);
                x -= 4;
                moveTo(x, y);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                x += 4;
                moveTo(x, y);
                break;
            case KEY_PRESS_UP:
                y += 4;
                moveTo(x, y);
                break;
            case KEY_PRESS_SPACE:
                moveTo(getX(), getY() - 4);
                break;
        }
    }
}

void Peach::bonk(){
    
}

bool Peach::isImpeded(const Actor &other){
    int x = getX();
    int y = getY();
    if(m_keyIsPressed){
        switch(m_key){
            case KEY_PRESS_LEFT:
                x -= 4;
                break;
            case KEY_PRESS_RIGHT:
                x += 4;
                break;
            case KEY_PRESS_UP:
                y += 4;
                break;
            case KEY_PRESS_SPACE:
                y -= 4;
                break;
        }
    }
    return collides(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, other.getX(), other.getY(),SPRITE_WIDTH, SPRITE_HEIGHT);
}

void Peach::setKey(int key){
    m_key = key;
}

void Peach::setKeyIsPressed(bool keyIsPressed){
    m_keyIsPressed = keyIsPressed;
}

//////////////////////////////////////////////////////////////////////////////
///Obstacle Implementation
//////////////////////////////////////////////////////////////////////////////

bool Obstacle::impedes() const{
    return true;
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



