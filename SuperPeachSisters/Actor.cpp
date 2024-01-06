#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, int dir, double depth, StudentWorld* world)
    :GraphObject(imageID, startX, startY, dir, depth), m_world(world), status(true)
{}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

//True for everything except blocks which cant be crossed over
bool Actor::crossover()
{
    return true;
}

//Used to damage actors
void Actor::damaged()
{
    setStatus();
}

//True for everything except enemies
bool Actor::isEnemy()
{
    return false;
}

//Changes status to false and thus dead
void Actor::setStatus()
{
    status = false;
}

//Returns current status
bool Actor::getStatus()
{
    return status;
}

//Peach constructor that uses the actor class
Peach::Peach(int imageID, double startX, double startY, int dir, double depth, StudentWorld* world)
    :Actor(IID_PEACH, startX, startY, 0, 0, world), flowPow(false), invinPow(0), jumPow(false), recharge_time(0), health(1), remaining_jump_distance(0)
{}

void Peach::doSomething()
{
    if(getWorld()->getKey(keyValue))
    {
        move();
    }
    if(remaining_jump_distance > 0)
    {
        if(getWorld()->onGround(getX(), getY()+4))
        {
            getWorld()->bonkActor(getX(), getY()+4);
            remaining_jump_distance = 0;
        } else {
            moveTo(getX(), getY()+4);
            remaining_jump_distance--;
        }
    } else {
        if(!getWorld()->onGround(getX(), getY()-4))
        {
            moveTo(getX(), getY()-4);
        }
    }
    if(getWorld()->Overlap(getX(), getY()))
    {
        getWorld()->bonkActor(getX(), getY());
    }
    if(invinPow > 0)
    {
        invinPow--;
    }
    if(recharge_time > 0)
    {
        recharge_time--;
    }
}

void Peach::move()
{
    switch (keyValue)
    {
        case KEY_PRESS_UP:
            if (getWorld()->onGround(getX(), getY()-1))
            {
                if(jumPow)
                {
                    remaining_jump_distance = 12;
                } else {
                remaining_jump_distance = 8;
                }
                getWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        case KEY_PRESS_LEFT:
        setDirection(180);
            if (!getWorld()->onGround(getX()-4, getY()))
            {
                moveTo(getX()-4, getY());
            } else {
                getWorld()->bonkActor(getX()-4, getY());
            }
            break;
        case KEY_PRESS_RIGHT:
        setDirection(0);
            if (!getWorld()->onGround(getX()+4, getY()))
            {
                moveTo(getX()+4, getY());
            } else {
                getWorld()->bonkActor(getX()+4, getY());
            }
            break;
        case KEY_PRESS_SPACE:
            if(flowPow && recharge_time == 0)
            {
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                if(getDirection() == 0)
                {
                    getWorld()->newActor(new PFireball(getX()+4, getY(), getDirection(), getWorld()));
                } else if (getDirection() == 180)
                {
                    getWorld()->newActor(new PFireball(getX()-4, getY(), getDirection(), getWorld()));
                }
                recharge_time = 8;
            }
            break;
    }
}

void Peach::bonk()
{
    if(invinPow > 0)
    {
        return;
    }
    decHealth();
    flowPow = false;
    jumPow = false;
    if(getHealth() > 0)
    {
        getWorld()->playSound(SOUND_PLAYER_HURT);
    } else {
        getWorld()->decLives();
    }
}

void Peach::decHealth()
{
    health--;
}

void Peach::incHealth()
{
    health++;
}

void Peach::setInvin()
{
    invinPow = 150;
}

void Peach::setFlow()
{
    flowPow = true;
}

void Peach::setJum()
{
    jumPow = true;
}

bool Peach::hasInvin()
{
    if(invinPow > 0)
    {
        return true;
    }
    return false;
}

bool Peach::hasJum()
{
    return jumPow;
}

bool Peach::hasFlow()
{
    return flowPow;
}

int Peach::getHealth()
{
    return health;
}

//Ground constructor that uses the actor class
Ground::Ground(int imageID, double startX, double startY, StudentWorld* world)
    :Actor(imageID, startX, startY, 0, 2, world)
{}

bool Ground::crossover()
{
    return false;
}

//Block contrusctor inherited from Ground
Block::Block(double startX, double startY, StudentWorld* world, int goodie)
    :Ground(IID_BLOCK, startX, startY, world)
{
    ngoodie = goodie;
}

//Blocks bonk method checks to see if the block holds a goodie or not and if yes then creates a goodie above the block
void Block::bonk()
{
    if(ngoodie == 0)
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
        return;
    } else {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        switch (ngoodie)
        {
            case 1:
                getWorld()->newActor(new Star(getX(), getY()+SPRITE_HEIGHT, getWorld()));
                break;
            case 2:
                getWorld()->newActor(new Mushroom(getX(), getY()+SPRITE_HEIGHT, getWorld()));
                break;
            case 3:
                getWorld()->newActor(new Flower(getX(), getY()+SPRITE_HEIGHT, getWorld()));
                break;
        }
        setGoodie();
    }
}

