//
// Created by Ben on 23/02/2019.
//


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <vector>

#include "tiles.h"
#include "buildings.h"
#include "resources.h"
#include "entities.h"
#include "tools.h"

sf::Sprite charSprite;
sf::Sprite enemySprite;

void drawCharAt(sf::Sprite* t_sprite,sf::RenderWindow &win,int charDir,int x,int y,float scaleX,float scaleY){
    sf::Sprite sprite=*t_sprite;
    sprite.setOrigin(0,0);
    sprite.setTextureRect(sf::IntRect(charDir*tileSize,0,tileSize,tileSize));
    sprite.setOrigin(tileSize/2,tileSize/2);
    sprite.setScale(scaleFactor.x*scaleX,scaleFactor.y*scaleY);
    sprite.setOrigin(0,0);
    sprite.setPosition(x-(scaleX*scaleFactor.x*tileSize)/2,y-(scaleY*scaleFactor.y*tileSize)/2);
    win.draw(sprite);
}

void Entity::draw(sf::RenderWindow* win) {
    if(!invisible) {

        if (m_targets.size() > 1) {


            sf::Vertex pathLine[m_targets.size() + 1];
            pathLine[0] = sf::Vertex(m_tPos);
            pathLine[0].color = sf::Color::Red;
            //for(auto target=targets.begin();target!=targets.end();++target){
            for (int i = 0; i < m_targets.size(); i++) {
                //circ.setPosition(*target);
                //circ.setFillColor(sf::Color::Red);
                //win.draw(circ);
                pathLine[i + 1] = sf::Vertex(m_targets[i]);
                pathLine[i + 1].color = sf::Color::Red;
            }
            (*win).draw(pathLine, m_targets.size() + 1, sf::LinesStrip);
        }

        drawCharAt(m_sprite,(*win), getVelDirInt(), m_pos.x, m_pos.y, 2, 2);
        m_res->ui->drawHealthBar(*(getHP()),hpMax,sf::Vector2f(m_pos.x,m_pos.y-tileSize*1.5),sf::Vector2f(25,5),sf::Color::White,sf::Color::Red);
    }
}

bool Entity::addHP(float t_hp){
    if(t_hp>0){
        m_res->ui->addPopup(m_pos.x,m_pos.y,"+"+std::to_string((int)t_hp),sf::Color::Green);
    }else{
        m_res->ui->addPopup(m_pos.x,m_pos.y,std::to_string((int)t_hp),sf::Color::Red);
    }
    float* hp=getHP();
    //printf("add %f to %f\n",t_hp,*hp);
    if((*hp)+t_hp>0 && (*hp)+t_hp<=hpMax) {
        *hp = *hp+t_hp;
        //printf("\taddhp currentHp: %f\n",*hp);
        return true;
    }else if((*hp)+t_hp>hpMax){
        *hp=hpMax;
        //printf("\tatmax currentHp: %f\n",*hp);
        return true;
    }else{
        m_res->ui->addPopup(m_pos.x,m_pos.y,"DEAD",sf::Color::Red);
        *hp=0;
        markForDeletion();
        //printf("\tdead currentHp: %f\n",*hp);
        return false;
    }
}

void Entity::moveToTarget(){
    float speed=m_speed;
    if(getCurrentTile()->basicTile==ROAD||getCurrentTile()->basicTile==ROADWATER){
        speed*=s_roadSpeedBoost;
    }

    if(vec::Length(m_tPos-m_pos)>speed){
        //printf("%f\n",vec::Length(m_tPos-m_pos));
        m_vel=vec::Normalise(m_tPos-m_pos)*speed;
        m_pos+=m_vel;
    }else{
        nextTarget();
    }
    //printf("%f,%f\n",m_pos.x,m_pos.y);
}
void Entity::update(float time,float dt){

}
TileStruct* Entity::getCurrentTile(){
    return m_tMap->getTileFromId(m_tMap->getTileIdFromPos(m_tPos,1));
}

void Entity::attachToBuilding(Building* curBuild) {
    m_buildingParent = curBuild;
    m_buildingParent->addInhabitant(this);
    invisible = true;
    m_targets.clear();
}

void Entity::detachFromBuilding() {
    m_buildingParent->removeInhabitant(this);
    m_buildingParent=nullptr;
    invisible = false;
}


