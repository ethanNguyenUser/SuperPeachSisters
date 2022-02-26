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

Actor::Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir, int depth) : GraphObject(imageID, startX * VIEW_WIDTH / GRID_WIDTH, startY * VIEW_HEIGHT / GRID_HEIGHT, dir, depth, 1){
    m_sWP = sWP;
    m_alive = true;
}

inline
void Actor::doSomething(){
//    if (m_alive)
//        doSomethingAux();
}

// Bonk this actor.  Parameter says whether bonker is Peach with invincibiity.
//inline
inline
void Actor::getBonked(bool bonkerIsInvinciblePeach){
    //TODO: Finish
}

inline
void Actor::bonk(){}

inline
StudentWorld* Actor::sWP() const{
    return m_sWP;
}

inline
bool Actor::isAlive() const{
    return m_alive;
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

// Do what the spec says happens when damage is inflicted on this actor.
inline
void Actor::sufferDamageIfDamageable(){
    //TODO: Finish
}

// Fall the indicated distance if not blocked.
inline
void Actor::fallIfPossible(int dist){
    //TODO: Finish
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

// Reverse the direction this actor is facing.
void Actor::reverseDirection(){
    //TODO: Finish
}

// Set destx and desty to the coordinates dist pixels away in direction
// dir from this actor's position.
void Actor::converDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const{
    //TODO: Finish
}

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY, StudentWorld* sWP) : Actor(IID_PEACH, startX, startY, sWP){
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

inline
void Peach::getBonked(bool bonkerIsInvinciblePeach){
    //TODO: Finish
}

inline
void Peach::sufferDamageIfDamageable(){
    //TODO: Finish
}

void Peach::bonk(){
}

bool Peach::canMove() const{
    return true;
}

// Set Peach's hit points.
void setHP(int hp){
    
}

// Grant Peach invincibility for this number of ticks.
void gainInvincibility(int ticks){
    //TODO: Finish
}

// Grant Peach Shoot Power.
void gainShootPower(){
    //TODO: Finish
}

// Grant Peach Jump Power.
void gainJumpPower(){
    //TODO: Finish
}

// Is Peach invincible?
bool Peach::isInvincible() const{
    //TODO: Finish
    return true;
}

// Does Peach have Shoot Power?
bool Peach::hasShootPower() const{
    //TODO: Finish
    return true;
}

// Does Peach have Jump Power?
bool Peach::hasJumpPower() const{
    //TODO: Finish
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///Obstacle Implementation
//////////////////////////////////////////////////////////////////////////////

Obstacle::Obstacle(int imageID, int startX, int startY, StudentWorld* sWP) : Actor(imageID, startX, startY, sWP, right, 2){}

void Obstacle::doSomething(){}

bool Obstacle::impedes() const{
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///Block Implementation
//////////////////////////////////////////////////////////////////////////////

Block::Block(int startX, int startY, StudentWorld* sWP, GoodieType g) : Obstacle(IID_BLOCK, startX, startY, sWP){
    m_g = g;
    m_wasBonked = false;
}

void Block::bonk(){
    std::cerr << "bonk\n";
    sWP()->playSound(SOUND_PLAYER_BONK);
    if(!m_wasBonked)
        m_wasBonked = true;
}
    

//////////////////////////////////////////////////////////////////////////////
///Pipe Implementation
//////////////////////////////////////////////////////////////////////////////


Pipe::Pipe(int startX, int startY, StudentWorld* sWP) : Obstacle(IID_PIPE, startX, startY, sWP){}

void Pipe::bonk(){
}


//////////////////////////////////////////////////////////////////////////////
///Objective Implementation
//////////////////////////////////////////////////////////////////////////////

Objective::Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder) : Actor(isGameEnder ? IID_MARIO : IID_FLAG, startX, startY, sWP){
    m_isGameEnder = isGameEnder;
}

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



