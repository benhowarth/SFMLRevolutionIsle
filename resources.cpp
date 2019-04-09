//
// Created by Ben on 22/02/2019.
//
#include <vector>
#include "resources.h"
#include "entities.h"

void ResourceBoss::incrementSavedDucks(){
    m_savedDucks++;
    if(m_savedDucks==10){inControl=false;}
}
int ResourceBoss::getSavedDucks(){
    return m_savedDucks;
}

bool ResourceBoss::addResources(int t_resources){

    std::string popUpString = "";
    if (t_resources >= 0) { popUpString = "+"; }
    popUpString += std::to_string(t_resources);

    if(m_resources+t_resources>=0 && m_resources+t_resources<=m_resourcesMax) {
        m_resources += t_resources;
        ui->addPopup(10, 80, popUpString, sf::Color::Blue, true);
        return true;
    }else if(m_resources+t_resources>m_resourcesMax){
        m_resources=m_resourcesMax;
        ui->addPopup(10, 80, popUpString, sf::Color::Blue, true);
        return true;
    }else{
        return false;
    }
}

bool ResourceBoss::hasPlayerFinishedWave(){
    return enemies.size()==0;
    //return m_waveEnded && m_waveTimer<=0 && enemies.size() <= 0;
}

void ResourceBoss::tryToGoToNextWave(){
    if(m_waveTimer<=0){startNextWave();}
}

float ResourceBoss::getWaveFactor(){
    float waveFactor=m_waveCounter/25;
    if(m_waveCounter>25){waveFactor=1;}
    return waveFactor;
}

float ResourceBoss::getResourcesOnWaveEnd(){
    return (int)10+(getWaveFactor()*100);
}
void ResourceBoss::startNextWave(){
    if(m_waveEnded) {
        //printf("start wave\n");
        m_waveCounter++;
        m_waveEnded = false;
        m_waveTimer = m_waveTimerMax;
        enemySpawnDir = m_enemySpawnDirNextWave;
    }else{
        printf("can't start wave\n");
    }
}
void ResourceBoss::endWave(){
    if(!m_waveEnded && m_waveTimer<=0 && m_enemies->size()==0) {
        //printf("ending wave\n");
        m_waveEnded = true;


        float r=getRandom();
        if(r>0.75) {
            m_enemySpawnDirNextWave = N;
        }else if(r>0.5) {
            m_enemySpawnDirNextWave = E;
        } else if(r>0.25) {
            m_enemySpawnDirNextWave = S;
        }else{
            m_enemySpawnDirNextWave = W;
        }


        addResources(getResourcesOnWaveEnd());
        newFriend(m_tMap,this);

        for(auto l:m_lighthouses){
            l.m_bullets.clear();
        }

    }/*else{
        printf("can't end wave\n");
    }*/
}
int ResourceBoss::getCurrentWave(){
    return m_waveCounter;
}
float ResourceBoss::getWavePercentageComplete(){
    return 1-(m_waveTimer/m_waveTimerMax);
}

float ResourceBoss::getEnemySpawnInterval(){
    return 7-(getWaveFactor()*6.9);
}

bool ResourceBoss::addHP(float t_damage){
    if(t_damage<0 && m_takenDamageAnimationTimer==0){m_takenDamageAnimationTimer=m_takenDamageAnimationTimerMax;}
    if(m_hp+t_damage>0){
        m_hp+=t_damage;
        //printf("resources: %i",m_resources);
        return true;
    }else {
        inControl=false;
        m_hp = 0;
        return false;
    }
}

bool ResourceBoss::getTakenDamageAnimationTimer(){return m_takenDamageAnimationTimer;}