bool Entity::tryPath(int tileId) {
    if(m_buildingParent){detachFromBuilding();}
    int curTileId = (*m_tMap).getTileIdFromPos(m_pos,1);
    //printf("from id:%i to id:%i\n", curTileId, tileId);
    //int curTileId=GetTileIdFromPos(f.targets[0]);
    if (curTileId != tileId) {
        //std::vector<int> path = (*g).GetPath(curTileId, tileId, tMap);
        std::vector<int> path = (*m_tMap).getPath(curTileId, tileId, m_pathMask);

        /*
        printf("PATH: ");
        for (int p = 0; p < path.size(); p++) {
            printf("%i, ", path[p]);
        }
        printf("\n");
        //printf("%i to %i\n",curTileId,tileClickId);
        */


        m_targets.clear();
        for (int p = 0; p < path.size(); p++) {
            int tilePathId = path[p];
            sf::Vector2f tilePos = sf::Vector2f((tilePathId % colNo) * tileSize * scaleFactor.x,
                                                (tilePathId / colNo) * tileSize * scaleFactor.y);
            tilePos.x += scaleFactor.x * (tileSize / 4);
            tilePos.y += scaleFactor.y * (tileSize / 4);
            addTarget(tilePos);
        }

        if(path.size()<2){return false;}else{return true;}
        //f.addTarget(sf::Vector2f(tileClick.x*tileSize*scaleFactor.x,tileClick.y*tileSize*scaleFactor.y));
    }
}

Friend::Friend(TileMap* tMap,ResourceBoss* res,int t_x,int t_y):Entity(tMap,res,t_x,t_y,1){
    m_pathMask=&tMask_island_general;
    m_sprite=&charSprite;
    res->ui->addPopup(t_x,t_y,"NEW DUCK!",colorMult(sf::Color::Green,0.8));
};

void Friend::update(float time,float dt){
    if(m_targets.size()>0) {
        moveToTarget();
    }else if(!m_buildingParent){
        //printf("not in building\n");
        sf::Vector2f testPos=m_tPos;
        testPos.x+=tileSize/2;
        testPos.y+=tileSize/2;
        sf::Vector2i coords=m_tMap->getTileCoordsFromPos(testPos,1);
        Building* curBuild=m_res->getBuildingByCoords(coords);
        //Building* curBuild=nullptr;
        if(curBuild) {
            m_pos.x+=tileSize*1.25;

            attachToBuilding(curBuild);
            //printf("got building\n");
        }
        else if(time>idleMoveTime) {
            sf::Vector2f newTPos = m_pos;
            Tile tileToGo = WATER;
            int tries = 0;
            while (tileToGo == WATER && tries < 2) {
                newTPos = m_pos;
                float rx = ((float) rand() / ((float) RAND_MAX)) * 10 - 5;
                float ry = ((float) rand() / ((float) RAND_MAX)) * 10 - 5;
                //printf("rand x:%f y:%f\n", rx, ry);
                //printf("%f,%f\n", m_pos.x, m_pos.y);
                newTPos.x += rx;
                newTPos.y += ry;
                //printf("%f,%f\n", newTPos.x, newTPos.y);
                tileToGo = m_tMap->getTileFromId(m_tMap->getTileIdFromPos(newTPos, 1))->basicTile;
                tries++;
            }
            addTarget(newTPos);

            double rtime = ((double) rand() / ((double) RAND_MAX) * 4) + 2;
            //printf("time now %f vs time to next move %f\n",time,time+rtime);
            idleMoveTime = time + rtime;
        }

    }else{
        //if in building
        //printf("in building\n");
        /*if(m_targets.size()>0){
            detachFromBuilding();
        }*/
    }
}

