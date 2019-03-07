//
// Created by Ben on 12/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_UI_H
#define SFMLREVOLUTIONISLE_UI_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

sf::Color colorMult(const sf::Color &col,const float &mult);

struct UIPopupStruct{
    float x,y;
    std::string text;
    float timer;
    float timerMax=3;
    float speed=20;
    sf::Color col;
    bool onUILayer=false;
    UIPopupStruct(const float &t_x,const float &t_y,const std::string &t_text,const sf::Color &t_col,const bool &t_onUILayer):x(t_x),y(t_y),text(t_text),col(t_col),onUILayer(t_onUILayer){timer=timerMax;};
};

class UIBoss{
private:
    sf::RenderWindow* m_window;
    sf::View* m_view;
    sf::View* m_viewUI;
    float m_buttonSize=50;
    std::vector<UIPopupStruct> m_popups;
public:
    UIBoss(sf::RenderWindow* t_window,sf::View* t_view,sf::View* t_viewUI):m_window(t_window),m_view(t_view),m_viewUI(t_viewUI){};
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill);
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer);
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer,const std::string &str);
    void drawOnUI(const sf::Drawable &toDraw);
    void drawButton(const std::string &str,const sf::Vector2f &pos,const sf::Vector2f &size,const float &charSize,const sf::Color &colFill,const sf::Color &colOutline);
    void drawButtons();
    void addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col,const bool &onUILayer);
    void addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col);
    void drawPopups(float &dt);
    void draw(float &dt);
};



#endif //SFMLREVOLUTIONISLE_UI_H
