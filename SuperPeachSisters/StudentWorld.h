#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

//Dummy classes
class Peach;
class Actor;

class StudentWorld: public GameWorld
{
public:
    StudentWorld(string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void eraseDead();
    
    bool onGround(double x, double y);
    bool Overlap(double x, double y);
    bool onPeach(double x, double y);
    bool onEnemy(double x, double y);
   
    void bonkActor(double x, double y);
    void bonkPeach();
    
    void newActor(Actor* actor);
    Peach* getPlayer();
    void attackEnemy(double x, double y);
    bool isAlive(double x, double y);
    
    void displayText();
    void setGame();
    void setLevel();
    
private:
    Peach* player;
    vector<Actor*> actors;
    bool gameWon;
    bool levelWon;
};

#endif // STUDENTWORLD_H_
