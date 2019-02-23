//
// Created by Ben on 23/02/2019.
//


#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "tiles.h"
#include "entities.h"
#include "tools.h"

sf::Sprite charSprite;

void drawCharAt(sf::RenderWindow &win,int charDir,int x,int y,float scaleX,float scaleY){
    charSprite.setOrigin(0,0);
    charSprite.setTextureRect(sf::IntRect(charDir*tileSize,0,tileSize,tileSize));
    charSprite.setOrigin(tileSize/2,tileSize/2);
    charSprite.setScale(scaleFactor.x*scaleX,scaleFactor.y*scaleY);
    charSprite.setOrigin(0,0);
    //charSprite.setPosition((x-(scaleX-1)/2)*tileSize*scaleFactor.x,(y-(scaleY-1)/2)*tileSize*scaleFactor.y);
    charSprite.setPosition(x-(scaleX*scaleFactor.x*tileSize)/2,y-(scaleY*scaleFactor.y*tileSize)/2);
    win.draw(charSprite);
}

void Entity::draw(sf::RenderWindow* win) {


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

    drawCharAt((*win), getVelDirInt(), m_pos.x, m_pos.y, 1, 1);
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
void Entity::update(float time){

    if(m_targets.size()>0) {
        moveToTarget();
    }else{
        if(time>idleMoveTime){
            sf::Vector2f newTPos=m_pos;
            Tile tileToGo=WATER;
            int tries=0;
            while(tileToGo==WATER && tries<2) {
                newTPos = m_pos;
                float rx = ((float) rand() / ((float) RAND_MAX))*10-5;
                float ry = ((float) rand() / ((float) RAND_MAX))*10-5;
                //printf("rand x:%f y:%f\n", rx, ry);
                //printf("%f,%f\n", m_pos.x, m_pos.y);
                newTPos.x += rx;
                newTPos.y += ry;
                //printf("%f,%f\n", newTPos.x, newTPos.y);
                tileToGo=m_tMap->getTileFromId(m_tMap->getTileIdFromPos(newTPos,1))->basicTile;
                tries++;
            }
            addTarget(newTPos);

            double rtime=((double)rand()/((double)RAND_MAX)*4)+2;
            //printf("time now %f vs time to next move %f\n",time,time+rtime);
            idleMoveTime=time+rtime;
        }
    }
}
TileStruct* Entity::getCurrentTile(){
    return m_tMap->getTileFromId(m_tMap->getTileIdFromPos(m_tPos,1));
}

void Entity::tryPath(int tileId) {
    int curTileId = (*m_tMap).getTileIdFromPos(m_tPos,1);
    printf("from id:%i to id:%i\n", curTileId, tileId);
    //int curTileId=GetTileIdFromPos(f.targets[0]);
    if (curTileId != tileId) {
        //std::vector<int> path = (*g).GetPath(curTileId, tileId, tMap);
        std::vector<int> path = (*m_tMap).getPath(curTileId, tileId, tMask_land);

        printf("PATH: ");
        for (int p = 0; p < path.size(); p++) {
            printf("%i, ", path[p]);
        }
        printf("\n");
        //printf("%i to %i\n",curTileId,tileClickId);
        m_targets.clear();
        for (int p = 0; p < path.size(); p++) {
            int tilePathId = path[p];
            sf::Vector2f tilePos = sf::Vector2f((tilePathId % colNo) * tileSize * scaleFactor.x,
                                                (tilePathId / colNo) * tileSize * scaleFactor.y);
            tilePos.x += scaleFactor.x * (tileSize / 4);
            tilePos.y += scaleFactor.y * (tileSize / 4);
            addTarget(tilePos);
        }
        //f.addTarget(sf::Vector2f(tileClick.x*tileSize*scaleFactor.x,tileClick.y*tileSize*scaleFactor.y));
    }
}

//Entity f;
std::vector<Entity> friends;
std::vector<Entity> enemies;

void newFriend(TileMap &tMap){
    friends.reserve(1);
    friends.emplace_back(&tMap,tileSize*colNo/2,tileSize*rowNo/2);
}

bool selectionBoxOn;
sf::Vector2f selectionBoxStart;
sf::Vector2f selectionBoxEnd;
std::vector<int> selectedFriends;

void updateAndDrawFriends(float time){
    for(auto f=friends.begin();f!=friends.end();++f) {
        //(*f).moveToTarget();
        (*f).update(time);
        (*f).draw(&window);
    }
}