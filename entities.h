//
// Created by Ben on 06/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_ENTITIES_H
#define SFMLREVOLUTIONISLE_ENTITIES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::Sprite charSprite;

void DrawCharAt(sf::RenderWindow &win,int charDir,int x,int y,float scaleX,float scaleY){
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
//private:
public:
    sf::Vector2f m_tPos;
    sf::Vector2f m_pos;
    sf::Vector2f m_size=sf::Vector2f(tileSize,tileSize);
    sf::Vector2f m_vel=sf::Vector2f(0,0);;
    std::vector<sf::Vector2f> targets;
    float speed=2;
public:
    Entity(){
        m_pos=sf::Vector2f(screenWidth*0.3,screenHeight*0.3);
        m_tPos=m_pos;
    }
    Entity(float _x,float _y){
        m_pos=sf::Vector2f(_x,_y);
        m_tPos=m_pos;
    }
    Entity(float _x,float _y,float _speed){
        m_pos=sf::Vector2f(_x,_y);
        m_tPos=m_pos;
        speed=_speed;
    }
    void Draw(sf::RenderWindow &win){


        if(targets.size()>0) {
            sf::CircleShape circ = sf::CircleShape(10);
            //circ.setPosition(m_tPos);
            sf::Vector2f circPos=targets[targets.size() - 1];
            circPos.x-=5;
            circPos.y-=5;
            circ.setPosition(circPos);
            circ.setFillColor(sf::Color::Green);
            win.draw(circ);
        }

        sf::Vertex pathLine[targets.size()+1];
        pathLine[0]=sf::Vertex(m_tPos);
        pathLine[0].color=sf::Color::Red;
        //for(auto target=targets.begin();target!=targets.end();++target){
        for(int i=0;i<targets.size();i++){
            //circ.setPosition(*target);
            //circ.setFillColor(sf::Color::Red);
            //win.draw(circ);
            pathLine[i+1]=sf::Vertex(targets[i]);
            pathLine[i+1].color=sf::Color::Red;
        }
        win.draw(pathLine,targets.size()+1,sf::LinesStrip);

        /*
        sf::RectangleShape rect=sf::RectangleShape(m_pos);
        rect.setPosition(m_pos);
        rect.setSize(sf::Vector2f(m_size.x*scaleFactor.x,m_size.y*scaleFactor.y));
        rect.setFillColor(sf::Color::White);
        rect.setOrigin(m_size.x/2,m_size.y/2);
        win.draw(rect);
        */

        DrawCharAt(win,getVelDirInt(),m_pos.x,m_pos.y,1,1);

        //printf("angleVel: %f (dir:%i)\n",getVelAngle(),getVelDirInt());

    }
    float getVelAngle(){
        return std::atan2(m_vel.x,m_vel.y);
    }
    int getVelDirInt(){
        return ((int)std::round((getVelAngle()*8)/(2*3.14)+8))%8;
    }
    void setVel(float x,float y){
        m_vel.x=x;
        m_vel.y=y;
    }
    void addTarget(sf::Vector2f _tar){
        targets.push_back(_tar);
    }
    void nextTarget(){

        if(!targets.empty()) {
            sf::Vector2f nextTar = targets.front();
            targets.erase(targets.begin());
            m_tPos.x = nextTar.x;
            m_tPos.y = nextTar.y;
        }
    }
    void setTarget(float x,float y){
        m_tPos.x=x;
        m_tPos.y=y;
    }
    void setTarget(sf::Vector2f _newPos){
        m_tPos=_newPos;
    }
    void moveToTarget(){
        if(vec::Length(m_tPos-m_pos)>speed){
            //printf("%f\n",vec::Length(m_tPos-m_pos));
            m_vel=vec::Normalise(m_tPos-m_pos)*speed;
            m_pos+=m_vel;
        }else{
            nextTarget();
        }
        //printf("%f,%f\n",m_pos.x,m_pos.y);
    }
    void TryPath(int tileId,Graph* g) {
        int curTileId = GetTileIdFromPos(m_tPos);
        printf("from id:%i to id:%i\n", curTileId, tileId);
        //int curTileId=GetTileIdFromPos(f.targets[0]);
        if (curTileId != tileId) {
            std::vector<int> path = (*g).GetPath(curTileId, tileId, tMap);
            printf("PATH: ");
            for (int p = 0; p < path.size(); p++) {
                printf("%i, ", path[p]);
            }
            printf("\n");
            //printf("%i to %i\n",curTileId,tileClickId);
            targets.clear();
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
