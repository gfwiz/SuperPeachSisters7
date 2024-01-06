#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include "Actor.h"
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

// Calls clean up to delete all objects and end the game
StudentWorld::~StudentWorld()
{
    cleanUp();
}

//Creates all items to construct the game and loads the level file from assets
int StudentWorld::init()
{
    gameWon = false;
    levelWon = false;
    Level lev(assetPath());
    ostringstream curLev;
    curLev.setf(ios::fixed);
    curLev.precision(2);
    curLev << "level0" << getLevel()  << ".txt";
    string level_file = curLev.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
            for (int i = 0; i < GRID_WIDTH; i++)
            {
                for (int k = 0; k < GRID_HEIGHT; k++)
                {
                    Level::GridEntry ge;
                    ge = lev.getContentsOf(i, k); // x=5, y=10
                    //Creating all objects
                    switch (ge)
                    {
                        case Level::empty:
                               break;
                        case Level::flag:
                            actors.push_back(new Flag(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::mario:
                            actors.push_back(new Mario(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::koopa:
                            actors.push_back(new Koopa(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::piranha:
                            actors.push_back(new Piranha(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::goomba:
                            actors.push_back(new Goomba(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::peach:
                            player = new Peach(IID_PEACH, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, 0, 1, this);
                            break;
                        case Level::pipe:
                        actors.push_back(new Pipe(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this));
                            break;
                        case Level::block:
                            actors.push_back(new Block(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this, 0));
                            break;
                        case Level::star_goodie_block:
                            actors.push_back(new Block(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this, 1));
                            break;
                        case Level::mushroom_goodie_block:
                            actors.push_back(new Block(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this, 2));
                            break;
                        case Level::flower_goodie_block:
                            actors.push_back(new Block(i*SPRITE_WIDTH, k*SPRITE_HEIGHT, this, 3));
                            break;
                    }
                }
            }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

// Runs every objects doSomething methods if they are alive and displays text as well as checks if the game or level was won
int StudentWorld::move()
{
    displayText();
    player->doSomething();
    eraseDead();
    int i = 0;
    while(i < actors.size())
    {
        if(actors[i]->getStatus() && player->getHealth() > 0)
        {
            actors[i]->doSomething();
            if(player->getHealth() == 0)
            {
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
        i++;
    }
    if(gameWon)
    {
        return GWSTATUS_PLAYER_WON;
    }
    if(levelWon)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

//Deletes all actors and player
void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end())
    {
        delete (*it);
        it = actors.erase(it);
    }
    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }
}

//Function to check if there is a block or pipe at a certain position given with x and y
bool StudentWorld::onGround(double x, double y)
{
    vector<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        //Method I use a lot determine if an actor is in the same block as another using difX an difY
        double difX = x - (*it)->getX();
        double difY = y - (*it)->getY();
        if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT && !(*it)->crossover())
        {
            return true;
        }
        it++;
    }
    return false;
}

//Function to check if there is another actor at position given with x and y
bool StudentWorld::Overlap(double x, double y)
{
    vector<Actor*>::iterator it = actors.begin();
    while(it != actors.end())
    {
        double difX = x - (*it)->getX();
        double difY = y - (*it)->getY();
        if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT)
        {
            return true;
        }
        it++;
    }
    return false;
}
 
//Function to check if there is an enemy at a certain position given with x and y
bool StudentWorld::onEnemy(double x, double y)
{
    for(int i = 0; i < actors.size(); i++)
    {
        double difX = x - actors[i]->getX();
        double difY = y - actors[i]->getY();
        if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT && actors[i]->isEnemy())
        {
            return true;
        }
    }
    return false;
}

//Function to check if peach is at a certain position given with x and y
bool StudentWorld::onPeach(double x, double y)
{
    double difX = x - player->getX();
    double difY = y - player->getY();
    if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT)
    {
        return true;
    }
    return false;
}

//Attacks the enemy actor at that specific x and y coordinate
void StudentWorld::attackEnemy(double x, double y)
{
    for(int i = 0; i < actors.size(); i++)
    {
        double difX = x - actors[i]->getX();
        double difY = y - actors[i]->getY();
        if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT)
        {
            actors[i]->damaged();
        }
    }
}

//Used to create new actors via the given parameter
void StudentWorld::newActor(Actor *actor)
{
    actors.push_back(actor);}

//An overall bonk method used to bonk an actor and that specific coord
void StudentWorld::bonkActor(double x, double y)
{
    vector<Actor*>::iterator it = actors.begin();
        while(it != actors.end())
        {
            double difX = x - (*it)->getX();
            double difY = y - (*it)->getY();
            if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT)
            {
                (*it)->bonk();
            }
            it++;
        }
}

//Bonks peach
void StudentWorld::bonkPeach()
{
    player->bonk();
}
  
//Returns a pointer to peach
Peach* StudentWorld::getPlayer()
{
    return player;
}

//Checks if the object is alive at that block
bool StudentWorld::isAlive(double x, double y)
{
    for(int i = 0; i < actors.size(); i++)
    {
        double difX = x - actors[i]->getX();
        double difY = y - actors[i]->getY();
        if(abs(difX) < SPRITE_WIDTH && abs(difY) < SPRITE_HEIGHT)
        {
            return true;
        }
    }
    return false;
}

//Used to delete an object once it has died and is called every tick in move()
void StudentWorld::eraseDead()
{
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end())
    {
        if(!(*it)->getStatus())
        {
            //if a koopa dies, create a shell
            if((*it)->isKoopa())
           {
                newActor(new Shell((*it)->getX(), (*it)->getY(), (*it)->getDirection(), this));
            }
            delete (*it);
            it = actors.erase(it);
        } else {
            it++;
        }
    }
}

//Using ossstreams this displays the text at the top of level
void StudentWorld::displayText()
{
    ostringstream display;
    display.fill(' ');
    display << "Lives: " << setw(2) << getLives() << "  ";
    display.fill('0');
    display << "Level: " << setw(2) << getLevel() << "  ";
    display.fill('0');
    display << "Points: " << setw(6) << getScore();
    display.fill(' ');
    if (player->hasInvin()) {
        display << " StarPower!";
    }
    if (player->hasFlow()) {
        display << " ShootPower!";
    }
    if (player->hasJum()) {
        display << " JumpPower!";
    }
    setGameStatText(display.str());
}

//Used to set the game as won
void StudentWorld::setGame()
{
    gameWon = true;
}

//used to change levels when won
void StudentWorld::setLevel()
{
    levelWon = true;
}


