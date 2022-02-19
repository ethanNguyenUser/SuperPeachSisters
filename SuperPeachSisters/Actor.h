#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
    
private:
};

class Peach: public Actor{
public:
    
private:
};

//Block or pipe
class Obstacle: public Actor{
public:
    
private:
};

class Block: public Obstacle{
public:
    
private:
};

class Pipe: public Obstacle{
public:
    
private:
};

//Mario or Flag
class Objective: public Actor{
public:
    
private:
};

class Mario: public Objective{
public:
    
private:
};

class Flag: public Objective{
public:
    
private:
};

class Enemy: public Actor{
public:
    
private:
};

class Goodie: public Actor{
public:
    
private:
};

class Flower: public Goodie{
public:
    
private:
};

class Mushroom: public Goodie{
public:
    
private:
};

class Star: public Goodie{
public:
    
private:
};

class Weapon: public Actor{
public:
    
private:
};

class PeachWeapon: public Weapon{
public:
    
private:
};

class PiranhaFireball: public Weapon{
public:
    
private:
};

class PeachFireball: public PeachWeapon{
public:
    
private:
};

class Shell: public PeachWeapon{
public:
    
private:
};

class MobileEnemy: public Enemy{
public:
    
private:
};

class Goomba: public MobileEnemy{
public:
    
private:
};

class Koopa: public MobileEnemy{
public:
    
private:
};

class Piranha: public Enemy{
public:
    
private:
};

#endif // ACTOR_H_
