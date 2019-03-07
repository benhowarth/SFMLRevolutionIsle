//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_BUILDINGS_H
#define SFMLREVOLUTIONISLE_BUILDINGS_H

#include "resources.h"
#include "entities.h"
#include "tools.h"
#include "ui.h"

class ResourceBoss;
class Entity;
class Enemy;


class Bullet{
public:
    sf::Vector2f m_pos;
    sf::Vector2f m_vel;
    float m_damage=10;
    float m_lifeSpan=3;
    float m_rad=5;
public:
    Bullet(sf::Vector2f t_pos,float t_damage,float speed,sf::Vector2f t_target):m_pos(t_pos),m_damage(t_damage){
        m_vel=speed*vec::Normalise(sf::Vector2f(t_target-t_pos));
    }
    void draw(sf::RenderWindow &win);
    void update(float &time,float &dTime);
};

class Building{
public:
    TileStruct* m_tileParent;
    sf::Vector2i m_pos;
    sf::Vector2i m_coords;
    //ResourceBoss* rb;
    float m_percentageBuilt=0;
    float m_buildSpeed=1;
    float m_timer=0;
    float m_timeInterval=5;
    float m_timeUpgrade=0.3;
    std::vector<Entity*> inhabitants;
    ResourceBoss* m_res;
    bool m_showUI=true;
    Building(TileStruct* t_tileParent,sf::Vector2i t_pos,sf::Vector2i t_coords,ResourceBoss* t_res):m_tileParent(t_tileParent),m_pos(t_pos),m_coords(t_coords),m_res(t_res){
        //printf("new building with parent %x or tile type %i\n",m_tileParent,m_tileParent->basicTile);
    }

    ~Building();

    void addInhabitant(Entity* t_entity){
        inhabitants.push_back(t_entity);
    }

    void removeInhabitant(Entity* t_entity){
        for(int i=0;i<inhabitants.size();i++){
            if(inhabitants[i]==t_entity){
                inhabitants.erase(inhabitants.begin()+i);
                break;
            }
        }
    }

    virtual float getTimeInterval(){};
    virtual void doAction();
    //virtual void draw(sf::RenderWindow &win);

    void update(float &time,float &dTime);
    virtual void personalUpdate(float &time,float &dTime){};

};

class Lighthouse:public Building{
    float m_rad=50;
    float m_radIncrement=25;
    float m_damage=10;
    float m_damageIncrement=2;
    float m_bulletSpeed=2;
public:
    std::vector<Enemy>* m_enemies;
    std::vector<Bullet> m_bullets;
    static constexpr float cost=5;
    Lighthouse(TileStruct* t_tileParent,sf::Vector2i t_pos,sf::Vector2i t_coords,ResourceBoss* t_res,std::vector<Enemy>* t_enemies):Building(t_tileParent,t_pos,t_coords,t_res){
        m_enemies=t_enemies;
        m_buildSpeed=0.1;
        m_timeInterval=2;
        m_timeUpgrade=0.15;
    };
    void draw(sf::RenderWindow &win);

    float getTimeInterval();
    void doAction();
    void personalUpdate(float &time,float &dTime);

    float getRadius(){return m_rad+inhabitants.size()*m_radIncrement;}
};

class Warehouse:public Building{
public:
    static constexpr float cost=10;
    Warehouse(TileStruct* t_tileParent,sf::Vector2i t_pos,sf::Vector2i t_coords,ResourceBoss* t_res):Building(t_tileParent,t_pos,t_coords,t_res){
        m_buildSpeed=0.3;
        m_timeInterval=10;
        m_timeUpgrade=0.8;
    };
    void draw(sf::RenderWindow &win);

    float getTimeInterval();
    void doAction();
    void personalUpdate(float &time,float &dTime);
};


#endif //SFMLREVOLUTIONISLE_BUILDINGS_H
