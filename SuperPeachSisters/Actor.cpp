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

//constructor
Actor::Actor(int imageID, int startX, int startY, StudentWorld* sWP, int dir, int depth) : GraphObject(imageID, startX, startY, dir, depth, 1){
    m_sWP = sWP;
    m_alive = true;
}

//destructor
Actor::~Actor(){}

//Actor will only try doing it's special behavior if it's alive
void Actor::doSomething(){
    if (m_alive)
        doSomethingAux();
}

//bonk this actor, parameter says whether bonker is Peach with invincibiity
void Actor::getBonked(bool bonkerIsInvinciblePeach){}

StudentWorld* Actor::sWP() const{
    return m_sWP;
}

//returns whether or not the actor is alive
bool Actor::isAlive() const{
    return m_alive;
}

//set Actor's alive status to dead
void Actor::setDead(){
    m_alive = false;
}

//an Actor does not impede or block other actors (unlike Obstacles) by default
bool Actor::impedes() const{
    return false;
}

//an Actor cannot have a Projectile pass through it by default
bool Actor::projectileCanPassThrough() const{
    return false;
}

//do what the spec says happens when damage is inflicted on this actor
void Actor::sufferDamageIfDamageable(){}

//fall the indicated distance if not blocked
void Actor::fallIfPossible(int dist){
    sWP()->moveIfPossible(this, getX(), getY() - dist);
}

//reverse the direction this actor is facing
void Actor::reverseDirection(){
    setDirection((getDirection() + 180) % 360);
}

