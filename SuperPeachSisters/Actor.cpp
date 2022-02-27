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

Actor::Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir, int depth) : GraphObject(imageID, startX, startY, dir, depth, 1){
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

StudentWorld* Actor::sWP() const{
    return m_sWP;
}

bool Actor::isAlive() const{
    return m_alive;
}

void Actor::setDead(){
    m_alive = false;
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
        destx += dist;
    else if(dir == left)
        destx -= dist;
    else if(dir == up)
        desty += dist;
    else if(dir == down)
        desty -= dist;
}

//////////////////////////////////////////////////////////////////////////////
///Peach Implementation
//////////////////////////////////////////////////////////////////////////////

Peach::Peach(int startX, int startY, StudentWorld* sWP) : Actor(IID_PEACH, startX, startY, sWP){
    m_hp = 1;
    
    //tick-based variables
    m_invTick = 0;
    m_tempInvTick = 0;
    m_fBTick = 0;
    m_remainingJumpDistance = 0;
    
    //status-based variables
    m_hasShootPower = false;
    m_hasJumpPower = false;
}

void Peach::getBonked(bool bonkerIsInvinciblePeach){
    sufferDamageIfDamageable();
}

void Peach::sufferDamageIfDamageable(){
    setHP(m_hp - 1);
    if(m_hp < 1){
        setDead();
        return;
    }
    if(m_hp < 2){
        m_hasShootPower = false;
        m_hasJumpPower = false;
    }
    
    if(m_tempInvTick == 0)
        m_tempInvTick = TEMP_INV_TICKS;
}

// Set Peach's hit points.
void Peach::setHP(int hp){
    if(hp < m_hp && (m_tempInvTick > 0 || m_invTick > 0))
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
    m_hp = 2;
    m_hasShootPower = true;
}

// Grant Peach Jump Power.
void Peach::gainJumpPower(){
    //TODO: Finish
    m_hp = 2;
    m_hasJumpPower = true;
}

// Is Peach invincible?
bool Peach::isInvincible() const{
    return m_invTick > 0;
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
    
    //check if Peach overlaps an object
    sWP()->bonkOverlappingActor(this);
    
    
    //check jump
    if(m_remainingJumpDistance > 0){
        if(sWP()->moveIfPossible(this, getX(), getY() + JUMP_DISTANCE))
            m_remainingJumpDistance--;
        else{
            m_remainingJumpDistance = 0;
            sWP()->moveOrBonk(this, getX(), getY() + JUMP_DISTANCE);
        }
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
                setDirection(left);
                sWP()->moveIfPossible(this, getX() - MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                sWP()->moveIfPossible(this, getX() + MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_UP:
                if(!sWP()->isMovePossible(this, getX(), getY() - JUMP_HEIGHT_CHECK)){
                    sWP()->playSound(SOUND_PLAYER_JUMP);
                    m_remainingJumpDistance = m_hasJumpPower ? MUSHROOM_JUMP_DISTANCE : NORMAL_JUMP_DISTANCE;
                }
                break;
            case KEY_PRESS_SPACE:
                if(!m_hasShootPower || m_fBTick > 0)
                    break;
                sWP()->playSound(SOUND_PLAYER_FIRE);
                m_fBTick = FB_RECHARGE_TICKS;
                int destx = getX();
                int desty = getY();
                converDirectionAndDistanceToXY(getDirection(), FIREBALL_SPAWN_DISTANCE, destx, desty);
                sWP()->addActor(new PeachFireball(destx, desty, sWP(), getDirection()));
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

void Block::getBonked(bool bonkerIsInvinciblePeach){
    if(m_g == none || m_wasBonked){
        sWP()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    
    m_wasBonked = true;
    sWP()->playSound(SOUND_POWERUP_APPEARS);
    if(m_g == flower)
        sWP()->addActor(new Flower(getX(), getY() + SPRITE_HEIGHT, sWP()));
    else if(m_g == mushroom)
        sWP()->addActor(new Mushroom(getX(), getY() + SPRITE_HEIGHT, sWP()));
    else if(m_g == star)
        sWP()->addActor(new Star(getX(), getY() + SPRITE_HEIGHT, sWP()));
}

//////////////////////////////////////////////////////////////////////////////
///Pipe Implementation
//////////////////////////////////////////////////////////////////////////////


Pipe::Pipe(int startX, int startY, StudentWorld* sWP) : Obstacle(IID_PIPE, startX, startY, sWP){}

void Pipe::getBonked(bool bonkerIsInvinciblePeach){}


//////////////////////////////////////////////////////////////////////////////
///Objective Implementation
//////////////////////////////////////////////////////////////////////////////

Objective::Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder) : Actor(isGameEnder ? IID_MARIO : IID_FLAG, startX, startY, sWP, right, 1){
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
///Goodie Implementation
//////////////////////////////////////////////////////////////////////////////

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* sWP) : Actor(imageID, startX, startY, sWP, right, 1){}

void Goodie::doSomethingAux(){
    if(sWP()->overlapsPeach(this)){
        doSomethingGoodieAux();
        sWP()->playSound(SOUND_PLAYER_POWERUP);
        setDead();
        return;
    }
    
    fallIfPossible(GOODIE_FALL_DISTANCE);
    
    int destx = getX();
    int desty = getY();
    converDirectionAndDistanceToXY(getDirection(), GOODIE_MOVEMENT_DISTANCE, destx, desty);
    if(!sWP()->isMovePossible(this, destx, desty)){
        reverseDirection();
        return;
    }
    moveTo(destx, desty);
}

//////////////////////////////////////////////////////////////////////////////
///Flower Implementation
//////////////////////////////////////////////////////////////////////////////


Flower::Flower(int startX, int startY, StudentWorld* sWP) : Goodie(IID_FLOWER, startX, startY, sWP){
}

void Flower::doSomethingGoodieAux(){
    sWP()->increaseScore(FLOWER_SCORE);
    sWP()->grantShootPower();
}

//////////////////////////////////////////////////////////////////////////////
///Mushroom Implementation
//////////////////////////////////////////////////////////////////////////////

Mushroom::Mushroom(int startX, int startY, StudentWorld* sWP) : Goodie(IID_MUSHROOM, startX, startY, sWP){
}

void Mushroom::doSomethingGoodieAux(){
    sWP()->increaseScore(MUSHROOM_SCORE);
    sWP()->grantJumpPower();
}

//////////////////////////////////////////////////////////////////////////////
///Star Implementation
//////////////////////////////////////////////////////////////////////////////

Star::Star(int startX, int startY, StudentWorld* sWP) : Goodie(IID_STAR, startX, startY, sWP){
}

void Star::doSomethingGoodieAux(){
    sWP()->increaseScore(STAR_SCORE);
    sWP()->grantInvincibility(STAR_TICKS);
}

//////////////////////////////////////////////////////////////////////////////
///Projectile Implementation
//////////////////////////////////////////////////////////////////////////////

Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* sWP, int dir) : Actor(imageID, startX, startY, sWP, dir, 1){}

void Projectile::doSomethingAux(){
    doSomethingProjectileAux();
    
    fallIfPossible(PROJECTILE_FALL_DISTANCE);
    
    int destx = getX();
    int desty = getY();
    converDirectionAndDistanceToXY(getDirection(), PROJECTILE_MOVEMENT_DISTANCE, destx, desty);
    if(!sWP()->isMovePossible(this, destx, desty)){
        setDead();
        return;
    }
    moveTo(destx, desty);
}

//////////////////////////////////////////////////////////////////////////////
///Piranha Fireball Implementation
//////////////////////////////////////////////////////////////////////////////

PiranhaFireball::PiranhaFireball(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_PIRANHA_FIRE, startX, startY, sWP, dir){
}

void PiranhaFireball::doSomethingProjectileAux(){
    if(sWP()->damageOverlappingPeach(this))
        setDead();
}


//////////////////////////////////////////////////////////////////////////////
///Peach Fireball Implementation
//////////////////////////////////////////////////////////////////////////////

PeachFireball::PeachFireball(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_PEACH_FIRE, startX, startY, sWP, dir){
}

void PeachFireball::doSomethingProjectileAux(){
    if(sWP()->damageOverlappingActor(this))
        setDead();
}

//////////////////////////////////////////////////////////////////////////////
///Shell Implementation
//////////////////////////////////////////////////////////////////////////////

Shell::Shell(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_SHELL, startX, startY, sWP, dir){
}

void Shell::doSomethingProjectileAux(){
    sWP()->damageOverlappingActor(this);
}

//////////////////////////////////////////////////////////////////////////////
///Enemy Implementation
//////////////////////////////////////////////////////////////////////////////

Enemy::Enemy(int imageID, int startX, int startY, StudentWorld* sWP, bool isMobile) : Actor(imageID, startX, startY, sWP, 180 * randInt(0, 1), 0){
    m_isMobile = isMobile;
}

void Enemy::doSomethingAux(){
    doSomethingEnemyAux();

    if(sWP()->bonkOverlappingPeach(this))
        return;
    
    if(!m_isMobile)
        return;
    
    int destx = getX();
    int destXFallCheck = getX();
    int desty = getY();
    converDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
    converDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE + SPRITE_WIDTH - 1, destXFallCheck, desty);
    if(!sWP()->isMovePossible(this, destx, desty) || sWP()->isMovePossible(this, destXFallCheck, desty - SPRITE_HEIGHT)){
        reverseDirection();
    }
    destx = getX();
    desty = getY();
    converDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
    moveTo(destx, desty);
}

void Enemy::getBonked(bool bonkerIsInvinciblePeach){
    if(bonkerIsInvinciblePeach){
        sWP()->playSound(SOUND_PLAYER_KICK);
        sufferDamageIfDamageable();
    }
}

void Enemy::sufferDamageIfDamageable(){
    if(!isAlive())
        return;
    sWP()->increaseScore(ENEMY_SCORE);
    doSomethingEnemyDeathAux();
    setDead();
}

void Enemy::doSomethingEnemyDeathAux(){}

//////////////////////////////////////////////////////////////////////////////
///Goomba Implementation
//////////////////////////////////////////////////////////////////////////////

Goomba::Goomba(int startX, int startY, StudentWorld* sWP) : Enemy(IID_GOOMBA, startX, startY, sWP, true){
}

void Goomba::doSomethingEnemyAux(){
    
}

//////////////////////////////////////////////////////////////////////////////
///Koopa Implementation
//////////////////////////////////////////////////////////////////////////////

Koopa::Koopa(int startX, int startY, StudentWorld* sWP) : Enemy(IID_KOOPA, startX, startY, sWP, true){
}

void Koopa::doSomethingEnemyAux(){
    
}

void Koopa::doSomethingEnemyDeathAux(){
    sWP()->addActor(new Shell(getX(), getY(), sWP(), getDirection()));
}

//////////////////////////////////////////////////////////////////////////////
///Piranha Implementation
//////////////////////////////////////////////////////////////////////////////

Piranha::Piranha(int startX, int startY, StudentWorld* sWP) : Enemy(IID_PIRANHA, startX, startY, sWP, false){
    m_firingDelay = 0;
}

void Piranha::doSomethingEnemyAux(){
    increaseAnimationNumber();
    int x;
    if(!sWP()->getPeachTargetingInfo(this, PIRANHA_DETECTION_DISTANCE, x))
        return;
    if(x > 0)
        setDirection(right);
    else
        setDirection(left);
    if(m_firingDelay > 0){
        m_firingDelay--;
        return;
    }
    if(abs(x) < PIRANHA_RANGE){
        sWP()->addActor(new PiranhaFireball(getX(), getY(), sWP(), getDirection()));
        m_firingDelay = PIRANHA_COOLDOWN;
    }
}
