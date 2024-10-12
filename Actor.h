#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject { //Abstract Base Class every other actor is derived from
public:
    Actor(int imageID, int startX, int startY, int dir, double size, StudentWorld* world);
    StudentWorld* returnWorld();
    bool isAlive() const{
        return m_isAlive;
    }
    void setAliveStatus(bool status) {
        m_isAlive = status;
    }
    virtual bool canOverlap() const {
        return false;
    }
    virtual bool damagable() const {
        return false;
    }
    virtual void setHealth(int n) {
        m_Health = n;
    }
    virtual void setMaxHealth(int n) {
        m_MaxHealth = n;
    }
    int getMaxHealth() {
        return m_MaxHealth;
    }
    int getHealth() {
        return m_Health;
    }
    
    virtual void doSomething() = 0;
    virtual bool canMakeRobotSounds() const { return false; }
    virtual bool canBePickedUp() const { return false; }
    virtual bool canBeShot() const { return false; }
    virtual bool isMovable() const { return false; }
    virtual bool canSwallowMarbles() const { return false; }
    virtual bool canStealGoodies() const { return false; }
    virtual bool isEnemy() const { return false; }
    virtual bool canGetToNextLevel() const { return false; }
private:
    StudentWorld* m_studentWorld;
    int m_Health;
    int m_MaxHealth;
    bool m_isAlive = true;
    
};
class Exit : public Actor {
public:
    Exit(int startX, int startY, StudentWorld* world);
    virtual bool canOverlap() const {
        return openExit;
    }
    void setOverlap(bool new_openExit) {
        openExit = new_openExit;
    }
    virtual bool canGetToNextLevel() const { return true; }
    virtual void doSomething();
private:
    bool openExit = false;
};
class Crystal : public Actor {
public:
    Crystal(int startX, int startY, StudentWorld* world);
    virtual bool canOverlap() const {
        return true;
    }
    virtual void doSomething();
};
class Pit : public Actor {
public:
    Pit(int startX, int startY, StudentWorld* world);
    virtual bool canOverlap() const override {
        return pitMarbled;
    }
    void setOverlap(bool new_pitMarbled) {
        pitMarbled = new_pitMarbled;
    }
    virtual void doSomething() override;
    bool canSwallowMarbles() const override { return true; }
private:
    bool pitMarbled = false;
};
class Wall : public Actor {
public:
    Wall(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool canOverlap() const {
        return false;
    }
};
class Avatar : public Actor {
public:
    Avatar(int startX, int startY, StudentWorld* world);

    int getAmmo() {
        return playerAmmo;
    }
    void setAmmo(int new_ammo) {
        playerAmmo = new_ammo;
    }
    
    virtual bool damagable() const {
        return true;
    }
    virtual void doSomething();
private:

    int playerAmmo = 20;
};
class Pea : public Actor {
public:
    Pea(int imageID, int startX, int startY, int direction, StudentWorld* world);

    void moveProjectile();
    void setDirection(int newDirection) {
        pea_direction = newDirection;
    }
    bool canBeShot() const override { return true; } 