//set destx and desty to the coordinates dist pixels away in direction dir from this actor's position.
void Actor::convertDirectionAndDistanceToXY(int dir, int dist, int& destx, int& desty) const{
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

//constructor
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

//destructor
Peach::~Peach(){}

void Peach::getBonked(bool bonkerIsInvinciblePeach){
    sufferDamageIfDamageable();
}

//causes Peach to lose HP if she can
void Peach::sufferDamageIfDamageable(){
    //Peach can't take damage if she's invincible
    if(m_tempInvTick > 0 || m_invTick > 0)
        return;
    
    m_hp--;
    
    //if Peach's dead, play the death sound, decrease lives, and set her to the dead state
    if(m_hp < 1){
        sWP()->playSound(SOUND_PLAYER_DIE);
        sWP()->decLives();
        setDead();
        return;
    }
    
    //if Peach got hit but isn't dead because she had a power up, play her hurt sound and reset her power ups to none
    if(m_hp < 2){
        sWP()->playSound(SOUND_PLAYER_HURT);
        m_hasShootPower = false;
        m_hasJumpPower = false;
    }
    
    //if Peach got hurt recently, make her temporarily invulnerable
    if(m_tempInvTick == 0)
        m_tempInvTick = TEMP_INV_TICKS;
}

//grant Peach invincibility for this number of ticks.
void Peach::gainInvincibility(int ticks){
    m_invTick = ticks;
}

//grant Peach shoot power.
void Peach::gainShootPower(){
    m_hp = 2;
    m_hasShootPower = true;
}

//grant Peach jump power.
void Peach::gainJumpPower(){
    m_hp = 2;
    m_hasJumpPower = true;
}

//return whether Peach is invincible
bool Peach::isInvincible() const{
    return m_invTick > 0;
}

//return whether Peach has shoot power
bool Peach::hasShootPower() const{
    return m_hasShootPower;
}

//return whether Peach has jump power
bool Peach::hasJumpPower() const{
    return m_hasJumpPower;
}

//Peach will try to decrement tick variables, bonk enemies, and take input
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
    
    
    //check if previously jumped
    if(m_remainingJumpDistance > 0){
        //if Peach can still rise, increase her positiona and decrement remaining jumping distance
        if(sWP()->moveIfPossible(this, getX(), getY() + JUMP_DISTANCE))
            m_remainingJumpDistance--;
        
        //otherwise, Peach will have bonked her head on something
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
                //move left
            case KEY_PRESS_LEFT: //move left
                setDirection(left);
                sWP()->moveIfPossible(this, getX() - MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_RIGHT: //move right
                setDirection(right);
                sWP()->moveIfPossible(this, getX() + MOVEMENT_DISTANCE, getY());
                break;
                //jump
            case KEY_PRESS_UP: //jump if there's an obstacle below Peach
                //if there's an obstacle below Peach
                if(!sWP()->isMovePossible(this, getX(), getY() - JUMP_HEIGHT_CHECK)){
                    sWP()->playSound(SOUND_PLAYER_JUMP);
                    
                    //remaining jump distance is higher value if she has the jump power, lower value otherwise
                    m_remainingJumpDistance = m_hasJumpPower ? MUSHROOM_JUMP_DISTANCE : NORMAL_JUMP_DISTANCE;
                }
                break;
            case KEY_PRESS_SPACE: //shoot fireball if Peach has the power up
                //if Peach has no shoot power or is on cooldown, don't shoot a fireball
                if(!m_hasShootPower || m_fBTick > 0)
                    break;
                
                //otherwise, play the fireball sound, reset the fireball cooldown, and spawn a fireball at a distance in the direction Peach is facing
                sWP()->playSound(SOUND_PLAYER_FIRE);
                m_fBTick = FB_RECHARGE_TICKS;
                int destx = getX();
                int desty = getY();
                convertDirectionAndDistanceToXY(getDirection(), FIREBALL_SPAWN_DISTANCE, destx, desty);
                sWP()->addActor(new PeachFireball(destx, desty, sWP(), getDirection()));
                break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
///Obstacle Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Obstacle::Obstacle(int imageID, int startX, int startY, StudentWorld* sWP) : Actor(imageID, startX, startY, sWP, right, 2){}

//destructor
Obstacle::~Obstacle(){}


bool Obstacle::impedes() const{
    return true;
}

void Obstacle::doSomethingAux(){}

//////////////////////////////////////////////////////////////////////////////
///Block Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Block::Block(int startX, int startY, StudentWorld* sWP, GoodieType g) : Obstacle(IID_BLOCK, startX, startY, sWP){
    m_g = g;
    m_wasBonked = false;
}

//destructor
Block::~Block(){}

//Block will do nothing if it has no power up inside of it, otherwise, spawn a power up
void Block::getBonked(bool bonkerIsInvinciblePeach){
    //if the Block has nothing or was bonked already, play the regular bonk sound and skip the rest of the steps
    if(m_g == none || m_wasBonked){
        sWP()->playSound(SOUND_PLAYER_BONK);
        return;
    }
    
    //state that the block was bonked, play the power up appears sound, and spawn the correct power up above the block
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

//constructor
Pipe::Pipe(int startX, int startY, StudentWorld* sWP) : Obstacle(IID_PIPE, startX, startY, sWP){}

//destructor
Pipe::~Pipe(){}

//////////////////////////////////////////////////////////////////////////////
///Objective Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Objective::Objective(int startX, int startY, StudentWorld* sWP, bool isGameEnder) : Actor(isGameEnder ? IID_MARIO : IID_FLAG, startX, startY, sWP, right, 1){
    m_isGameEnder = isGameEnder;
}

//destructor
Objective::~Objective(){}

//if the objective overlaps with peach, it will end the level if it's a flag or end the game if it's Mario
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

//constructor
Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* sWP) : Actor(imageID, startX, startY, sWP, right, 1){}

//destructor
Goodie::~Goodie(){}

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
    convertDirectionAndDistanceToXY(getDirection(), GOODIE_MOVEMENT_DISTANCE, destx, desty);
    if(!sWP()->isMovePossible(this, destx, desty)){
        reverseDirection();
        return;
    }
    moveTo(destx, desty);
}

//////////////////////////////////////////////////////////////////////////////
///Flower Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Flower::Flower(int startX, int startY, StudentWorld* sWP) : Goodie(IID_FLOWER, startX, startY, sWP){}

//destructor
Flower::~Flower(){}

void Flower::doSomethingGoodieAux(){
    sWP()->increaseScore(FLOWER_SCORE);
    sWP()->grantShootPower();
}

//////////////////////////////////////////////////////////////////////////////
///Mushroom Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Mushroom::Mushroom(int startX, int startY, StudentWorld* sWP) : Goodie(IID_MUSHROOM, startX, startY, sWP){}