//Changes the goodie to 0 so then the block is empty
void Block::setGoodie()
{
    ngoodie = 0;
}

//Pipe contstructor that is inhertied from ground, this object does nothing
Pipe::Pipe(double startX, double startY, StudentWorld* world)
    :Ground(IID_PIPE, startX, startY, world)
{}

//Goal constructor that creates objects based on actor class
Goal::Goal(int imageID, double startX, double startY, StudentWorld* world)
    :Actor(imageID, startX, startY, 0, 1, world)
{}

//
Flag::Flag(double startX, double startY, StudentWorld* world)
    :Goal(IID_FLAG, startX, startY, world)
{}

void Flag::doSomething()
{
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->setLevel();
    }
}

Mario::Mario(double startX, double startY, StudentWorld* world)
    :Goal(IID_MARIO, startX, startY, world)
{}

void Mario::doSomething()
{
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->increaseScore(1000);
        getWorld()->setGame();
    }
}
 
Enemies::Enemies(int imageID, double startX, double startY, StudentWorld* world)
    :Actor(imageID, startX, startY, 0, 1, world)
{
    if (randInt(0, 1) == 1)
    {
            setDirection(180);
    }
}

void Enemies::doSomething()
{
    move();
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->getPlayer()->bonk();
    }
}

void Enemies::move()
{
    if(getDirection() == 0)
    {
        if(getWorld()->onGround(getX()+1, getY()) || !getWorld()->onGround(getX()+SPRITE_WIDTH, getY()-1))
        {
            setDirection(180);
        }
    }
    if(getDirection() == 180)
    {
        if(getWorld()->onGround(getX()-1, getY()) || !getWorld()->onGround(getX()-SPRITE_WIDTH, getY()-1))
        {
            setDirection(0);
        }
    }
   if(getDirection() == 0)
   {
       if(!getWorld()->onGround(getX()+1, getY()))
       {
           moveTo(getX()+1, getY());
       }
   } else if (getDirection() == 180)
   {
       if(!getWorld()->onGround(getX()-1, getY()))
       {
           moveTo(getX()-1, getY());
       }
   }
}

bool Enemies::isEnemy()
{
    return true;
}

void Enemies::bonk()
{
    if(getWorld()->getPlayer()->hasInvin())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setStatus();
    }
}

void Enemies::damaged()
{
    setStatus();
    getWorld()->increaseScore(100);
}

Goomba::Goomba(double startX, double startY, StudentWorld* world)
    :Enemies(IID_GOOMBA, startX, startY, world)
{}

Koopa::Koopa(double startX, double startY, StudentWorld* world)
    :Enemies(IID_KOOPA, startX, startY, world)
{}


Piranha::Piranha(double startX, double startY, StudentWorld* world)
    :Enemies(IID_PIRANHA, startX, startY, world), recharge_time(0)
{}

