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

//bonk this Actor, parameter says whether bonker is Peach with invincibiity
void Actor::getBonked(bool bonkerIsInvinciblePeach){}

StudentWorld* Actor::sWP() const{
    return m_sWP;
}

//returns whether or not the Actor is alive
bool Actor::isAlive() const{
    return m_alive;
}

//set Actor's alive status to dead
void Actor::setDead(){
    m_alive = false;
}

//an Actor does not impede or block other Actors (unlike Obstacles) by default
bool Actor::impedes() const{
    return false;
}

//an Actor cannot have a Projectile pass through it by default
bool Actor::projectileCanPassThrough() const{
    return false;
}

//do what the spec says happens when damage is inflicted on this Actor
void Actor::sufferDamageIfDamageable(){}

//fall the indicated distance if not blocked
void Actor::fallIfPossible(int dist){
    sWP()->moveIfPossible(this, getX(), getY() - dist);
}

//reverse the direction this actor is facing
void Actor::reverseDirection(){
    setDirection((getDirection() + 180) % 360);
}

//set destx and desty to the coordinates dist pixels away in direction dir from this Actor's position.
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
Peach::Peach(int startX, int startY, StudentWorld* sWP) : Actor(IID_PEACH, startX, startY, sWP), m_hp(1){
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

//Peach will try to decrement tick variables, bonk Enemies, and take input
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
    
    //check if Peach overlaps an Actor
    sWP()->bonkOverlappingActor(this);
    
    
    //check if previously jumped
    if(m_remainingJumpDistance > 0){
        //if Peach can still rise, increase her position and decrement remaining jumping distance
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
                sWP()->moveOrBonk(this, getX() - MOVEMENT_DISTANCE, getY());
                break;
            case KEY_PRESS_RIGHT: //move right
                setDirection(right);
                sWP()->moveOrBonk(this, getX() + MOVEMENT_DISTANCE, getY());
                break;
                //jump
            case KEY_PRESS_UP: //jump if there's an Obstacle below Peach
                //if there's an Obstacle below Peach
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
                
                //otherwise, play the fireball sound, reset the fireball cooldown, and spawn a PeachFireball at a distance in the direction Peach is facing
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

//behavior differentiator
bool Obstacle::impedes() const{
    return true;
}

//empty function that allows instances of Blocks and Pipes to be instantiated/allocated
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
    
    //state that the Block was bonked, play the power up appears sound, and spawn the correct power up above the Block
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

//if the objective overlaps with Peach, it will end the level if it's a Flag or end the game if it's Mario
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

//Goodies will first check if they're overlapping with Peach, giving her a power up if so. they will then try to fall and try to move
void Goodie::doSomethingAux(){
    //if Goodie overlaps with Peach, do specific Goodie behavior, play the power up sound, and kill itself
    if(sWP()->overlapsPeach(this)){
        doSomethingGoodieAux();
        sWP()->playSound(SOUND_PLAYER_POWERUP);
        setDead();
        return;
    }
    
    //try to fall
    fallIfPossible(GOODIE_FALL_DISTANCE);
    
    //calculate next position. if Goodie can't move due to something blocking it, reverse direction, otherwise, move to destination
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

//behavior differentiator
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

//behavior differentiator
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

//behavior differentiator
void Star::doSomethingGoodieAux(){
    sWP()->increaseScore(STAR_SCORE);
    sWP()->grantInvincibility(STAR_TICKS);
}

//////////////////////////////////////////////////////////////////////////////
///Projectile Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* sWP, int dir) : Actor(imageID, startX, startY, sWP, dir, 1){
    if(!sWP->isMovePossible(this, startX, startY)){
        setDead();
        return;
    }
}

//destructor
Projectile::~Projectile(){}

//Projectiles do specific behavior, try to fall, and then try to move if possible
void Projectile::doSomethingAux(){
    //behavior differentiator
    doSomethingProjectileAux();
    
    //try to fall
    fallIfPossible(PROJECTILE_FALL_DISTANCE);
    
    //calculate new position, if the Projectile will be impeded, kill itself, otherwise, move to destination
    int destx = getX();
    int desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), PROJECTILE_MOVEMENT_DISTANCE, destx, desty);
    if(!sWP()->isMovePossible(this, destx, desty)){
        setDead();
        return;
    }
    moveTo(destx, desty);
}