//destructor
Mushroom::~Mushroom(){}

void Mushroom::doSomethingGoodieAux(){
    sWP()->increaseScore(MUSHROOM_SCORE);

    sWP()->grantJumpPower();
}

//////////////////////////////////////////////////////////////////////////////
///Star Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Star::Star(int startX, int startY, StudentWorld* sWP) : Goodie(IID_STAR, startX, startY, sWP){}

//destructor
Star::~Star(){}

void Star::doSomethingGoodieAux(){
    sWP()->increaseScore(STAR_SCORE);
    sWP()->grantInvincibility(STAR_TICKS);
}

//////////////////////////////////////////////////////////////////////////////
///Projectile Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* sWP, int dir) : Actor(imageID, startX, startY, sWP, dir, 1){}

//destructor
Projectile::~Projectile(){}

void Projectile::doSomethingAux(){
    doSomethingProjectileAux();
    
    fallIfPossible(PROJECTILE_FALL_DISTANCE);
    
    int destx = getX();
    int desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), PROJECTILE_MOVEMENT_DISTANCE, destx, desty);
    if(!sWP()->isMovePossible(this, destx, desty)){
        setDead();
        return;
    }
    moveTo(destx, desty);
}

bool Projectile::projectileCanPassThrough() const{
    return true;
}

//////////////////////////////////////////////////////////////////////////////
///Piranha Fireball Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
PiranhaFireball::PiranhaFireball(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_PIRANHA_FIRE, startX, startY, sWP, dir){}

//destructor
PiranhaFireball::~PiranhaFireball(){}

void PiranhaFireball::doSomethingProjectileAux(){
    if(sWP()->damageOverlappingPeach(this))
        setDead();
}


//////////////////////////////////////////////////////////////////////////////
///Peach Fireball Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
PeachFireball::PeachFireball(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_PEACH_FIRE, startX, startY, sWP, dir){}

//destructor
PeachFireball::~PeachFireball(){}

void PeachFireball::doSomethingProjectileAux(){
    if(sWP()->damageOverlappingActor(this))
        setDead();
}

//////////////////////////////////////////////////////////////////////////////
///Shell Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Shell::Shell(int startX, int startY, StudentWorld* sWP, int dir) : Projectile(IID_SHELL, startX, startY, sWP, dir){}

//destructor
Shell::~Shell(){}

void Shell::doSomethingProjectileAux(){
    sWP()->damageOverlappingActor(this);
}

//////////////////////////////////////////////////////////////////////////////
///Enemy Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Enemy::Enemy(int imageID, int startX, int startY, StudentWorld* sWP, bool isMobile) : Actor(imageID, startX, startY, sWP, 180 * randInt(0, 1), 0){
    m_isMobile = isMobile;
}

//destructor
Enemy::~Enemy(){}

void Enemy::doSomethingAux(){
    doSomethingEnemyAux();

    if(sWP()->bonkOverlappingPeach(this))
        return;
    
    if(!m_isMobile)
        return;
    
    int destx = getX();
    int destXFallCheck = getX();
    int desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE + SPRITE_WIDTH - 1, destXFallCheck, desty);
    if(!sWP()->isMovePossible(this, destx, desty) || sWP()->isMovePossible(this, destXFallCheck, desty - SPRITE_HEIGHT)){
        reverseDirection();
    }
    destx = getX();
    desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
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

void Enemy::doSomethingEnemyAux(){}

//////////////////////////////////////////////////////////////////////////////
///Goomba Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Goomba::Goomba(int startX, int startY, StudentWorld* sWP) : Enemy(IID_GOOMBA, startX, startY, sWP, true){}

//destructor
Goomba::~Goomba(){}

//////////////////////////////////////////////////////////////////////////////
///Koopa Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Koopa::Koopa(int startX, int startY, StudentWorld* sWP) : Enemy(IID_KOOPA, startX, startY, sWP, true){}

//destructor
Koopa::~Koopa(){}

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
        sWP()->playSound(SOUND_PIRANHA_FIRE);
        sWP()->addActor(new PiranhaFireball(getX(), getY(), sWP(), getDirection()));
        m_firingDelay = PIRANHA_COOLDOWN;
    }
}
