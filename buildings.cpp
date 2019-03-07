//
// Created by Ben on 22/02/2019.
//

#include <vector>
#include "tools.h"
#include "buildings.h"
#include "entities.h"
class Enemy;



void Lighthouse::draw(sf::RenderWindow &win){
    sf::Vector2f progressBarPos=sf::Vector2f(m_pos);
    progressBarPos.x+=tileSize/2;
    progressBarPos.y-=tileSize*4;

    if(m_percentageBuilt>=1) {
        float rad=getRadius();
        if(m_showUI) {

            sf::Text levelText;
            levelText.setFont(font);
            levelText.setString(std::to_string(inhabitants.size()));
            levelText.setOutlineColor(sf::Color::Green);
            levelText.setFillColor(sf::Color::Green);
            levelText.setPosition(m_pos.x,m_pos.y-tileSize*7);
            levelText.setCharacterSize(15);
            win.draw(levelText);

            sf::CircleShape shape = sf::CircleShape(rad);
            shape.setOutlineColor(sf::Color::Green);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(0.5);
            shape.setPosition(m_pos.x, m_pos.y);
            shape.setOrigin(rad - tileSize / 2, rad - tileSize / 2);
            win.draw(shape);
            float timerDisplay=m_timer;
            if(m_timer>getTimeInterval()){timerDisplay=getTimeInterval();}
            m_res->ui->drawHealthBar(timerDisplay,getTimeInterval(),progressBarPos,sf::Vector2f(20,10),sf::Color::White,sf::Color::Green);
        }


        for (Bullet bullet:m_bullets) {
            bullet.draw(win);
        }



    }else{
        if(m_showUI) {
            m_res->ui->drawHealthBar(m_percentageBuilt, 1, progressBarPos, sf::Vector2f(20, 10), sf::Color::White,sf::Color::White);
        }
    }

};

void Lighthouse::doAction(){
    //m_bullets.reserve(1);
    //m_bullets.emplace_back(sf::Vector2f(m_pos),m_damage,m_bulletSpeed,sf::Vector2f(0,0));
    //printf("Lighthouse at %i,%i (tile %i,%i) does action (new action in %f)\n",m_pos.x,m_pos.y,m_coords.x,m_coords.y,m_timeInterval);
    int enemyNo=m_enemies->size();
    for(int i=0;i<enemyNo;i++){
        Enemy* e=&m_enemies->at(i);
        float dist=vec::Length(sf::Vector2f(m_pos)-(e->m_pos));
        if(dist<=getRadius()){
            //shoot
            m_bullets.reserve(1);
            m_bullets.emplace_back(sf::Vector2f(m_pos.x,m_pos.y-tileSize*3),m_damage+(m_damageIncrement*inhabitants.size()),m_bulletSpeed,e->m_pos);
            m_timer=0;
            break;
        }
    }
};

float Lighthouse::getTimeInterval() {
    return m_timeInterval-(m_timeUpgrade*inhabitants.size());
}

void Lighthouse::personalUpdate(float &time, float &dTime) {
    for(auto b=m_bullets.begin();b!=m_bullets.end();){
        b->update(time,dTime);
        if(b->m_lifeSpan>0) {
            bool bulletDestroy=false;
            int enemyNo = m_enemies->size();
            for (int i = 0; i < enemyNo; i++) {
                Enemy *e = &m_enemies->at(i);
                float dist = vec::Length(b->m_pos - e->m_pos);
                if (dist < b->m_rad) {
                    e->addHP(-b->m_damage);
                    //destroy bullet
                    bulletDestroy=true;
                }

            }
            if(bulletDestroy){
                b=m_bullets.erase(b);
            }else{
                ++b;
            }
        }else{
            //destroy bullet
            b=m_bullets.erase(b);
        }
    }
}

void Warehouse::draw(sf::RenderWindow &win){
    if(m_showUI) {

        if(m_percentageBuilt>=1) {
            sf::Text levelText;
            levelText.setFont(font);
            levelText.setString(std::to_string(inhabitants.size()));
            levelText.setOutlineColor(sf::Color::Blue);
            levelText.setFillColor(sf::Color::Blue);
            levelText.setPosition(m_pos.x + tileSize / 2, m_pos.y - tileSize *4);
            levelText.setCharacterSize(15);
            win.draw(levelText);
        }

        sf::Vector2f progressBarPos = sf::Vector2f(m_pos);
        progressBarPos.x += tileSize;
        progressBarPos.y -= tileSize;
        if (m_percentageBuilt < 1) {
            m_res->ui->drawHealthBar(m_percentageBuilt, 1, progressBarPos, sf::Vector2f(20, 10), sf::Color::White,
                                     sf::Color::White);
        } else {
            float timerDisplay=m_timer;
            if(m_timer>getTimeInterval()){timerDisplay=getTimeInterval();}
            m_res->ui->drawHealthBar(timerDisplay, getTimeInterval(), progressBarPos, sf::Vector2f(20, 10), sf::Color::White,
                                     sf::Color::Blue);
        }
    }
};

void Warehouse::doAction(){
    //printf("Warehouse at %i,%i (tile %i,%i) does action (new action in %f)\n",m_pos.x,m_pos.y,m_coords.x,m_coords.y,m_timeInterval);
    m_res->addResources(1);
    m_timer=0;
    m_res->ui->addPopup(m_pos.x,m_pos.y,"+1R",sf::Color::Blue);
};

float Warehouse::getTimeInterval() {
    return m_timeInterval-(m_timeUpgrade*inhabitants.size());
}

void Warehouse::personalUpdate(float &time, float &dTime) {

}

void Building::doAction() {
    printf("Building %i,%i does work!\n", m_coords.x, m_coords.y);
};

void Building::update(float &time,float &dTime){
    //check if built
    if(enemies.size()>0) {
        if (m_percentageBuilt + dTime < 1) {
            m_percentageBuilt += m_buildSpeed * dTime;
        } else if (m_percentageBuilt + dTime > 1 && m_percentageBuilt != 1) {
            m_percentageBuilt = 1;
            m_res->ui->addPopup(m_pos.x, m_pos.y, "DONE!", sf::Color::White);
            m_showUI = false;
        } else {
            if (m_timer > getTimeInterval()) {
                doAction();
            } else {
                m_timer += dTime;
            }
        }
        personalUpdate(time, dTime);
    }

};

Building::~Building(){

    /*for(Entity* inhabitant:inhabitants){
        if(inhabitant) {
            inhabitant->detachFromBuilding();
        }
    }*/

    //printf("building with parent %x destroyed\n",m_tileParent);
}


void Bullet::draw(sf::RenderWindow &win) {
    sf::Color col=sf::Color::Green;
    if(m_lifeSpan<0.5){
        col=sf::Color(col.r,col.g,col.b,(m_lifeSpan/0.5)*255);
    }
    sf::CircleShape shape=sf::CircleShape(m_rad);
    shape.setOrigin(m_rad,m_rad);
    shape.setFillColor(col);
    shape.setOutlineColor(col);
    shape.setPosition(sf::Vector2f(m_pos));
    win.draw(shape);
}

void Bullet::update(float &time, float &dTime) {
    m_lifeSpan-=dTime;
    m_pos+=m_vel;
}
