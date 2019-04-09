//
// Created by Ben on 06/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_ENTITIES_H
#define SFMLREVOLUTIONISLE_ENTITIES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "tiles.h"
#include "buildings.h"
#include "tools.h"
#include "ui.h"
#include "resources.h"

class ResourceBoss;
class UIBoss;
class Building;
class Lighthouse;
class Warehouse;


extern sf::Sprite charSprite;
extern sf::Sprite enemySprite;
extern CompassDir enemySpawnDir;

void drawCharAt(sf::RenderWindow &win,int charDir,int x,int y,float scaleX,float scaleY);

class TileMap;

class Entity{
private:
    float s_roadSpeedBoost=1.5;
public:
    float hpMax=100;
    Building* m_buildingParent=nullptr;
    bool invisible=false;
    sf::Vector2f m_tPos;
    sf::Vector2f m_pos;
    sf::Vector2f m_size=sf::Vector2f(tileSize,tileSize);
    sf::Vector2f m_vel=sf::Vector2f(0,0);;
    std::vector<sf::Vector2f> m_targets;
    float m_speed=0.5;
    TileMap* m_tMap;
    ResourceBoss* m_res;
    int idleMoveTime=0;
    bool toDelete=false;
    std::vector<Tile> const* m_pathMask=&tMask_all;
    sf::Sprite* m_sprite=&charSprite;
    bool m_dancing=false;
public:
    bool m_saved=false;
    Entity(TileMap* t_tMap,ResourceBoss* t_res){
        m_pos=sf::Vector2f(0,0);
        m_tPos=m_pos;
        m_tMap=t_tMap;
        m_res=t_res;
    }
    Entity(TileMap* t_tMap,ResourceBoss* t_res,float t_x,float t_y){
        m_pos=sf::Vector2f(t_x,t_y);
        m_tPos=m_pos;
        m_tMap=t_tMap;
        m_res=t_res;
    }
    Entity(TileMap* t_tMap,ResourceBoss* t_res,float t_x,float t_y,float t_speed){
        m_pos=sf::Vector2f(t_x,t_y);
        m_tPos=m_pos;
        m_speed=t_speed;
        m_tMap=t_tMap;
        m_res=t_res;
    }
    void draw(sf::RenderWindow* win);

    float getVelAngle(){
        return std::atan2(m_vel.x,m_vel.y);
    }
    int getVelDirInt(){
        return ((int)std::round((getVelAngle()*8)/(2*3.14)+8))%8;
    }
    void setVel(float t_x,float t_y){
        m_vel.x=t_x;
        m_vel.y=t_y;
    }
    void addTarget(sf::Vector2f t_target){
        m_targets.push_back(t_target);
    }
    void nextTarget(){

        if(!m_targets.empty()) {
            sf::Vector2f nextTar = m_targets.front();
            m_targets.erase(m_targets.begin());
            setTarget(nextTar);
        }
    }
    void setTarget(sf::Vector2f t_newPos){
        m_tPos=t_newPos;
    }
    virtual float* getHP(){};
    bool addHP(float t_hp);
    void markForDeletion(){toDelete=true;}
    bool isToDelete(){return toDelete;}
    void moveToTarget();
    void update(float time,float dt);
    TileStruct* getCurrentTile();
    bool tryPath(int tileId);
    void attachToBuilding(Building* curBuild);
    void detachFromBuilding();
    void save();
    bool getSaved();
};
class Friend:public Entity{
public:
    float hp=100;
    Friend(TileMap* tMap,ResourceBoss* res,int t_x,int t_y);
    void update(float time,float dt);
    float* getHP(){return &hp;}
};
class Enemy:public Entity{
public:
    float hp=100;
    float damageTime=0;
    float damageTimeInterval=3;
    float damage=0.5;
    Enemy(TileMap* tMap,ResourceBoss* res,int t_x,int t_y):Entity(tMap,res,t_x,t_y,0.2){
        m_sprite=&enemySprite;
        m_pathMask=&tMask_island_general;
    };
    void update(float time,float dt);
    float* getHP(){return &hp;}
};



extern std::vector<Friend> friends;
extern std::vector<Enemy> enemies;

void newFriend(TileMap* tMap,ResourceBoss* res);
void newEnemy(TileMap* tMap,ResourceBoss* res);

extern bool selectionBoxOn;
extern sf::Vector2f selectionBoxStart;
extern sf::Vector2f selectionBoxEnd;
extern std::vector<int> selectedFriends;

void updateAndDrawFriends(float time,float dt,ResourceBoss* t_res);
void updateAndDrawEnemies(float time,float dt,ResourceBoss* t_res);

void toggleSelectFriend(const int &i,bool clearRest);
void toggleSelectFriend(const int &i);

#endif //SFMLREVOLUTIONISLE_ENTITIES_H