void ResourceBoss::buildWarehouse(int t_x,int t_y){
    m_warehouses.reserve(1);
    sf::Vector2i coords = sf::Vector2i(t_x, t_y);
    sf::Vector2i pos = sf::Vector2i(t_x * tileSize * scaleFactor.x, t_y * tileSize * scaleFactor.y);
    TileStruct *tileParent = m_tMap->getTileFromCoords(coords);
    m_warehouses.emplace_back(tileParent, pos, coords,this);
}
void ResourceBoss::buildLighthouse(int t_x,int t_y){
    m_lighthouses.reserve(1);
    sf::Vector2i coords = sf::Vector2i(t_x, t_y);
    sf::Vector2i pos = sf::Vector2i(t_x * tileSize * scaleFactor.x, t_y * tileSize * scaleFactor.y);
    TileStruct *tileParent = m_tMap->getTileFromCoords(coords);
    m_lighthouses.emplace_back(tileParent, pos, coords,this,m_enemies);
    //m_lighthouses.back().m_enemies=m_enemies;
}

void ResourceBoss::buildRocket(int t_x,int t_y){
    //printf("rocket at %i,%i\n",t_x,t_y);
    m_rockets.reserve(1);
    sf::Vector2i coords = sf::Vector2i(t_x, t_y);
    sf::Vector2i pos = sf::Vector2i(t_x * tileSize * scaleFactor.x, t_y * tileSize * scaleFactor.y);
    TileStruct *tileParent = m_tMap->getTileFromCoords(coords);
    m_rockets.emplace_back(tileParent, pos, coords,this);
}

void ResourceBoss::destroyWarehouse(int t_x,int t_y){
    int id=m_tMap->getTileIdFromCoords(sf::Vector2i(t_x,t_y));
    for(int i=0;i<m_warehouses.size();i++){
        Warehouse &w=m_warehouses[i];
        if(id==w.m_tileParent->id){
            for(Entity* inhabitant:w.inhabitants){
                if(inhabitant) {
                    inhabitant->detachFromBuilding();
                }
            }
            //delete warehouse from list
            m_warehouses.erase(m_warehouses.begin()+i);
            addResources(Warehouse::cost);
            break;
        }
    }
}
void ResourceBoss::destroyLighthouse(int t_x,int t_y){
    int id=m_tMap->getTileIdFromCoords(sf::Vector2i(t_x,t_y));
    for(int i=0;i<m_lighthouses.size();i++){
        Lighthouse &l=m_lighthouses[i];
        if(id==l.m_tileParent->id){
            for(Entity* inhabitant:l.inhabitants){
                if(inhabitant) {
                    inhabitant->detachFromBuilding();
                }
            }
            //delete lighthouse from list
            m_lighthouses.erase(m_lighthouses.begin()+i);
            addResources(Lighthouse::cost);
            break;
        }
    }
}

void ResourceBoss::destroyLighthouses(int t_x, int t_y,int t_brushSize) {
    for(int x=0;x<t_brushSize;x++){
        for(int y=0;y<t_brushSize;y++){
            //printf("deleting at %i,%i\n",t_x+x,t_y+y);
            destroyLighthouse(t_x+x,t_y+y);
        }
    }
}

void ResourceBoss::destroyWarehouses(int t_x, int t_y,int t_brushSize) {
    for(int x=0;x<t_brushSize;x++){
        for(int y=0;y<t_brushSize;y++){
            destroyWarehouse(t_x+x,t_y+y);
        }
    }
}
Lighthouse* ResourceBoss::getLighthouseByCoord(sf::Vector2i t_coords) {
    //printf("lighthouse size2 %i",m_lighthouses.size());
    if (m_lighthouses.size() > 0){
        for (int i = 0; i < m_lighthouses.size(); i++) {
            Lighthouse* l = &m_lighthouses[i];
            if (l->m_coords.x == t_coords.x && l->m_coords.y == t_coords.y && !l->m_disabled) {
                return l;
            }
        }
    }
    return nullptr;


}

Warehouse* ResourceBoss::getWarehouseByCoord(sf::Vector2i t_coords) {

    if(m_warehouses.size()>0){
        for (int i = 0; i < m_warehouses.size(); i++) {
            Warehouse* w = &m_warehouses[i];
            if ((w->m_coords.x == t_coords.x || w->m_coords.x+1 == t_coords.x) && (w->m_coords.y == t_coords.y || w->m_coords.y+1 == t_coords.y)&& !w->m_disabled) {
                return w;
            }
        }
    }
    return nullptr;

}

