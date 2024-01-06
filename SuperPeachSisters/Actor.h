#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int dir, double depth, StudentWorld* world);
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    virtual void move() = 0;
    virtual bool crossover();
    virtual void bonk() {};
    virtual bool isEnemy();
    StudentWorld* getWorld() const;
    void setStatus();
    bool getStatus();
    virtual void damaged();
    virtual bool isKoopa() {return false;}
private:
    bool status;
    StudentWorld* m_world;
};

class Peach: public Actor
{
public:
    Peach(int imageID, double startX, double startY, int dir, double depth, StudentWorld* world);
    virtual ~Peach() {};
    virtual  void doSomething();
    virtual void move();
    virtual void bonk();
    int getHealth();
    void decHealth();
    void incHealth();
    void setInvin();
    void setFlow();
    void setJum();
    bool hasJum();
    bool hasInvin();
    bool hasFlow();
private:
    int health;
    int keyValue;
    int remaining_jump_distance;
    int recharge_time;
    int invinPow;
    bool flowPow;
    bool jumPow;
};

class Ground: public Actor
{
public:
    Ground(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~Ground() {};
    virtual void doSomething() {};
    virtual void move() {};
    virtual bool crossover();
};

class Block: public Ground
{
public:
    Block(double startX, double startY, StudentWorld* world, int goodie);
    virtual ~Block() {};
    virtual void bonk();
    void setGoodie();
private:
    int ngoodie;
};

class Pipe: public Ground
{
public:
    Pipe(double startX, double startY, StudentWorld* world);
    virtual ~Pipe() {};
};

class Goal: public Actor
{
public:
    Goal(int imageID, double startX, double startY, StudentWorld* world);
    virtual void move() {};
    virtual ~Goal() {};
};

class Flag: public Goal
{
public:
    Flag(double startX, double startY, StudentWorld* world);
    virtual ~Flag() {};
    virtual void doSomething();
};

class Mario: public Goal
{
public:
    Mario(double startX, double startY, StudentWorld* world);
    virtual ~Mario() {};
    virtual void doSomething();
};

class Enemies: public Actor
{
public:
    Enemies(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~Enemies() {};
    virtual void move();
    virtual void doSomething();
    virtual bool isEnemy();
    virtual void bonk();
    virtual void damaged();
};

class Goomba: public Enemies
{
public:
    Goomba(double startX, double startY, StudentWorld* world);
    virtual ~Goomba() {};
};

class Piranha: public Enemies
{
public:
    Piranha(double startX, double startY, StudentWorld* world);
    virtual ~Piranha() {};
    virtual void move() {};
    virtual void doSomething();
private:
    int recharge_time;
};

class Koopa: public Enemies
{
public:
    Koopa(double startX, double startY, StudentWorld* world);
    virtual ~Koopa() {};
    virtual bool isKoopa() {return true;}
};

class Projectile: public Actor
{
public:
    Projectile(int imageID, double startX, double startY, int startDir, StudentWorld* world);
    virtual ~Projectile() {};
    virtual void doSomething();
    virtual void move();
};

class PFireball: public Projectile
{
public:
    PFireball(double startX, double startY, int startDir, StudentWorld* world);
    virtual ~PFireball() {};
};

class Fireball: public Projectile
{
public:
    Fireball(double startX, double startY, int startDir, StudentWorld* world);
    virtual ~Fireball() {};
    virtual void doSomething();
};

class Shell: public Projectile
{
public:
    Shell(double startX, double startY, int startDir, StudentWorld* world);
    virtual ~Shell() {};
};

class PowerUp: public Actor
{
public:
    PowerUp(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~PowerUp() {};
    virtual void move();
    void Power();
};

class Flower: public PowerUp
{
public:
    Flower(double startX, double startY, StudentWorld* world);
    virtual ~Flower() {};
    virtual void doSomething();
};

class Mushroom: public PowerUp
{
public:
    Mushroom(double startX, double startY, StudentWorld* world);
    virtual ~Mushroom() {};
    virtual void doSomething();
};

class Star: public PowerUp
{
public:
    Star(double startX, double startY, StudentWorld* world);
    virtual ~Star() {};
    virtual void doSomething();
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


#endif // ACTOR_H_
