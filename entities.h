//
// Created by Ben on 06/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_ENTITIES_H
#define SFMLREVOLUTIONISLE_ENTITIES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "tiles.h"
#include "tools.h"

extern sf::Sprite charSprite;

void drawCharAt(sf::RenderWindow &win,int charDir,int x,int y,float scaleX,float scaleY);

class Entity{
private:
    float s_roadSpeedBoost=1.5;
public:
    sf::Vector2f m_tPos;
    sf::Vector2f m_pos;
    sf::Vector2f m_size=sf::Vector2f(tileSize,tileSize);
    sf::Vector2f m_vel=sf::Vector2f(0,0);;
    std::vector<sf::Vector2f> m_targets;
    float m_speed=0.5;
    TileMap* m_tMap;
    int idleMoveTime=0;
public:
    Entity(TileMap* t_tMap){
        m_pos=sf::Vector2f(0,0);
        m_tPos=m_pos;
        m_tMap=t_tMap;
    }
    Entity(TileMap* t_tMap,float t_x,float t_y){
        m_pos=sf::Vector2f(t_x,t_y);
        m_tPos=m_pos;
        m_tMap=t_tMap;
    }
    Entity(TileMap* t_tMap,float t_x,float t_y,float t_speed){
        m_pos=sf::Vector2f(t_x,t_y);
        m_tPos=m_pos;
        m_speed=t_speed;
        m_tMap=t_tMap;
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
    void moveToTarget();
    void update(float time);
    TileStruct* getCurrentTile();
    void tryPath(int tileId);
};

extern std::vector<Entity> friends;
extern std::vector<Entity> enemies;

void newFriend(TileMap &tMap);

extern bool selectionBoxOn;
extern sf::Vector2f selectionBoxStart;
extern sf::Vector2f selectionBoxEnd;
extern std::vector<int> selectedFriends;

void updateAndDrawFriends(float time);

#endif //SFMLREVOLUTIONISLE_ENTITIES_H
