//
// Created by Ben on 06/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_ENTITIES_H
#define SFMLREVOLUTIONISLE_ENTITIES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "tiles.h"

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

class Entity{
private:
    float s_roadSpeedBoost=1.5;
public:
    sf::Vector2f m_tPos;
    sf::Vector2f m_pos;
    sf::Vector2f m_size=sf::Vector2f(tileSize,tileSize);
    sf::Vector2f m_vel=sf::Vector2f(0,0);;
    std::vector<sf::Vector2f> m_targets;
    float m_speed=2;
    TileMap* m_tMap;
public:
    Entity(TileMap* t_tMap){
        m_pos=sf::Vector2f(screenWidth*0.3,screenHeight*0.3);
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
    void draw(sf::RenderWindow* win){


        if(m_targets.size()>0) {
            sf::CircleShape circ = sf::CircleShape(10);
            //circ.setPosition(m_tPos);
            sf::Vector2f circPos=m_targets[m_targets.size() - 1];
            circPos.x-=5;
            circPos.y-=5;
            circ.setPosition(circPos);
            circ.setFillColor(sf::Color::Green);
            (*win).draw(circ);
        }

        sf::Vertex pathLine[m_targets.size()+1];
        pathLine[0]=sf::Vertex(m_tPos);
        pathLine[0].color=sf::Color::Red;
        //for(auto target=targets.begin();target!=targets.end();++target){
        for(int i=0;i<m_targets.size();i++){
            //circ.setPosition(*target);
            //circ.setFillColor(sf::Color::Red);
            //win.draw(circ);
            pathLine[i+1]=sf::Vertex(m_targets[i]);
            pathLine[i+1].color=sf::Color::Red;
        }
        (*win).draw(pathLine,m_targets.size()+1,sf::LinesStrip);

        /*
        sf::RectangleShape rect=sf::RectangleShape(m_pos);
        rect.setPosition(m_pos);
        rect.setSize(sf::Vector2f(m_size.x*scaleFactor.x,m_size.y*scaleFactor.y));
        rect.setFillColor(sf::Color::White);
        rect.setOrigin(m_size.x/2,m_size.y/2);
        win.draw(rect);
        */

        drawCharAt((*win),getVelDirInt(),m_pos.x,m_pos.y,1,1);

        //printf("angleVel: %f (dir:%i)\n",getVelAngle(),getVelDirInt());

    }
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
    void moveToTarget(){
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
    TileStruct* getCurrentTile(){
        return m_tMap->getTileFromId(m_tMap->getTileIdFromPos(m_tPos,1));
    }
    void tryPath(int tileId) {
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
};

//Entity f;
std::vector<Entity> friends;

bool selectionBoxOn;
sf::Vector2f selectionBoxStart;
sf::Vector2f selectionBoxEnd;
std::vector<int> selectedFriends;

#endif //SFMLREVOLUTIONISLE_ENTITIES_H
