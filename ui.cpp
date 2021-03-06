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
        barText.setCharacterSize(size.y*0.5);
        barText.setPosition(pos.x+3,pos.y+3);
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

void UIBoss::addButton(const UIButtonStruct &btn){
    m_buttons.push_back(btn);
}

void UIBoss::drawButton(const UIButtonStruct &btn){
    sf::RectangleShape button=sf::RectangleShape(sf::Vector2f(btn.w,btn.h));
    button.setPosition(sf::Vector2f(btn.x,btn.y));
    button.setOutlineThickness(0.5);

    sf::Text buttonText;

    if(btn.selected) {
        button.setOutlineColor(btn.colOutline);
        buttonText.setFillColor(btn.colOutline);
    }else{
        button.setOutlineColor(sf::Color::Transparent);
        buttonText.setFillColor(sf::Color::White);
    }
    if(btn.disabled) {
        button.setFillColor(sf::Color::Red);
    }else{
        button.setFillColor(btn.colFill);
    }
    button.setOrigin(0,0);
    buttonText.setPosition(sf::Vector2f(btn.x+3,btn.y+3));
    buttonText.setString(btn.text);
    buttonText.setCharacterSize(btn.charSize);
    buttonText.setFont(font);

    m_window->draw(button);
    m_window->draw(buttonText);
}


void UIBoss::initButtons(){
    std::string buttonString;
    sf::Vector2f buttonPos;
    sf::Color fillNormal=colorMult(sf::Color::Blue,0.9);
    sf::Color fill=fillNormal;
    sf::Color outline=sf::Color::Green;
    sf::Vector2f buttonSizeVec=sf::Vector2f(m_buttonSize*1.6,m_buttonSize);
    int buttonCharSize=10;
    float bufferSize=20;
    float bufferSize2=100;

    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*3,bufferSize2);
    buttonString="Destroy\n(Q)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));


    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*2,bufferSize2);
    buttonString="Land\n(W)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));


    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*1,bufferSize2);
    buttonString="Select\nDucks\n(E)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));

    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*3,bufferSize+bufferSize2+buttonSizeVec.y);
    buttonString="Road\n(A)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));


    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*2,bufferSize+bufferSize2+buttonSizeVec.y);
    buttonString="Ware\nhouse\n(S)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));

    buttonPos=sf::Vector2f(screenWidth-(bufferSize+buttonSizeVec.x)*1,bufferSize+bufferSize2+buttonSizeVec.y);
    buttonString="Light\nhouse\n(D)";
    //drawButton(buttonString,buttonPos,buttonSizeVec,buttonCharSize,fill,outline);
    addButton(UIButtonStruct(buttonPos.x,buttonPos.y,buttonSizeVec.x,buttonSizeVec.y,buttonString,buttonCharSize,outline,fill,outline));
}

bool UIBoss::checkButtonsClick(const float &x,const float &y){
    for(int i=0;i<m_buttons.size();i++) {
        UIButtonStruct &btn = m_buttons[i];
        if (!btn.disabled && x > btn.x && x < btn.x + btn.w && y > btn.y && y < btn.y + btn.h) {

            switch(i){
                case KEY_BUTTON_Q:
                    mouseMode=DESTROY;
                    brushSize = 2;
                    break;
                case KEY_BUTTON_W:
                    mouseMode=PLACE_LAND;
                    brushSize = 2;
                    break;
                case KEY_BUTTON_E:
                    mouseMode=CONTROL_FRIENDS;
                    brushSize = 1;
                    break;
                case KEY_BUTTON_A:
                    mouseMode=PLACE_ROAD;
                    brushSize = 1;
                    break;
                case KEY_BUTTON_S:
                    mouseMode=BUILD_WAREHOUSE;
                    brushSize = 2;
                    break;
                case KEY_BUTTON_D:
                    mouseMode=BUILD_LIGHTHOUSE;
                    brushSize = 1;
                    break;
            }
            return true;
        }
    }
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
        UIButtonStruct btn = UIButtonStruct(newPos.x,newPos.y,m_buttonSize,m_buttonSize,buttonString,17,outline,fill,outline);

        if (!btn.disabled && x > btn.x && x < btn.x + btn.w && y > btn.y && y < btn.y + btn.h) {
            toggleSelectFriend(i,true);
            return true;
        }
    }
    return false;
}

void UIBoss::drawButtons(){
    m_window->setView(*m_viewUI);

    float bufferSize=(screenWidth-(m_buttonSize*10))/10;



    for(int i=0;i<friends.size();i++){
        std::string buttonString="Idlin'";
        if(friends[i].m_saved){buttonString="Saved!";}
        else if(friends[i].m_buildingParent){buttonString="Workin'";}
        else if(friends[i].m_targets.size()>1){buttonString="Movin'";}
        sf::Vector2f newPos=sf::Vector2f(bufferSize/2+(i*(m_buttonSize+bufferSize)),screenHeight-m_buttonSize-bufferSize);
        sf::Color fill=sf::Color::Magenta;
        sf::Color outline=sf::Color::Green;
        UIButtonStruct btn = UIButtonStruct(newPos.x,newPos.y,m_buttonSize,m_buttonSize,buttonString,14,outline,fill,outline);
        for(int j=0;j<selectedFriends.size();j++){
            if(selectedFriends[j]==i){btn.selected=true;break;}
        }
        //drawButton(buttonString,newPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);
        drawButton(btn);
    }

    for(int i=0;i<m_buttons.size();i++){
        UIButtonStruct &btn=m_buttons[i];
        switch(i){
            case KEY_BUTTON_Q:
                btn.selected=(mouseMode==DESTROY);
                break;
            case KEY_BUTTON_W:
                btn.selected=(mouseMode==PLACE_LAND);
                btn.disabled=(enemies.size()>0);
                break;
            case KEY_BUTTON_E:
                btn.selected=(mouseMode==CONTROL_FRIENDS);
                break;
            case KEY_BUTTON_A:
                btn.selected=(mouseMode==PLACE_ROAD);
                break;
            case KEY_BUTTON_S:
                btn.selected=(mouseMode==BUILD_WAREHOUSE);
                break;
            case KEY_BUTTON_D:
                btn.selected=(mouseMode==BUILD_LIGHTHOUSE);
                break;
        }
        drawButton(btn);
    }



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