    virtual void doSomething() override;
private:
    int pea_direction;
};
class Marble : public Actor {
public:
    Marble(int startX, int startY, StudentWorld* world);
    virtual void doSomething() override;
    virtual bool damagable() const override{
        return true;
    }
    virtual bool canOverlap() const override{
        return false;
    }
    bool isMovable() const override { return true; }
private:
};
class RageBot : public Actor {
public:
    RageBot(int startX, int startY, StudentWorld* world);
    virtual bool canMakeRobotSounds() const { return true; }
    int getCurLevel()
    {
        return curLevel;
    }
    void setCurLevel(int newLevel)
    {
        curLevel = newLevel;
    }
    int getTotalTicks()
    {
        return totalTicks;
    }
    void setTotalTicks(int newTicks)
    {
        totalTicks = newTicks;
    }
    int getRemainingTicks()
    {
        return curTicksLeft;
    }
    void setRemainingTicks(int newTicks)
    {
        curTicksLeft = newTicks;
    }
    virtual bool damagable() const {
        return true;
    }
    virtual bool isEnemy() const { return true; }
    bool moveIfPossible(int newX, int newY);
private:
    int curLevel = 0;
    int totalTicks = 0;
    int curTicksLeft = 0;
};
class ThiefBot : public Actor {
public:
    ThiefBot(int imageID, int startX, int startY, StudentWorld* world);
    virtual bool canMakeRobotSounds() const override { return true; }
    int getCurLevel()
    {
        return curLevel;
    }
    void setCurLevel(int newLevel)
    {
        curLevel = newLevel;
    }
    int getTotalTicks()
    {
        return totalTicks;
    }
    void setTotalTicks(int newTicks)
    {
        totalTicks = newTicks;
    }
    int getRemainingTicks()
    {
        return curTicksLeft;
    }
    virtual bool damagable() const override{
        return true;
    }
    void setRemainingTicks(int newTicks)
    {
        curTicksLeft = newTicks;
    }
    bool getGoodie()
    {
        return hasGoodie;
    }
    void setGoodie(bool status)
    {
        hasGoodie = status;
    }
    virtual bool isEnemy() const override { return true; }
    bool canStealGoodies() const override { return true; }
    bool moveIfPossible(int newX, int newY);
    bool moveInCurrentDirection();
    virtual void doSomething() override;
private:
    int curLevel = 0;
    int totalTicks = 0;
    int curTicksLeft = 0;
    bool m_extraLife = false;
    bool m_ammo = false;
    bool m_restore = false;
    int distanceBeforeTurning;
    int distanceMoved;
    bool hasGoodie;
};
class MeanThiefBot : public ThiefBot {
public: 
    MeanThiefBot(int startX, int startY, StudentWorld* world);
    bool canSeeAvatarAndShoot();
    void doSomething() override;
    bool canStealGoodies() const override { return true; }
private:
    int curLevel = 0;
    int totalTicks = 0;
    int curTicksLeft = 0;
    bool m_extraLife = false;
    bool m_ammo = false;
    bool m_restore = false;
    int distanceBeforeTurning;
    int distanceMoved;
};
class ThiefBotFactory : public Actor {
public:
    ThiefBotFactory(int startX, int startY, StudentWorld* world);
    int getCurLevel()
    {
        return curLevel;
    }
    void setCurLevel(int newLevel)
    {
        curLevel = newLevel;
    }
    int getTotalTicks()
    {
        return totalTicks;
    }
    void setTotalTicks(int newTicks)
    {
        totalTicks = newTicks;
    }
    int getRemainingTicks()
    {
        return curTicksLeft;
    }
    
    void setRemainingTicks(int newTicks)
    {
        curTicksLeft = newTicks;
    }
    
    void spawnThiefBot();
    virtual void doSomething();
private:
    int curLevel = 0;
    int totalTicks = 0;
    int curTicksLeft = 0;
};
class MeanThiefBotFactory : public Actor {
public:
    MeanThiefBotFactory(int startX, int startY, StudentWorld* world);
    int getCurLevel()
    {
        return curLevel;
    }
    void setCurLevel(int newLevel)
    {
        curLevel = newLevel;
    }
    int getTotalTicks()
    {
        return totalTicks;
    }
    void setTotalTicks(int newTicks)
    {
        totalTicks = newTicks;
    }
    int getRemainingTicks()
    {
        return curTicksLeft;
    }
    void setRemainingTicks(int newTicks)
    {
        curTicksLeft = newTicks;
    }
    void spawnThiefBot();
    virtual void doSomething();
private:
    int curLevel = 0;
    int totalTicks = 0;
    int curTicksLeft = 0;
};
class VertRageBot : public RageBot
{
public:
    VertRageBot(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};
class HorizRageBot : public RageBot
{
public:
    HorizRageBot(int startX, int startY, StudentWorld* world);
    virtual bool damagable() const {
        return true;
    }
    virtual void doSomething();
};
class Goodies : public Actor { 
public:
    virtual bool canOverlap() const {
        return canBeOverlapped;
    }
    void setOverlap(bool new_canBeOverlapped) {
        canBeOverlapped = new_canBeOverlapped;
    }
    Goodies(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething() = 0;
    ThiefBot* getAttachedThiefBot()
    {
        return attachedThiefBot;
    }
    void setAttachedThiefBot(ThiefBot* new_attachedbot)
    {
        attachedThiefBot = new_attachedbot;
    }
    virtual bool canBePickedUp() const { return true; }
private:
    ThiefBot* attachedThiefBot = nullptr;
    bool canBeOverlapped = false;
};
class Ammo : public Goodies {
public:
    Ammo(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class Restore : public Goodies {
public:
    Restore(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
private:
};

class ExtraLife : public Goodies {
public:
    ExtraLife(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
private:
};
#endif // ACTOR_H_
