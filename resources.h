//
// Created by Ben on 11/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_RESOURCES_H
#define SFMLREVOLUTIONISLE_RESOURCES_H

#include <vector>
#include "tools.h"
#include "entities.h"
#include "tiles.h"
#include "ui.h"

class UIBoss;


class Entity;
class Friend;
class Enemy;

class Building;
class Lighthouse;
class Warehouse;



class ResourceBoss{
private:
    int m_resources=20;
    float m_hp=100;
    TileMap* m_tMap;
    std::vector<Friend>* m_friends;
    std::vector<Enemy>* m_enemies;
    std::vector<Lighthouse> m_lighthouses;
    std::vector<Warehouse> m_warehouses;
    int m_waveCounter=0;
    float m_waveTimer=0;
    float m_waveTimerMax=10;
    CompassDir m_enemySpawnDirNextWave=N;
    float m_enemySpawnTime=0;
    bool m_waveEnded=true;
public:
    UIBoss* ui;
    ResourceBoss(TileMap* t_tMap,std::vector<Friend>* t_friends,std::vector<Enemy>* t_enemies,UIBoss* t_ui):m_tMap(t_tMap),m_friends(t_friends),m_enemies(t_enemies),ui(t_ui){};
    bool addResources(int t_resources);
    bool hasPlayerFinishedWave();
    void tryToGoToNextWave();
    float getWaveFactor();
    float getResourcesOnWaveEnd();
    void startNextWave();
    void endWave();
    int getCurrentWave();
    float getWavePercentageComplete();
    float getEnemySpawnInterval();

    bool addHP(float t_damage){
        if(m_hp+t_damage>0){
            m_hp+=t_damage;
            //printf("resources: %i",m_resources);
            return true;
        }else{
            m_hp=0;
            return false;
        }
    }
    int getResources(){return m_resources;}
    float getHP(){return m_hp;}
    void buildWarehouse(int t_x,int t_y);
    void buildLighthouse(int t_x,int t_y);
    void destroyWarehouse(int t_x,int t_y);
    void destroyLighthouse(int t_x,int t_y);
    void destroyWarehouses(int t_x,int t_y,int t_brushSize);
    void destroyLighthouses(int t_x,int t_y,int t_brushSize);
    Lighthouse* getLighthouseByCoord(sf::Vector2i t_coords);
    Warehouse* getWarehouseByCoord(sf::Vector2i t_coords);
    Building* getBuildingByCoords(sf::Vector2i t_coords);

    void updateAndDraw(sf::RenderWindow &t_win,float &time,float &dt);
    void printBuildings();

};


#endif //SFMLREVOLUTIONISLE_RESOURCES_H
