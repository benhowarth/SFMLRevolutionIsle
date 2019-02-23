//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_BUILDINGS_H
#define SFMLREVOLUTIONISLE_BUILDINGS_H

#include "resources.h"

class Building{
public:
    TileStruct* m_tileParent;
    //ResourceBoss* rb;
    float m_percentageBuilt=0;
    float m_buildSpeed;
    int m_cost;
    float m_timer=0;
    float m_timeInterval;
    Building(TileStruct* t_tileParent):m_tileParent(t_tileParent){
        printf("new building with parent %x or tile type %i",m_tileParent,m_tileParent->basicTile);
    }

    void doAction();
    //virtual void draw(sf::RenderWindow &win);

    void update(float &time,float &dTime);

};

class Lighthouse:public Building{
    float m_buildSpeed=7;
    float m_cost=5;
    float m_timeInterval=4;
public:
    Lighthouse(TileStruct* t_tileParent):Building(t_tileParent){};
    void draw(sf::RenderWindow &win);
    void doAction();
};

class Warehouse:public Building{
    float m_buildSpeed=10;
    float m_cost=10;
    float m_timeInterval=10;
public:
    Warehouse(TileStruct* t_tileParent):Building(t_tileParent){};
    void draw(sf::RenderWindow &win);
    void doAction();
};


#endif //SFMLREVOLUTIONISLE_BUILDINGS_H
