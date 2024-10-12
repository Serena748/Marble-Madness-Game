#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY, int dir, double size, StudentWorld* world) :
    GraphObject(imageID, startX, startY) {
    m_studentWorld = world;
}
Avatar::Avatar(int startX, int startY, StudentWorld* world) :
    Actor(IID_PLAYER, startX, startY, 0, 0, world) {
        setHealth(20);
        setMaxHealth(20);
}
Crystal::Crystal(int startX, int startY, StudentWorld* world)
    : Actor(IID_CRYSTAL, startX, startY, 0, 0, world) {
        if (returnWorld()->getNumCrystals() == -1) returnWorld()->setNumCrystals(returnWorld()->getNumCrystals()+2);
        else returnWorld()->setNumCrystals(returnWorld()->getNumCrystals()+1);
        
}
Pit::Pit(int startX, int startY, StudentWorld* world)
    : Actor(IID_PIT, startX, startY, 0, 0, world) {
        
}
Pea::Pea(int imageID, int startX, int startY, int direction, StudentWorld* world)
: Actor(imageID, startX, startY, direction, 0, world) {
}
Marble::Marble(int startX, int startY, StudentWorld* world)
    : Actor(IID_MARBLE, startX, startY, 0, 0, world) {
        setHealth(10);
        setMaxHealth(10);
        
}
Exit::Exit(int startX, int startY, StudentWorld* world)
: Actor(IID_EXIT, startX, startY, 0, 0, world) {
    setVisible(false);
}
Wall::Wall(int startX, int startY, StudentWorld* world)
    : Actor(IID_WALL, startX, startY, 0, 0, world) {
        
}
HorizRageBot::HorizRageBot(int startX, int startY, StudentWorld* world)
    : RageBot(startX, startY, world) {
    setHealth(10);
    setMaxHealth(10);
}
VertRageBot::VertRageBot(int startX, int startY, StudentWorld* world)
    : RageBot(startX, startY, world) {
    setHealth(10);
    setMaxHealth(10);
}
Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(imageID, startX, startY, 0,0,world) {
}
Ammo::Ammo(int startX, int startY, StudentWorld* world)
    : Goodies(IID_AMMO, startX, startY, world) {
}
Restore::Restore(int startX, int startY, StudentWorld* world)
    : Goodies(IID_RESTORE_HEALTH, startX, startY, world) {
}
ExtraLife::ExtraLife(int startX, int startY, StudentWorld* world)
    : Goodies(IID_EXTRA_LIFE, startX, startY, world) {
}

void Goodies::doSomething()
{
    if (returnWorld()->overlapActorThiefBot(getX()+1, getY(), this) != nullptr ||
        returnWorld()->overlapActorThiefBot(getX()-1, getY(), this) != nullptr ||
        returnWorld()->overlapActorThiefBot(getX(), getY()+1, this) != nullptr ||
        returnWorld()->overlapActorThiefBot(getX(), getY()-1, this) != nullptr ||
        returnWorld()->overlapAvatar(getX()+1, getY()) ||
        returnWorld()->overlapAvatar(getX()-1, getY()) ||
        returnWorld()->overlapAvatar(getX(), getY()+1) ||
        returnWorld()->overlapAvatar(getX(), getY()-1))
    {
        setOverlap(true);
    }
    else {
        setOverlap(false);
    }

    if(attachedThiefBot != nullptr) {
        if(!attachedThiefBot->isAlive()) {
            setVisible(true);
            attachedThiefBot = nullptr;
        } else {
            moveTo(attachedThiefBot->getX(), attachedThiefBot->getY());
        }
    } else {
        ThiefBot* potentialThiefBot = returnWorld()->overlapActorThiefBot(getX(), getY(), this);
        if(potentialThiefBot != nullptr && !potentialThiefBot->getGoodie()) {
            if(randInt(1, 10) == 1) {
                returnWorld()->playSound(SOUND_ROBOT_MUNCH);
                setVisible(false);
                setAttachedThiefBot(potentialThiefBot);
                moveTo(potentialThiefBot->getX(), potentialThiefBot->getY());
                potentialThiefBot->setGoodie(true);
            }
        }
    }
}

