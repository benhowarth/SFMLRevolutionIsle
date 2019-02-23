//
// Created by Ben on 11/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_RESOURCES_H
#define SFMLREVOLUTIONISLE_RESOURCES_H

#include <vector>
#include "entities.h"
#include "tiles.h"

class Building;
class Lighthouse;
class Warehouse;


class ResourceBoss{
private:
    int m_resources=0;
    TileMap* m_tMap;
    std::vector<Entity>* m_friends;
    std::vector<Entity>* m_enemies;
    std::vector<Lighthouse> m_lighthouses;
    std::vector<Warehouse> m_warehouses;
public:
    ResourceBoss(TileMap* t_tMap,std::vector<Entity>* t_friends,std::vector<Entity>* t_enemies):m_tMap(t_tMap),m_friends(t_friends),m_enemies(t_enemies){};
    void addResources(int t_resources){
        if(m_resources+t_resources>=0){
            m_resources+=t_resources;
        }else{
            m_resources=0;
        }
    }
    int getResources(){return m_resources;}
    void buildWarehouse(int t_x,int t_y);
    void buildLighthouse(int t_x,int t_y);
    void destroyWarehouse(int t_x,int t_y);
    void destroyLighthouse(int t_x,int t_y);
    void updateAndDraw(sf::RenderWindow &t_win,float &time,float &dt);

};


#endif //SFMLREVOLUTIONISLE_RESOURCES_H