void Enemy::update(float time,float dt){

    if(m_pos.x<screenWidth && m_pos.x>0 && m_pos.y<screenHeight && m_pos.y>0) {
        if(m_tMap->getTileFromPos(m_pos,1)->basicTile==WATER) {
            //get nearest island tile
            setTarget(sf::Vector2f(m_tMap->getNearestTileAt(m_pos,ISLAND)));
            moveToTarget();
       }else{
            if (vec::Length(m_tMap->getMiddleTilePos() - sf::Vector2i(m_pos)) > tileSize * 2) {
                if (m_targets.size() > 0) {
                    moveToTarget();
                } else {
                    if(tryPath(m_tMap->getMiddleTileId())) {
                        nextTarget();
                    }else{
                        setTarget(sf::Vector2f(m_tMap->getMiddleTilePos()));
                    }
                }
            } else {
                if (time > damageTime) {
                    //if meteor can be damaged, damage it and reset timer
                    if (m_res->addHP(-damage)) {
                        damageTime = time + damageTimeInterval;
                    } else {
                        //meteor is dead
                    }
                }
            }

        }
    }else{
        m_tPos=sf::Vector2f(m_tMap->getMiddleTilePos());
    }


}

std::vector<Friend> friends;
std::vector<Enemy> enemies;

void newFriend(TileMap* tMap,ResourceBoss* res){
    if(friends.size()<10) {
        friends.reserve(1);
        friends.emplace_back(tMap, res, tileSize * colNo / 2, tileSize * rowNo / 2);
    }
}
float enemyRad=200;
CompassDir enemySpawnDir=N;

void newEnemy(TileMap* tMap,ResourceBoss* res){
    enemies.reserve(1);
    //work out coords
    /*
    float r = getRandom(0,2*M_PI);
    int x=tileSize * colNo / 2;
    int y= tileSize * rowNo / 2;
    x+=std::cos(r)*enemyRad;
    y+=std::sin(r)*enemyRad;
    */
    int x=0;
    int y=0;
    switch(enemySpawnDir){
        case N:
            x=getRandom(0,colNo*tileSize);
            y=0;
            break;
        case E:
            x=rowNo*tileSize;
            y=getRandom(0,rowNo*tileSize);
            break;
        case S:
            x=getRandom(0,colNo*tileSize);
            y=colNo*tileSize;
            break;
        case W:
            x=0;
            y=getRandom(0,rowNo*tileSize);
            break;
    }
    if(x<tileSize){x=tileSize;}else if(x>(colNo-1)*tileSize){x=(colNo-1)*tileSize;}

    if(y<tileSize){y=tileSize;}else if(y>(rowNo-1)*tileSize){y=(rowNo-1)*tileSize;}
    //printf("making %i,%i\n",x,y);
    //printf("%i:%i\n",screenWidth,screenHeight);

    enemies.emplace_back(tMap,res,x,y);
    //enemies.back().addTarget(sf::Vector2f(screenWidth/2,screenHeight/2));
}


bool selectionBoxOn;
sf::Vector2f selectionBoxStart;
sf::Vector2f selectionBoxEnd;
std::vector<int> selectedFriends;

void updateAndDrawFriends(float time,float dt,ResourceBoss* t_res) {
    if(friends.size()>0){
        int deletedFriends = 0;
        for (auto f = friends.begin(); f != friends.end();) {
            if (!f->isToDelete()) {
                f->update(time, dt);
                f->draw(&window);
                ++f;
            } else {
                //delete friend
                f = friends.erase(f);
                deletedFriends++;
            }
        }
        if (deletedFriends > 0) {
            friends.shrink_to_fit();
            //cleanup
        }
    }
}

void updateAndDrawEnemies(float time,float dt,ResourceBoss* t_res){
    if(enemies.size()>0) {
        int deletedEnemies = 0;
        for (auto e = enemies.begin(); e != enemies.end();) {
            if (!e->isToDelete()) {
                e->update(time, dt);
                e->draw(&window);
                ++e;
            } else {
                //delete enemy
                e = enemies.erase(e);
                deletedEnemies++;
            }
        }
        if (deletedEnemies > 0) {
            //cleanup
            enemies.shrink_to_fit();
        }
    }
}


void toggleSelectFriend(const int &i,bool clearRest){
    if(clearRest){selectedFriends.clear();}
    if(i<friends.size()) {
        int selectedAlreadyId = -1;
        for (int j=0;j<selectedFriends.size();j++) {
            if (selectedFriends[j] == i) {
                selectedAlreadyId = j;
                break;
            }
        }
        if (selectedAlreadyId > -1) {
            selectedFriends.erase(selectedFriends.begin() + selectedAlreadyId);
        } else {
            selectedFriends.push_back(i);
        }
    }
}
void toggleSelectFriend(const int &i){
    toggleSelectFriend(i,false);
}