void Piranha::doSomething()
{
    increaseAnimationNumber();
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->getPlayer()->bonk();
    }
    double difY = getWorld()->getPlayer()->getY() - getY();
    if(abs(difY) > 1.5*SPRITE_HEIGHT || recharge_time > 0)
    {
        recharge_time--;
        return;
    }
    if (getX() < getWorld()->getPlayer()->getX()) {
            setDirection(0);
        }
        else {
            setDirection(180);
        }
    double difX = getWorld()->getPlayer()->getX() - getX();
    if(abs(difX) < 8*SPRITE_WIDTH)
    {
        getWorld()->newActor(new Fireball(getX(), getY(), getDirection(), getWorld()));
        getWorld()->playSound(SOUND_PIRANHA_FIRE);
        recharge_time = 40;
    }
}

Projectile::Projectile(int imageID, double startX, double startY, int startDir, StudentWorld* world)
    :Actor(imageID, startX, startY, startDir, 1, world)
{}

void Projectile::doSomething()
{
    move();
        if(getWorld()->onEnemy(getX(), getY()) && getWorld()->isAlive(getX(), getY()))
        {
            getWorld()->attackEnemy(getX(), getY());
            damaged();
        }
}

void Projectile::move()
{
    if(!getWorld()->onGround(getX(), getY()-2))
    {
        moveTo(getX(), getY()-2);
    }
    if(getDirection() == 0)
    {
        if(!getWorld()->onGround(getX()+2, getY()))
        {
            moveTo(getX()+2, getY());
        } else {
            setStatus();
        }
    }
    if (getDirection() == 180) {
        if(!getWorld()->onGround(getX()-2, getY()))
        {
            moveTo(getX()-2, getY());
        } else {
            setStatus();
        }
    }
}

PFireball::PFireball(double startX, double startY, int startDir, StudentWorld* world)
    :Projectile(IID_PEACH_FIRE, startX, startY, startDir, world)
{}

Fireball::Fireball(double startX, double startY, int startDir, StudentWorld* world)
    :Projectile(IID_PIRANHA_FIRE, startX, startY, startDir, world)
{}

void Fireball::doSomething()
{
    move();
        if(getWorld()->onPeach(getX(), getY()))
        {
            getWorld()->bonkPeach();
            setStatus();
        }
}

Shell::Shell(double startX, double startY, int startDir, StudentWorld* world)
    :Projectile(IID_SHELL, startX, startY, startDir, world)
{}

PowerUp::PowerUp(int imageID, double startX, double startY, StudentWorld* world)
 :Actor(imageID, startX, startY, 0, 1, world)
{}

void PowerUp::move()
{
    if(!getWorld()->onGround(getX(), getY()-2))
        {
            moveTo(getX(), getY()-2);
        }
    if(getDirection() == 0)
    {
        if(!getWorld()->onGround(getX()+2, getY()))
        {
            moveTo(getX()+2, getY());
        } else {
            setDirection(180);
        }
    } else if (getDirection() == 180)
    {
        if(!getWorld()->onGround(getX()-2, getY()))
        {
            moveTo(getX()-2, getY());
        } else {
        setDirection(0);
        }
    }
}

void PowerUp::Power()
{
    if(getWorld()->getPlayer()->getHealth() < 2)
    {
        getWorld()->getPlayer()->incHealth();
    }
    setStatus();
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
}

Star::Star(double startX, double startY, StudentWorld* world)
 :PowerUp(IID_STAR, startX, startY, world)
{}

void Star::doSomething()
{
    move();
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->getPlayer()->setInvin();
        getWorld()->increaseScore(100);
        Power();
    }
}

Mushroom::Mushroom(double startX, double startY, StudentWorld* world)
 :PowerUp(IID_MUSHROOM, startX, startY, world)
{}

void Mushroom::doSomething()
{
    move();
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->getPlayer()->setJum();
        getWorld()->increaseScore(75);
        Power();
    }
}

Flower::Flower(double startX, double startY, StudentWorld* world)
 :PowerUp(IID_FLOWER, startX, startY, world)
{}

void Flower::doSomething()
{
    move();
    if(getWorld()->onPeach(getX(), getY()))
    {
        getWorld()->getPlayer()->setFlow();
        getWorld()->increaseScore(50);
        Power();
    }
}

