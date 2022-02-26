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

void Actor::doSomething(){
    if (m_alive)
        doSomethingAux();
}

// Bonk this actor.  Parameter says whether bonker is Peach with invincibiity.
void Actor::getBonked(bool bonkerIsInvinciblePeach){
    //TODO: Finish
}

void Actor::bonk(){}

StudentWorld* Actor::sWP() const{
    return m_sWP;
}

bool Actor::isAlive() const{
    return m_alive;
}

void Actor::setDead(){
    m_alive = false;
}

bool Actor::canMove() const{
    return false;
}

bool Actor::impedes() const{
    return false;
}

// Do what the spec says happens when damage is inflicted on this actor.
void Actor::sufferDamageIfDamageable(){}

// Fall the indicated distance if not blocked.
void Actor::fallIfPossible(int dist){
    sWP()->moveIfPossible(this, getX(), getY() - dist);
}

// Reverse the direction this actor is facing.
void Actor::reverseDirection(){
    setDirection((getDirection() + 180) % 360);
}

// Set destx and desty to the coordinates dist pixels away in direction
// dir from this actor's position.
void Actor::converDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const{
    if(dir == right)
        destx += dir;
    else if(dir == left)
        destx -= dir;
    else if(dir == up)
        desty += dir;
    else if(dir == down)
        desty -= dir;
}

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY, StudentWorld* sWP) : Actor(IID_PEACH, startX, startY, sWP){
    m_hp = 1;
}

void Peach::getBonked(bool bonkerIsInvinciblePeach){
    //TODO: Finish
}

void Peach::sufferDamageIfDamageable(){
    setHP(m_hp - 1);
    if(m_hp < 1)
        setDead();
}

void Peach::bonk(){
}

bool Peach::canMove() const{
    return true;
}

// Set Peach's hit points.
void Peach::setHP(int hp){
    if(m_tempInvTick > 0 || m_invTick > 0)
        return;
    m_hp = hp;
}

int Peach::getHP() const{
    return m_hp;
}

// Grant Peach invincibility for this number of ticks.
void Peach::gainInvincibility(int ticks){
    m_invTick = ticks;
}

// Grant Peach Shoot Power.
void Peach::gainShootPower(){
    
}

// Grant Peach Jump Power.
void Peach::gainJumpPower(){
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

void Peach::doSomethingAux(){
    //check if invincible
    if(m_invTick > 0)
        m_invTick--;
    
    //check if temporarily invincible
    if(m_tempInvTick > 0)
        m_tempInvTick--;
    
    //check if in recharge mode
    if(m_fBTick > 0)
        m_fBTick--;
    
    //check jump
    if(m_remainingJumpDistance > 0){
        if(!sWP()->moveOrBonk(this, getX(), getY() + JUMP_DISTANCE))
            m_remainingJumpDistance = 0;
        else
            m_remainingJumpDistance--;
    }
    
    //check falling
    else if(m_remainingJumpDistance == 0){
        fallIfPossible(FALL_DISTANCE);
    }
    
    //check keystroke
    int key;
    if(sWP()->getKey(key)){
        switch(key){
            case KEY_PRESS_LEFT:
                setDirection(180);
                sWP()->moveIfPossible(this, getX() - MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                sWP()->moveIfPossible(this, getX() + MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_UP:
                if(!sWP()->isMovePossible(this, getX(), getY() - JUMP_HEIGHT_CHECK))
                    m_remainingJumpDistance = 8;
                break;
            case KEY_PRESS_SPACE:
                break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
///Obstacle Implementation
//////////////////////////////////////////////////////////////////////////////

Obstacle::Obstacle(int imageID, int startX, int startY, StudentWorld* sWP) : Actor(imageID, startX, startY, sWP, right, 2){}

bool Obstacle::impedes() const{
    return true;
}

void Obstacle::doSomethingAux(){}

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

void Objective::doSomethingAux(){
    if(sWP()->overlapsPeach(this)){
        sWP()->increaseScore(LEVEL_CLEARANCE_SCORE);
        setDead();
        
        if(m_isGameEnder)
            sWP()->endLevel(true);
        else
            sWP()->endLevel(false);
    }
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