void Ammo::doSomething() {
    if(!isAlive()) return;
    
    Goodies::doSomething();
    if(getAttachedThiefBot() == nullptr && returnWorld()->overlapAvatar(getX(), getY())) {
        returnWorld()->getAvatar()->setAmmo(returnWorld()->getAvatar()->getAmmo() + 20);
        returnWorld()->increaseScore(returnWorld()->getScore()+100);
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveStatus(false);
    }
}
void Restore::doSomething() {
    if(!isAlive()) return;
    
    Goodies::doSomething();
    if(getAttachedThiefBot() == nullptr && returnWorld()->overlapAvatar(getX(), getY()))
    {
        returnWorld()->getAvatar()->setHealth(returnWorld()->getAvatar()->getMaxHealth());
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        returnWorld()->increaseScore(returnWorld()->getScore()+500);
        setAliveStatus(false);
    }
    
}
void ExtraLife::doSomething() {
    if(!isAlive()) return;
    
    Goodies::doSomething();
    if(getAttachedThiefBot() == nullptr && returnWorld()->overlapAvatar(getX(), getY()))
    {
        returnWorld()->incLives();
        returnWorld()->playSound(SOUND_GOT_GOODIE);
        returnWorld()->increaseScore(returnWorld()->getScore()+1000);
        setAliveStatus(false);
    }
    
}
ThiefBot::ThiefBot(int imageID, int startX, int startY, StudentWorld* world)
    : Actor(IID_THIEFBOT, startX, startY, 0, 0, world) {
        Level lev(world->assetPath());
        ostringstream oss;
        oss << world->getLevel();
        string s = oss.str();
        int level = std::stoi(s);
        setCurLevel(level);
        totalTicks = (28 - level) / 4;
        curTicksLeft = (28 - level) / 4;
        if (totalTicks < 3) {
            totalTicks = 3;
            curTicksLeft = 3;
        }
        setHealth(5);
        setMaxHealth(5);
}
MeanThiefBot::MeanThiefBot(int startX, int startY, StudentWorld* world)
    : ThiefBot(IID_MEAN_THIEFBOT, startX, startY, world) {
        Level lev(world->assetPath());
        ostringstream oss;
        oss << world->getLevel();
        string s = oss.str();
        int level = std::stoi(s);
        setCurLevel(level);
        totalTicks = (28 - level) / 4;
        curTicksLeft = (28 - level) / 4;
        if (totalTicks < 3) {
            totalTicks = 3;
            curTicksLeft = 3;
        }
        setHealth(8);
        setMaxHealth(8);
}