Rocket* ResourceBoss::getRocketByCoord(sf::Vector2i t_coords) {

    if(m_rockets.size()>0){
        for (int i = 0; i < m_rockets.size(); i++) {
            Rocket* r = &m_rockets[i];
            if ((r->m_coords.x == t_coords.x || r->m_coords.x+1 == t_coords.x) && (r->m_coords.y == t_coords.y || r->m_coords.y+1 == t_coords.y) && !r->m_disabled) {
                return r;
            }
        }
    }
    return nullptr;

}
Building* ResourceBoss::getBuildingByCoords(sf::Vector2i t_coords){
    Lighthouse* l=getLighthouseByCoord(t_coords);
    if(l){
        return l;
    }

    Warehouse* w=getWarehouseByCoord(t_coords);
    if(w){
        return w;
    }
    return getRocketByCoord(t_coords);
}






void ResourceBoss::updateAndDraw(sf::RenderWindow &t_win,float &time,float &dt){
    if(m_takenDamageAnimationTimer>0){m_takenDamageAnimationTimer--;}
    for(Lighthouse &l:m_lighthouses){
        l.update(time,dt);
        l.draw(t_win);
    }
    for(Warehouse &w:m_warehouses){
        w.update(time,dt);
        w.draw(t_win);
    }
    for(Rocket &r:m_rockets){
        r.update(time,dt);
        r.draw(t_win);
    }
    if(m_waveTimer>0){
        m_waveTimer-=dt;
        if(time>=m_enemySpawnTime){
            m_enemySpawnTime=time+getEnemySpawnInterval();
            newEnemy(m_tMap,this);
        }
    }

    if(m_waveTimer<=0 && !m_waveEnded && m_enemies->size()==0){m_waveTimer=0;endWave();}
    else if(m_waveEnded && m_enemies->size()==0 && inControl){
        float warningSize=(((sin(time*5)+2)/2)*20)+80;
        sf::RectangleShape warning=sf::RectangleShape(sf::Vector2f(warningSize,warningSize));
        warning.setOrigin(warningSize/2,warningSize/2);
        warning.setFillColor(sf::Color::Red);
        warning.setOutlineColor(sf::Color::Red);
        sf::Vector2f warningPos=sf::Vector2f(-warningSize,-warningSize);
        switch(m_enemySpawnDirNextWave){
            case N:
                warningPos.x=(colNo*tileSize)/2;
                break;
            case E:
                warningPos.x=colNo*tileSize+warningSize;
                warningPos.y=(rowNo*tileSize)/2;
                break;
            case S:
                warningPos.x=(colNo*tileSize)/2;
                warningPos.y=rowNo*tileSize+warningSize;
                break;
            case W:
                warningPos.y=(rowNo*tileSize)/2;
                break;
        }
        warning.setPosition(warningPos);
        t_win.draw(warning);

        sf::Text exclamation=sf::Text();
        exclamation.setPosition(sf::Vector2f(warningPos.x-warningSize/12,warningPos.y-warningSize/6));
        exclamation.setOutlineColor(sf::Color::White);
        exclamation.setFillColor(sf::Color::White);
        exclamation.setFont(font);
        exclamation.setCharacterSize(warningSize/3);
        exclamation.setString("!");

        t_win.draw(exclamation);
    }
}


void ResourceBoss::printBuildings() {
    printf("LIGHTHOUSES\n");
    for(auto &l:m_lighthouses){
        sf::Vector2i coords=m_tMap->getTileCoordsFromId(l.m_tileParent->id);
        printf("one at %i,%i\n",coords.x,coords.y);
    }
    printf("WAREHOUSES\n");
    for(auto &w:m_warehouses){
        sf::Vector2i coords=m_tMap->getTileCoordsFromId(w.m_tileParent->id);
        printf("one at %i,%i\n",coords.x,coords.y);
    }
}