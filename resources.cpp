//
// Created by Ben on 22/02/2019.
//
#include "resources.h"
#include "buildings.h"


void ResourceBoss::buildWarehouse(int t_x,int t_y){
    m_warehouses.reserve(1);
    TileStruct* tileParent=m_tMap->getTileFromCoords(sf::Vector2i(t_x,t_y));
    m_warehouses.emplace_back(tileParent);
}
void ResourceBoss::buildLighthouse(int t_x,int t_y){
    m_lighthouses.reserve(1);
    TileStruct* tileParent=m_tMap->getTileFromCoords(sf::Vector2i(t_x,t_y));
    m_lighthouses.emplace_back(tileParent);
}
void ResourceBoss::destroyWarehouse(int t_x,int t_y){

}
void ResourceBoss::destroyLighthouse(int t_x,int t_y){

}
void ResourceBoss::updateAndDraw(sf::RenderWindow &t_win,float &time,float &dt){
    for(Lighthouse l:m_lighthouses){
        l.update(time,dt);
        l.draw(t_win);
    }
    for(Warehouse w:m_warehouses){
        w.update(time,dt);
        w.draw(t_win);
    }
}