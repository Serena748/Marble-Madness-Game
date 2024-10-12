#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

#include <sstream>
#include <iomanip>

#include <list>
#include "Actor.h"

class GraphObject;
class Avatar;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool damageWProjectile(int X, int Y, Actor* ptr);
    bool overlapActor(int X, int Y, Actor* ptr);
    bool overlapActorNoPeaNoPitNoExitNoGoodies(int X, int Y, Actor* ptr);
    Actor* overlapActorMarble(int X, int Y, Actor* ptr);
    Actor* overlapActorPit(int X, int Y, Actor* ptr);
    Actor* overlapActorPea(int X, int Y, Actor* ptr);
    ThiefBot* overlapActorThiefBot(int X, int Y, Actor* ptr);
    bool overlapAvatar(int X, int Y);
    void displayGameText();
    Avatar* getAvatar() {
        return m_player;
    }
    bool getCompleteLevel() const {
        return m_completeLevel;
    }
    void setCompleteLevel(bool status) {
        m_completeLevel = status;
    }
    void setLevelBonus(int new_levelBonus) {
        levelBonus = new_levelBonus;
    }
    bool finishGame() const {
        return m_finishGame;
    }
    void setFinishGame(bool status) {
            m_finishGame = status;
        }
    bool playerDiedDuringThisTick();
    bool playerCompletedTheCurrentLevel();
    void reduceLevelBonusByOne();
    int getCurrentLevelBonus() const;
    bool checkActorsOverlap(int X1, int Y1, int X2, int Y2);
    void createProjectile(int X, int Y, int direction);
    void createThiefBot(int X, int Y, int direction, bool isMean);
    int getNumCrystals() const {
        return numCrystals;
    }
    void setNumCrystals(int value) {
        numCrystals = value;
    }
    void spawnThiefBot(ThiefBot* thiefbot);
private:
    Avatar* m_player;
    std::list<Actor*> actors;
    int numCrystals = 0;
    bool m_completeLevel = false;
    bool m_finishGame = false;
    int levelBonus = 1000;
};

#endif // STUDENTWORLD_H_
