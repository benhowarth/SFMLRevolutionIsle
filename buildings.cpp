//
// Created by Ben on 22/02/2019.
//

#include "buildings.h"

void Building::doAction() {

};

void Lighthouse::draw(sf::RenderWindow &win){

};

void Lighthouse::doAction(){

};


void Warehouse::draw(sf::RenderWindow &win){

};

void Warehouse::doAction(){

};


void Building::update(float &time,float &dTime){
    //check if built
    if(m_percentageBuilt>=1){
        m_percentageBuilt+=m_buildSpeed*dTime;
    }else{
        if(time>m_timer){
            doAction();
            m_timer=time+m_timeInterval;
        }
    }

};