StudentWorld* Actor::returnWorld() {
    return m_studentWorld;
}
void Avatar::doSomething() {
    
    int key;
    if (!isAlive()) return;
    if (returnWorld()->getKey(key)) {
        switch (key) {
            {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if (!returnWorld()->overlapActor(getX() - 1, getY(), this)) {
                    moveTo(getX() - 1, getY());
                }
                else if (returnWorld()->overlapActorMarble(getX() - 1, getY(), this) != nullptr && !returnWorld()->overlapActor(getX() - 2, getY(), this))
                {
                    returnWorld()->overlapActorMarble(getX() - 1, getY(), this)->doSomething();
                    moveTo(getX() - 1, getY());
                }
                else {
                    return;
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if (!returnWorld()->overlapActor(getX() + 1, getY(), this)) {
                    moveTo(getX() + 1, getY());
                }
                else if (returnWorld()->overlapActorMarble(getX() + 1, getY(), this) != nullptr  && !returnWorld()->overlapActor(getX() + 2, getY(), this))
                {
                    returnWorld()->overlapActorMarble(getX() + 1, getY(), this)->doSomething();
                    moveTo(getX() + 1, getY());
                }
                else {
                    return;
                    
                }
                break;
            case KEY_PRESS_UP:
                setDirection(90);
                if (!returnWorld()->overlapActor(getX(), getY()+ 1, this)) {
                    moveTo(getX(), getY()+ 1);
                }
                else if (returnWorld()->overlapActorMarble(getX(), getY()+1, this) != nullptr  && !returnWorld()->overlapActor(getX(), getY()+2, this))
                {
                    returnWorld()->overlapActorMarble(getX(), getY()+1, this)->doSomething();
                    moveTo(getX(), getY()+ 1);
                }
                else {
                    return;
                    
                }
                break;
            
            case KEY_PRESS_DOWN:
                setDirection(270);
                if (!returnWorld()->overlapActor(getX(), getY()-1, this)) {
                    moveTo(getX(), getY()-1);
                }
                else if (returnWorld()->overlapActorMarble(getX(), getY()-1, this) != nullptr  && !returnWorld()->overlapActor(getX(), getY()-2, this))
                {
                    returnWorld()->overlapActorMarble(getX(), getY()-1, this)->doSomething();
                    moveTo(getX(), getY()-1);
                }
                else {
                    return;
                    
                }
                break;
            }
            case KEY_PRESS_SPACE:
                if(getAmmo() > 0){
                    returnWorld()->playSound(SOUND_PLAYER_FIRE);
                    int currentDirection = getDirection();
                    int newX = getX() + (currentDirection == 0 ? 1 : (currentDirection == 180 ? -1 : 0));
                    int newY = getY() + (currentDirection == 90 ? 1 : (currentDirection == 270 ? -1 : 0));
                    returnWorld()->createProjectile(newX, newY, currentDirection);
                    setAmmo(getAmmo()-1);
                }
                break;
        }
    }
    
}
void Wall::doSomething() {
    ;
}

void Pea::doSomething() {
    if (!isAlive()) return;
        
    if (returnWorld()->damageWProjectile(getX(), getY(), this) || returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(getX(), getY(), this))
    {
        setAliveStatus(false);return;
    }
    if (returnWorld()->overlapAvatar(getX(),getY()))
    {
        if (returnWorld()->getAvatar()->getHealth() >0)
        {
            returnWorld()->getAvatar()->setHealth(returnWorld()->getAvatar()->getHealth()-2);
            returnWorld()->playSound(SOUND_PLAYER_IMPACT);
        }
        else
        {
            returnWorld()->getAvatar()->setAliveStatus(false);
            returnWorld()->decLives();
            returnWorld()->playSound(SOUND_PLAYER_DIE);
        }
        setAliveStatus(false);return;
        
    }
    moveProjectile();
        
    
}
void Pea::moveProjectile() {
    if (pea_direction == 0) {
        moveTo(getX() + 1, getY());
    }
    else if (pea_direction == 90) {
        moveTo(getX(), getY()+1);
    }
    else if (pea_direction == 180) {
        moveTo(getX() - 1, getY());
    }
    else if (pea_direction == 270) {
        moveTo(getX(), getY()-1);
    }
}
void Crystal::doSomething() {
    if (returnWorld()->overlapAvatar(getX(), getY())) {
        
        returnWorld()->increaseScore(returnWorld()->getScore()+50);
        returnWorld()->setNumCrystals(returnWorld()->getNumCrystals()-1);
        setAliveStatus(false);

    }
}
void Exit::doSomething() {
    if (returnWorld()->overlapAvatar(getX()+1, getY()) || returnWorld()->overlapAvatar(getX()-1, getY()) ||
        returnWorld()->overlapAvatar(getX(), getY()+1) ||
        returnWorld()->overlapAvatar(getX(), getY()-1))
    {
        setOverlap(true);
    }
    else {setOverlap(false);}
    if(returnWorld()->getNumCrystals() == 0)
    {
        returnWorld()->setNumCrystals(-1);
        returnWorld()->playSound(SOUND_REVEAL_EXIT);
        setVisible(true);
    }  
    
    if (returnWorld()->overlapAvatar(getX(), getY()) && isVisible()) {
        returnWorld()->playSound(SOUND_FINISHED_LEVEL);
        returnWorld()->setCompleteLevel(true);
        setAliveStatus(false);

    }
}


void ThiefBot::doSomething() {
    if (!isAlive()) return;

    if (getRemainingTicks() > 0) {
        setRemainingTicks(getRemainingTicks() - 1);
        return;
    }

    setRemainingTicks(getTotalTicks());

    if (distanceBeforeTurning == 0 || !moveInCurrentDirection()) {
        distanceBeforeTurning = randInt(1, 6);

        bool moved = false;
        for (int attempt = 0; attempt < 4 && !moved; ++attempt) {
            setDirection(randInt(0, 3) * 90);

            moved = moveInCurrentDirection();
        }
    }
}
void MeanThiefBot::doSomething() {
    if (!isAlive()) return;

    if (getRemainingTicks() > 0) {
        setRemainingTicks(getRemainingTicks() - 1);
        return;
    }

    setRemainingTicks(getTotalTicks());

    if (canSeeAvatarAndShoot()) {
        int currentDirection = getDirection();
        int newX = getX() + (currentDirection == 0 ? 1 : (currentDirection == 180 ? -1 : 0));
        int newY = getY() + (currentDirection == 90 ? 1 : (currentDirection == 270 ? -1 : 0));
        returnWorld()->playSound(SOUND_ENEMY_FIRE);
        returnWorld()->createProjectile(newX, newY, currentDirection);
        returnWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }

    if (distanceBeforeTurning == 0 || !moveInCurrentDirection()) {
        distanceBeforeTurning = randInt(1, 6);

        bool moved = false;
        for (int attempt = 0; attempt < 4 && !moved; ++attempt) {
            setDirection(randInt(0, 3) * 90);

            moved = moveInCurrentDirection();
        }
    }
}
bool MeanThiefBot::canSeeAvatarAndShoot() {
    int avatarLocationX = returnWorld()->getAvatar()->getX();
    int avatarLocationY = returnWorld()->getAvatar()->getY();

    bool sameRow = (avatarLocationY == getY());
    bool sameColumn = (avatarLocationX == getX());

    if (!sameRow && !sameColumn) {
        return false;
    }

    bool clearPath = true;
    int distance;

    if (sameRow) {
        distance = avatarLocationX - getX();
        int direction = distance > 0 ? 0 : 180;

        if (getDirection() == direction) {
            for (int i = 1; i <= abs(distance); i++) {
                if (returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(getX() + (direction == 0 ? i : -i), getY(), this)) {
                    clearPath = false;
                    break;
                }
            }
        } else {
            clearPath = false;
        }
    } else if (sameColumn) {
        distance = avatarLocationY - getY();
        int direction = distance > 0 ? 90 : 270;

        if (getDirection() == direction) {
            for (int i = 1; i <= abs(distance); i++) {
                if (returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(getX(), getY() + (direction == 90 ? i : -i), this)) {
                    clearPath = false;
                    break;
                }
            }
        } else {
            clearPath = false;
        }
    }

    return clearPath;
}
bool ThiefBot::moveInCurrentDirection() {
    int newX = getX(), newY = getY();
    switch (getDirection()) {
        case 0: newX += 1; break; // Right
        case 90: newY += 1; break; // Up
        case 180: newX -= 1; break; // Left
        case 270: newY -= 1; break; // Down
        default: return false;
    }

    if (moveIfPossible(newX, newY)) {
        distanceBeforeTurning--;
        return true;
    }
    
    return false;
}
ThiefBotFactory::ThiefBotFactory(int startX, int startY, StudentWorld* world)
    : Actor(IID_ROBOT_FACTORY, startX, startY, 0, 0, world) {
        Level lev(world->assetPath());
        ostringstream oss;
        oss << world->getLevel();
        string s = oss.str();
        int level = std::stoi(s);
        setCurLevel(level);
        totalTicks = (28 - level) / 4;
        curTicksLeft = (28 - level) / 4;
        if (totalTicks < 3) {
            totalTicks = 3;
            curTicksLeft = 3;
        }
}
MeanThiefBotFactory::MeanThiefBotFactory(int startX, int startY, StudentWorld* world)
    : Actor(IID_ROBOT_FACTORY, startX, startY, 0, 0, world) {
        Level lev(world->assetPath());
        ostringstream oss;
        oss << world->getLevel();
        string s = oss.str();
        int level = std::stoi(s);
        setCurLevel(level);
        totalTicks = (28 - level) / 4;
        curTicksLeft = (28 - level) / 4;
        if (totalTicks < 3) {
            totalTicks = 3;
            curTicksLeft = 3;
        }
}
void ThiefBotFactory::doSomething() {
    int thiefBotCount = 0;
    
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            int checkX = getX() + dx;
            int checkY = getY() + dy;
            if (returnWorld()->overlapActorThiefBot(checkX, checkY, this)) {
                thiefBotCount++;
            }
        }
    }
    if (thiefBotCount < 3 && !returnWorld()->overlapActorThiefBot(getX(), getY(), this)) {
        if (randInt(1, 50) == 1) {
            spawnThiefBot();
            returnWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}
void MeanThiefBotFactory::doSomething() {
    if (!isAlive()) return;

    int thiefBotCount = 0;
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            int checkX = getX() + dx;
            int checkY = getY() + dy;
            if (returnWorld()->overlapActorThiefBot(checkX, checkY, this)) {
                thiefBotCount++;
            }
        }
    }
    if (thiefBotCount < 3 && !returnWorld()->overlapActorThiefBot(getX(), getY(), this)) {
        if (randInt(1, 50) == 1) {
            spawnThiefBot();
            returnWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}
void ThiefBotFactory::spawnThiefBot() {
    ThiefBot* thiefbot = new ThiefBot(IID_THIEFBOT, getX(), getY(), returnWorld());
    returnWorld()->spawnThiefBot(thiefbot);
}
void MeanThiefBotFactory::spawnThiefBot() {
    ThiefBot* thiefbot = new MeanThiefBot(getX(), getY(), returnWorld());
    returnWorld()->spawnThiefBot(thiefbot);
}
bool ThiefBot::moveIfPossible(int newX, int newY) {
    if (!returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(newX, newY, this) && !returnWorld()->overlapAvatar(newX, newY) && !returnWorld()->overlapActorPit(newX, newY, this)) {
        moveTo(newX, newY);
        return true;
    }
    return false;
}
RageBot::RageBot(int startX, int startY, StudentWorld* world)
    : Actor(IID_RAGEBOT, startX, startY, 0, 0, world) {
        Level lev(world->assetPath());
        ostringstream oss;
        oss << world->getLevel();
        string s = oss.str();
        int level = std::stoi(s);
        setCurLevel(level); 
        totalTicks = (28 - level) / 4;
        curTicksLeft = (28 - level) / 4;
}
bool RageBot::moveIfPossible(int newX, int newY) {
    if (!returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(newX, newY, this) && !returnWorld()->overlapAvatar(newX, newY) && !returnWorld()->overlapActorPit(newX, newY, this)) {
        moveTo(newX, newY);
        return true;
    }
    return false;
}


void VertRageBot::doSomething() {
    if (!isAlive()) return;
    
    if (getRemainingTicks() == 0) {
        int avatarLocationX = returnWorld()->getAvatar()->getX();
        int avatarLocationY = returnWorld()->getAvatar()->getY();

        setRemainingTicks(getTotalTicks());

        bool actionTaken = false;

        if (avatarLocationX == getX()) {
            bool clearPath = true;
            int distance = avatarLocationY - getY();
            int direction = distance > 0 ? 90 : 270;
            if (getDirection() == direction) {
                for (int i = 1; i <= abs(distance); i++) {
                    if (returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(getX(), getY() + (direction == 90 ? i : -i), this)) {
                        clearPath = false;
                        break;
                    }
                }
                if (clearPath) {
                    int currentDirection = getDirection();
                    int newX = getX() + (currentDirection == 0 ? 1 : (currentDirection == 180 ? -1 : 0));
                    int newY = getY() + (currentDirection == 90 ? 1 : (currentDirection == 270 ? -1 : 0));
                    returnWorld()->playSound(SOUND_ENEMY_FIRE);
                    returnWorld()->createProjectile(newX, newY, direction);
                    actionTaken = true;
                }
            }
        }

        if (!actionTaken) {
            int currentDirection = getDirection();
            int newX = getX();
            int newY = getY() + (currentDirection == 90 ? 1 : -1);

            if (!moveIfPossible(newX, newY)) {
                int newDirection = (currentDirection == 90) ? 270 : 90;
                setDirection(newDirection);
            }
        }
    } else {
        setRemainingTicks(getRemainingTicks() - 1);
        
    }
}



void HorizRageBot::doSomething() {
    if (!isAlive()) return;
    if (getRemainingTicks() == 0) {
        int avatarLocationX = returnWorld()->getAvatar()->getX();
        int avatarLocationY = returnWorld()->getAvatar()->getY();

        setRemainingTicks(getTotalTicks());
        
        bool actionTaken = false;

        if (avatarLocationY == getY()) {
            bool clearPath = true;
            int distance = avatarLocationX - getX();
            int direction = distance > 0 ? 0 : 180;
            if (getDirection() == direction) {
                for (int i = 1; i <= abs(distance); i++) {
                    if (returnWorld()->overlapActorNoPeaNoPitNoExitNoGoodies(getX() + (direction == 0 ? i : -i), getY(), this)) {
                        clearPath = false;
                        break;
                    }
                }
                if (clearPath) {
                    int currentDirection = getDirection();
                    int newX = getX() + (currentDirection == 0 ? 1 : (currentDirection == 180 ? -1 : 0));
                    int newY = getY() + (currentDirection == 90 ? 1 : (currentDirection == 270 ? -1 : 0));
                    returnWorld()->playSound(SOUND_ENEMY_FIRE);
                    returnWorld()->createProjectile(newX, newY, direction);
                    actionTaken = true;
                }
            }
        }

        if (!actionTaken) {
            int currentDirection = getDirection();
            int newX = getX() + (currentDirection == 0 ? 1 : -1);
            int newY = getY();

            if (!moveIfPossible(newX, newY)) {
                int newDirection = (currentDirection == 0) ? 180 : 0;
                setDirection(newDirection);
            }
        }
    } else {
        setRemainingTicks(getRemainingTicks() - 1);
        
    }
}




void Marble::doSomething() {
    if (!isAlive()) return;
    if (returnWorld()->overlapActorPit(getX(), getY(), this) != nullptr)
    {
        setAliveStatus(false);
        return;
    }
    if (returnWorld()->getAvatar()->getX() == this->getX() && returnWorld()->getAvatar()->getY() == this->getY() && returnWorld()->getAvatar()->getDirection() == 180) {
        if (!returnWorld()->overlapActor(getX() - 1, getY(), this)) {
            moveTo(getX() - 1, getY());
        }
    }
    if (returnWorld()->getAvatar()->getX() == this->getX() && returnWorld()->getAvatar()->getY() == this->getY() && returnWorld()->getAvatar()->getDirection() == 0) {
        if (!returnWorld()->overlapActor(getX() + 1, getY(), this)) {
            moveTo(getX() + 1, getY());
        }
    }
    if (returnWorld()->getAvatar()->getX() == this->getX() && returnWorld()->getAvatar()->getY() == this->getY() && returnWorld()->getAvatar()->getDirection() == 90) {
        if (!returnWorld()->overlapActor(getX() , getY()+1, this)) {
            moveTo(getX() , getY()+1);
        }
    }
    if (returnWorld()->getAvatar()->getX() == this->getX() && returnWorld()->getAvatar()->getY() == this->getY() && returnWorld()->getAvatar()->getDirection() == 270) {
        if (!returnWorld()->overlapActor(getX() , getY()- 1, this)) {
            moveTo(getX(), getY()- 1);
        }
    }
    
}
void Pit::doSomething()
{
    if(!isAlive()) return;
    
    if (returnWorld()->overlapActorMarble(getX()+1, getY(), this) != nullptr || returnWorld()->overlapActorMarble(getX()-1, getY(), this) != nullptr || returnWorld()->overlapActorMarble(getX(), getY()+1, this) != nullptr || returnWorld()->overlapActorMarble(getX(), getY()-1, this) != nullptr)
    {
        setOverlap(true);
    }
    else setOverlap(false);
    if (returnWorld()->overlapActorMarble(getX(), getY(), this))
    {
        setAliveStatus(false);
        returnWorld()->overlapActorMarble(getX(), getY(), this)->setAliveStatus(false);
    }
    
}