//behavior differentiator lets StudentWorld know that Projectiles can pass through each other
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

//behavior differentiator damages Peach rather than other Actors
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

//behavior differentiator damages Actors rather than Peach
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

//behavior differentiator damages Actors rather than Peach
void Shell::doSomethingProjectileAux(){
    if(sWP()->damageOverlappingActor(this))
        setDead();
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

//an Enemy will do specific Enemy behavior, try to bonk Peach, and if it can move, try to move
void Enemy::doSomethingAux(){
    //Enemy-specific behavior
    doSomethingEnemyAux();

    //try to bonk Peach if she's overlapping
    if(sWP()->bonkOverlappingPeach(this))
        return;
    
    //skip the case where the Enemy is a Piranha
    if(!m_isMobile)
        return;
    
    //calculate destination, and if the destination is impeded by an Obstacle or there are no blocks underneath the new position, reverse position and move, otherwise, move in new destination
    int destx = getX();
    int destXFallCheck = getX();
    int desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE + SPRITE_WIDTH - 1, destXFallCheck, desty);
    if(!sWP()->isMovePossible(this, destx, desty) || sWP()->isMovePossible(this, destXFallCheck, desty - SPRITE_HEIGHT)){
        reverseDirection();
    }
    
    //recalculate position after possibly changing direction
    destx = getX();
    desty = getY();
    convertDirectionAndDistanceToXY(getDirection(), ENEMY_MOVEMENT_DISTANCE, destx, desty);
    moveTo(destx, desty);
}

//an Enemy will take damage if the bonker is an invincible Peach
void Enemy::getBonked(bool bonkerIsInvinciblePeach){
    if(bonkerIsInvinciblePeach){
        sWP()->playSound(SOUND_PLAYER_KICK);
        sufferDamageIfDamageable();
    }
}

//an Enemy will suffer damage by increasing score, doing Enemy-specific behavior, and dying (Enemies only have 1 hp)
void Enemy::sufferDamageIfDamageable(){
    sWP()->increaseScore(ENEMY_SCORE);
    doSomethingEnemyDeathAux();
    setDead();
}

//Enemies by default don't have special death actions (only Koopas do)
void Enemy::doSomethingEnemyDeathAux(){}

//Enemies by default don't do anything besides moving (except Piranhas)
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

//when a Koopa dies, it spawns a shell in the direction it was facing
void Koopa::doSomethingEnemyDeathAux(){
    sWP()->addActor(new Shell(getX(), getY(), sWP(), getDirection()));
}

//////////////////////////////////////////////////////////////////////////////
///Piranha Implementation
//////////////////////////////////////////////////////////////////////////////

//constructor
Piranha::Piranha(int startX, int startY, StudentWorld* sWP) : Enemy(IID_PIRANHA, startX, startY, sWP, false){
    m_firingDelay = 0;
}

//destructor
Piranha::~Piranha(){}

//move to next animation frame. if Peach is within detection distance, face her and try to fire if a PiranhaFireball if it can
void Piranha::doSomethingEnemyAux(){
    //move to next animation frame
    increaseAnimationNumber();
    
    //calculate distance from Peach, skipping next steps if she's not on the same y-level
    int x;
    if(!sWP()->getPeachTargetingInfo(this, PIRANHA_DETECTION_DISTANCE, x))
        return;
    
    //face Peach
    if(x > 0)
        setDirection(right);
    else
        setDirection(left);
    
    //decrease cooldown for shooting fireballs, skipping next steps if there is still a cooldown
    if(m_firingDelay > 0){
        m_firingDelay--;
        return;
    }
    
    //if Peach is within horizontal range, play the fireball sound, spawn a new PiranhaFireball facing the same direction as the Piranha, and set a cooldown
    if(abs(x) < PIRANHA_RANGE){
        sWP()->playSound(SOUND_PIRANHA_FIRE);
        sWP()->addActor(new PiranhaFireball(getX(), getY(), sWP(), getDirection()));
        m_firingDelay = PIRANHA_COOLDOWN;
    }
}
