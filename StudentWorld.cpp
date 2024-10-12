#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
int StudentWorld::init() {
    Level lev(assetPath());
    ostringstream oss;
    oss << "level" << setfill('0') << setw(2) << getLevel() << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found || level_file == "level99.txt")
        return GWSTATUS_PLAYER_WON;
    
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        Level::MazeEntry ge;
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            for (int x = 0; x < VIEW_WIDTH; x++) {
                
                ge = lev.getContentsOf(x, y);
                
                switch (ge) {
                    case Level::empty:
                        break;
                    case Level::player:
                        m_player = new Avatar(x, y, this);
                        break;
                    case Level::wall:
                        actors.push_back(new Wall(x, y, this));
                        break;
                    case Level::crystal:
                        actors.push_back(new Crystal(x, y, this));
                        break;
                    case Level::marble:
                        actors.push_back(new Marble(x, y, this));
                        break;
                    case Level::pit:
                        actors.push_back(new Pit(x, y, this));
                        break;
                    case Level::vert_ragebot:
                        actors.push_back(new VertRageBot(x, y, this));
                        break;
                    case Level::horiz_ragebot:
                        actors.push_back(new HorizRageBot(x, y, this));
                        break;
                    case Level::thiefbot_factory:
                        actors.push_back(new ThiefBotFactory(x, y, this));
                        break;
                    case Level::mean_thiefbot_factory:
                        actors.push_back(new MeanThiefBotFactory(x, y, this));
                        break;
                    case Level::ammo:
                        actors.push_back(new Ammo(x, y, this));
                        break;
                    case Level::restore_health:
                        actors.push_back(new Restore(x, y, this));
                        break;
                    case Level::extra_life:
                        actors.push_back(new ExtraLife(x, y, this));
                        break;
                    case Level::exit:
                        actors.push_back(new Exit(x, y, this));
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}
   

int StudentWorld::move()
{

    if (m_player->isAlive()) {
        m_player->doSomething();
    }

    list<Actor*>::iterator it;
    it = actors.begin();
    while ((it) != actors.end()) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();
            if (!m_player->isAlive()) {
                return GWSTATUS_PLAYER_DIED;
            }

            if (finishGame()) {
                return GWSTATUS_PLAYER_WON;
            }

            if (getCompleteLevel()) {
                setCompleteLevel(false);
                increaseScore(getScore()+2000+getCurrentLevelBonus());
                return GWSTATUS_FINISHED_LEVEL;
            }

        }
        it++;
    }
    list<Actor*>::iterator it2;
    it2 = actors.begin();

    while ((it2) != actors.end()) {
        if (!(*it2)->isAlive()) {
            delete* it2;
            it2 = actors.erase(it2);
        }
        else
        {
            it2++;
        }
    }
    reduceLevelBonusByOne();
    displayGameText();

    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::displayGameText() {
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int bonus = getCurrentLevelBonus();

    std::ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(6) << score;
    oss.fill('0');
    oss << "  Level: " << setw(2) << level;
    oss << "  Lives: " << lives;
    oss << "  Health: " << m_player->getHealth()*100/m_player->getMaxHealth();
    oss << "%  Ammo: " << m_player->getAmmo();
    oss << "  Bonus: " << bonus;
    setGameStatText(oss.str());
}
bool StudentWorld::damageWProjectile(int X, int Y, Actor* ptr) {
    list<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) {
        if ((*it) == ptr) {
            it++;
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) ) {
            if ((*it)->damagable() && (*it)->isAlive()) {
                if ((*it)->getHealth() > 2)
                {
                    if((*it)->canMakeRobotSounds())
                    {
                        playSound(SOUND_ROBOT_IMPACT);
                    }
                    (*it)->setHealth((*it)->getHealth()-2);
                }
                else
                {
                    if((*it)->canMakeRobotSounds())
                    {
                        playSound(SOUND_ROBOT_DIE);
                    }
                    (*it)->setAliveStatus(false);
                }
                return true;
            }
        }
        it++;
    }
    return false;
}
bool StudentWorld::overlapActor(int X, int Y, Actor* ptr) {

    list<Actor*>::iterator it;
    it = actors.begin();
    while (it != actors.end()) {
        
        if ((*it) == ptr) {
            
            it++;
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y)) {
            if (!(*it)->canOverlap()) {
                return true;
            }
        }
        else {
            
        }
        it++;
    }
    return false;
}
bool StudentWorld::overlapActorNoPeaNoPitNoExitNoGoodies(int X, int Y, Actor* ptr) {
    for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
        if ((*it) == ptr || (*it)->canBeShot() || (*it)->canSwallowMarbles() || (*it)->canGetToNextLevel() || (*it)->canBePickedUp()) {
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) && !(*it)->canOverlap()) {
            return true;
        }
    }
    return false;
}

Actor* StudentWorld::overlapActorMarble(int X, int Y, Actor* ptr) {
    for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
        if ((*it) == ptr) {
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) && (*it)->isMovable()) {
            return (*it);
        }
    }
    return nullptr;
}

Actor* StudentWorld::overlapActorPit(int X, int Y, Actor* ptr) {
    for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
        if ((*it) == ptr) {
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) && (*it)->canSwallowMarbles()) {
            return (*it);
        }
    }
    return nullptr;
}

Actor* StudentWorld::overlapActorPea(int X, int Y, Actor* ptr) {
    for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
        if ((*it) == ptr) {
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) && (*it)->canBeShot()) {
            return (*it);
        }
    }
    return nullptr;
}

ThiefBot* StudentWorld::overlapActorThiefBot(int X, int Y, Actor* ptr) {
    for (std::list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it) {
        if ((*it) == ptr) {
            continue;
        }
        if (checkActorsOverlap((*it)->getX(), (*it)->getY(), X, Y) && (*it)->canStealGoodies()) {
            return static_cast<ThiefBot*>(*it);
        }
    }
    return nullptr;
}

bool StudentWorld::checkActorsOverlap(int X1, int Y1, int X2, int Y2) {
    if (abs(X1 - X2) <= 0 && abs(Y1 - Y2) <= 0) {
        return true;
    }
    return false;
}

bool StudentWorld::playerDiedDuringThisTick() {
    
    if (m_player != nullptr && m_player->getHealth() <= 0) {
        return true;
    }
    return false;
}

void StudentWorld::reduceLevelBonusByOne() {
    if (levelBonus > 0) {
        --levelBonus;
    }
}

int StudentWorld::getCurrentLevelBonus() const {
    return levelBonus;
}
void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    it = actors.begin();

    while (it != actors.end()) {
        delete* it;
        it = actors.erase(it);
    }

    delete m_player;
}
void StudentWorld::createProjectile(int X, int Y, int direction) {
    Pea* tempPea = new Pea(IID_PEA, X, Y, direction, this);
    tempPea->setDirection(direction);
    actors.push_back(tempPea);
}
void StudentWorld::createThiefBot(int X, int Y, int direction, bool isMean) {
    ThiefBot* tempActor = new ThiefBot(IID_THIEFBOT, X, Y, this);
    tempActor->setDirection(direction);
    actors.push_back(tempActor);
}
bool StudentWorld::overlapAvatar(int X, int Y) {

    return checkActorsOverlap( X, Y, m_player->getX(), m_player->getY());

}
void StudentWorld::spawnThiefBot(ThiefBot* newThiefBot) {
    playSound(SOUND_ROBOT_BORN);
    
    actors.push_back(newThiefBot);
}
    
