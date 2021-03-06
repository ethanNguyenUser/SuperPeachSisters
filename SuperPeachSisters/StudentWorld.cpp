#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

//GameWorld functions
//int getLevel() const;
//int getLives() const;
//void decLives();
//void incLives();
//int getScore() const;
//void increaseScore(int howMuch);
//void setGameStatText(string text);
//bool getKey(int& value);
//void playSound(int soundID);

GameWorld* createStudentWorld(string assetPath){
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath){
    levelIsEnded = false;
    gameIsEnded = false;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

//initialize game by transferring data from Level object to new Actors in an Actor vector
int StudentWorld::init(){
    //use level file to create Actor objects
    Level lev(assetPath());
    ostringstream oss;
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel();
    oss << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found){
        cerr << "Could not find " << level_file << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format){
        cerr << level_file << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        
        //run through level object grid, creating a new actor and pushing it into the m_actors vector if it exists in the Level grids
        for(int i = 0; i < GRID_WIDTH; i++){
            for(int j = 0; j < GRID_HEIGHT; j++){
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j);
                switch (ge){
                    case Level::empty:
                        break;
                    case Level::peach:
                        m_peach = new Peach(gridToCoord(i), gridToCoord(j), this);
                        break;
                    case Level::block:
                        m_actors.push_back(new Block(gridToCoord(i), gridToCoord(j), this, Block::none));
                        break;
                    case Level::pipe:
                        m_actors.push_back(new Pipe(gridToCoord(i), gridToCoord(j), this));
                        break;
                    case Level::flag:
                        m_actors.push_back(new Objective(gridToCoord(i), gridToCoord(j), this, false));
                        break;
                    case Level::mario:
                        m_actors.push_back(new Objective(gridToCoord(i), gridToCoord(j), this, true));
                        break;
                    case Level::flower_goodie_block:
                        m_actors.push_back(new Block(gridToCoord(i), gridToCoord(j), this, Block::flower));
                        break;
                    case Level::mushroom_goodie_block:
                        m_actors.push_back(new Block(gridToCoord(i), gridToCoord(j), this, Block::mushroom));
                        break;
                    case Level::star_goodie_block:
                        m_actors.push_back(new Block(gridToCoord(i), gridToCoord(j), this, Block::star));
                        break;
                    case Level::goomba:
                        m_actors.push_back(new Goomba(gridToCoord(i), gridToCoord(j), this));
                        break;
                    case Level::koopa:
                        m_actors.push_back(new Koopa(gridToCoord(i), gridToCoord(j), this));
                        break;
                    case Level::piranha:
                        m_actors.push_back(new Piranha(gridToCoord(i), gridToCoord(j), this));
                        break;
                }
            }
        }
    }
            
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // The term "actors" refers to all actors, e.g., Peach, goodies,
    // enemies, flags, blocks, pipes, fireballs, etc.
    // Give each actor a chance to do something, incl. Peach
   
    //for all actors, call their doSomething() methods, breaking out from the control flow if Peach dies, the level is finished, or if the game is finished
    for(int i = 0; i < m_actors.size(); i++){
        Actor* actor = m_actors[i];
        if (actor->isAlive()){
            // tell that actor to do something (e.g. move)
            m_actors[i]->doSomething();
            
            if(levelIsEnded){
                levelIsEnded = false;
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
            
            if(gameIsEnded){
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
            
            if(!m_peach->isAlive()){
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    //Peach actions
    m_peach->doSomething();

    //remove newly-dead actors after each tick
    for(int i = 0; i < m_actors.size(); i++){
        if(!m_actors[i]->isAlive()){
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
    }
    
    //update game text at top
    updateGameText();
    
    //resume game
    return GWSTATUS_CONTINUE_GAME;
}

//delete Peach and remove any remaining actors in the m_actors vector
void StudentWorld::cleanUp(){
    delete m_peach;
    for(int i = 0; i < m_actors.size(); i++){
        delete m_actors[i];
    }
    m_actors.clear();
}

//add an actor to the world
void StudentWorld::addActor(Actor* a){
    m_actors.push_back(a);
}

//record that a level was finished. the parameter is true if it was the last level
void StudentWorld::endLevel(bool isGameWon){
    if(isGameWon)
        gameIsEnded = true;
    else
        levelIsEnded = true;
}

//if a can move to (destx, desty), move it and return true, otherwise bonk the object that's blocking it and return false
bool StudentWorld::moveOrBonk(Actor *a, int destx, int desty) const{
    for(int i = 0; i < m_actors.size(); i++){
        Actor* temp = m_actors[i];
        if(temp->impedes() && collides(temp->getX(), temp->getY(), destx, desty)){
            temp->getBonked(false);
            return false;
        }
    }
    a->moveTo(destx, desty);
    return true;
}

//if a can move to (destx, desty), move it and return true, otherwise, return false
bool StudentWorld::moveIfPossible(Actor* a, int destx, int desty) const{
    if(!isMovePossible(a, destx, desty))
        return false;
    a->moveTo(destx, desty);
    return true;
}

//if a can move to (destx, destx), return true (but don't move it), otherwise (it would be blocked), return false
bool StudentWorld::isMovePossible(Actor* a, int destx, int desty) const{
    for(int i = 0; i < m_actors.size(); i++){
        Actor* temp = m_actors[i];
        if(temp->impedes() && collides(temp->getX(), temp->getY(), destx, desty))
            return false;
    }
    return true;
}

//if Peach overlaps bonker, bonk her and return true, otherwise, return false
bool StudentWorld::bonkOverlappingPeach(Actor* bonker) const{
    if(overlapsPeach(bonker)){
        m_peach->getBonked(false);
        return true;
    }
    return false;
}

//if a non-Peach actor overlaps bonker, bonk that non-Peach actor and return true, otherwise, return false (the bonker will be Peach)
void StudentWorld::bonkOverlappingActor(Peach* bonker) const{
    for(int i = 0; i < m_actors.size(); i++){
        Actor* temp = m_actors[i];
        if(collides(temp->getX(), temp->getY(), bonker->getX(), bonker->getY())){
            temp->getBonked(bonker->isInvincible());
        }
    }
}

//if Peach overlaps damager, damage her and return true, otherwise, return false
bool StudentWorld::damageOverlappingPeach(Actor* damager) const{
    if(overlapsPeach(damager)){
        m_peach->sufferDamageIfDamageable();
        return true;
    }
    return false;
}

//if a non-Peach actor overlaps damager, damage that non-Peach actor and return true, otherwise, return false
bool StudentWorld::damageOverlappingActor(Actor* damager) const{
    for(int i = 0; i < m_actors.size(); i++){
        Actor* temp = m_actors[i];
        if(temp == damager || temp->projectileCanPassThrough() || !temp->isAlive())
            continue;
        if(collides(temp->getX(), temp->getY(), damager->getX(), damager->getY())){
            temp->sufferDamageIfDamageable();
            return true;
        }
    }
    return false;
}

//return true if a overlaps Peach, otherwise, return false.
bool StudentWorld::overlapsPeach(Actor* a) const{
    return collides(a->getX(), a->getY(), m_peach->getX(), m_peach->getY());
}

//if the y cooodinates of a and Peach are at least yDeltaLimit apart, return false, otherwise, set xDeltaFromActor to the difference between Peach's and a's x coordinates (positive means Peach is to the right of a) and return true
bool StudentWorld::getPeachTargetingInfo(Actor* a, int yDeltaLimit, int& xDeltaFromActor) const{
    if(abs(m_peach->getY() - a->getY()) >= yDeltaLimit)
        return false;
    xDeltaFromActor = m_peach->getX() - a->getX();
    return true;
}

//grant Peach invincibility for this number of ticks
void StudentWorld::grantInvincibility(int ticks) const{
    m_peach->gainInvincibility(ticks);
}

//grant Peach Shoot Power
void StudentWorld::grantShootPower() const{
    m_peach->gainShootPower();
}

//grant Peach Jump Power
void StudentWorld::grantJumpPower() const{
    m_peach->gainJumpPower();
}

//takes two x and y positions of two Actors and determines if they overlap
bool StudentWorld::collides(int x, int y, int x0, int y0) const{
    int X = x + SPRITE_WIDTH - 1;
    int Y = y + SPRITE_HEIGHT - 1;
    int X0 = x0 + SPRITE_WIDTH - 1;
    int Y0 = y0 + SPRITE_HEIGHT - 1;
    
    return x <= X0 && X >= x0 && y <= Y0 && Y >= y0;
}

//update the game text in this format: Lives: 3  Level: 01  Points: 000050 StarPower! ShootPower! JumpPower!
void StudentWorld::updateGameText(){
    ostringstream oss;
    oss << "Lives: " << getLives();
    oss << "  Level: ";
    oss.fill('0');
    oss << setw(2) << getLevel();
    oss << "  Points: ";
    oss.fill('0');
    oss << setw(6) << getScore();
    if(m_peach->isInvincible())
        oss << " StarPower!";
    if(m_peach->hasShootPower())
        oss << " ShootPower!";
    if(m_peach->hasJumpPower())
        oss << " JumpPower!";
    setGameStatText(oss.str());
}

//converts grid position from the Level object to coordinates
int StudentWorld::gridToCoord(int grid) const{
    return grid * VIEW_WIDTH / GRID_WIDTH;
}
