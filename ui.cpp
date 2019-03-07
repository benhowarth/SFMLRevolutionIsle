//
// Created by Ben on 24/02/2019.
//

#include "ui.h"
#include "entities.h"
#include "click.h"

sf::Color colorMult(const sf::Color &col,const float &mult){
    return sf::Color(col.r*mult,col.g*mult,col.b*mult);
}


void UIBoss::drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer,const std::string &str){
    if(onUILayer) {
        m_window->setView(*m_viewUI);
    }
    //draw outline
    pos.x-=size.x/2;
    pos.y-=size.y/2;
    sf::RectangleShape r=sf::RectangleShape(size);
    r.setPosition(pos);
    r.setFillColor(sf::Color::Transparent);
    r.setOutlineThickness(1);
    r.setOutlineColor(colOutline);
    m_window->draw(r);
    //draw fill
    //printf("%f/%f final size:%f \n",currentHP,maxHP,size.x*(currentHP/maxHP));
    r=sf::RectangleShape(sf::Vector2f(size.x*(currentHP/maxHP),size.y));
    r.setPosition(pos);
    r.setFillColor(colFill);
    r.setOutlineColor(sf::Color::Transparent);
    m_window->draw(r);

    if(str!=""){
        sf::Text barText;
        barText.setString(str);
        barText.setFont(font);
        barText.setCharacterSize(size.y*0.8);
        barText.setPosition(pos.x,pos.y);
        barText.setOutlineColor(colOutline);
        barText.setFillColor(colOutline);
        m_window->draw(barText);
    }

    if(onUILayer) {
        m_window->setView(*m_view);
    }
}

void UIBoss::drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer) {
    drawHealthBar(currentHP,maxHP,pos,size,colOutline,colFill,onUILayer,"");
}

void UIBoss::drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill){
    drawHealthBar(currentHP,maxHP,pos,size,colOutline,colFill,false,"");
}

void UIBoss::drawOnUI(const sf::Drawable &toDraw) {
    m_window->setView(*m_viewUI);
    m_window->draw(toDraw);
    m_window->setView(*m_view);
}

void UIBoss::drawButton(const std::string &str,const sf::Vector2f &pos,const sf::Vector2f &size,const float &charSize,const sf::Color &colFill,const sf::Color &colOutline){
    sf::RectangleShape button=sf::RectangleShape(size);
    button.setPosition(pos);
    button.setOutlineThickness(0.5);
    button.setOutlineColor(colOutline);
    button.setFillColor(colFill);
    button.setOrigin(0,0);

    sf::Text buttonText;
    buttonText.setPosition(pos);
    buttonText.setString(str);
    buttonText.setFillColor(colOutline);
    buttonText.setCharacterSize(17);
    buttonText.setFont(font);

    m_window->draw(button);
    m_window->draw(buttonText);
}

void UIBoss::drawButtons(){
    m_window->setView(*m_viewUI);

    float bufferSize=(screenWidth-(m_buttonSize*10))/10;



    for(int i=0;i<friends.size();i++){
        std::string buttonString="Idlin'";
        if(friends[i].m_buildingParent){buttonString="Workin'";}
        else if(friends[i].m_targets.size()>1){buttonString="Movin'";}
        sf::Vector2f newPos=sf::Vector2f(bufferSize/2+(i*(m_buttonSize+bufferSize)),screenHeight-m_buttonSize-bufferSize);
        sf::Color fill=sf::Color::Magenta;
        sf::Color outline=sf::Color::Transparent;
        for(int j=0;j<selectedFriends.size();j++){
            if(selectedFriends[j]==i){outline=sf::Color::White;break;}
        }
        drawButton(buttonString,newPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);
    }

    std::string buttonString;
    sf::Vector2f buttonPos;
    sf::Color fillNormal=colorMult(sf::Color::Blue,0.9);
    sf::Color fill=fillNormal;
    sf::Color outline=sf::Color::Transparent;

    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*3,80);
    buttonString="Destroy\n(Q)";
    if(mouseMode==DESTROY){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);


    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*2,80);
    buttonString="Land\n(W)";
    if(enemies.size()>0) {
        fill = sf::Color::Red;
    }else{
        fill = fillNormal;
    }

    if(mouseMode==PLACE_LAND){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);


    fill = fillNormal;

    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*1,80);
    buttonString="Ducks\n(E)";
    if(mouseMode==CONTROL_FRIENDS){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);

    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*3,80+m_buttonSize);
    buttonString="Road\n(A)";
    if(mouseMode==PLACE_ROAD){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);


    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*2,80+m_buttonSize);
    buttonString="Warehouse\n(S)";
    if(mouseMode==BUILD_WAREHOUSE){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);

    buttonPos=sf::Vector2f(screenWidth-m_buttonSize*1,80+m_buttonSize);
    buttonString="Lighthouse\n(D)";
    if(mouseMode==BUILD_LIGHTHOUSE){
        outline=sf::Color::Green;
    }else{
        outline=sf::Color::White;
    }
    drawButton(buttonString,buttonPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);


    m_window->setView(*m_view);
}

void UIBoss::addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col,const bool &onUILayer){
    m_popups.reserve(1);
    m_popups.emplace_back(x,y,text,col,true);
}

void UIBoss::addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col) {
    m_popups.reserve(1);
    m_popups.emplace_back(x,y,text,col,false);
}


void UIBoss::drawPopups(float &dt) {
    sf::Text popupText;
    popupText.setFont(font);
    int count=0;
    for(auto popup=m_popups.begin();popup!=m_popups.end();){
        if(popup->timer>0) {
            //printf("draw popup %i\t",count);
            sf::Color col=popup->col;
            float alpha=255;
            if(popup->timer<popup->timerMax*0.6){
                alpha=((popup->timer/popup->timerMax))*255;
            }
            col=sf::Color(col.r,col.g,col.b,alpha);
            sf::Color col2=sf::Color(255,255,255,alpha);
            popupText.setOutlineColor(col2);
            popupText.setFillColor(col);
            popupText.setPosition(popup->x, popup->y);
            popupText.setString(popup->text);
            popup->timer -= dt;
            popup->y-=dt*popup->speed;
            if(popup->onUILayer){
                popupText.setCharacterSize(22);
                drawOnUI(popupText);
            }else {
                popupText.setCharacterSize(12);
                m_window->draw(popupText);
            }

            //printf("pos(%f,%f)\t",popup->x,popup->y);
            //printf("timer : %f\n",popup->timer);
            ++popup;
        }else{
            popup=m_popups.erase(popup);
        }
        ++count;
    }
}

void UIBoss::draw(float &dt){
    drawButtons();
    drawPopups(dt);
}