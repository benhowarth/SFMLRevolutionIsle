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

struct UIButtonStruct{
    float x,y;
    float w,h;
    std::string text;
    sf::Color colOutline,colFill,colText;
    bool selected=false;
    bool disabled=false;
    float charSize;
    UIButtonStruct(const float &t_x,const float &t_y,const float &t_w,const float &t_h,const std::string &t_text,const float &t_charSize,const sf::Color &t_colOutline,const sf::Color &t_colFill,const sf::Color &t_colText):x(t_x),y(t_y),w(t_w),h(t_h),text(t_text),charSize(t_charSize),colOutline(t_colOutline),colFill(t_colFill),colText(t_colText){};
};

enum KeyboardButtonIndex{
    KEY_BUTTON_Q=0,
    KEY_BUTTON_W=1,
    KEY_BUTTON_E=2,
    KEY_BUTTON_A=3,
    KEY_BUTTON_S=4,
    KEY_BUTTON_D=5
};

class UIBoss{
private:
    sf::RenderWindow* m_window;
    sf::View* m_view;
    sf::View* m_viewUI;
    float m_buttonSize=50;
    std::vector<UIPopupStruct> m_popups;
public:
    std::vector<UIButtonStruct> m_buttons;
    UIBoss(sf::RenderWindow* t_window,sf::View* t_view,sf::View* t_viewUI):m_window(t_window),m_view(t_view),m_viewUI(t_viewUI){};
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill);
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer);
    void drawHealthBar(float currentHP,float maxHP,sf::Vector2f pos,sf::Vector2f size,sf::Color colOutline,sf::Color colFill,bool onUILayer,const std::string &str);
    void drawOnUI(const sf::Drawable &toDraw);
    void addButton(const UIButtonStruct &btn);
    void drawButton(const UIButtonStruct &btn);
    //void drawButton(buttonString,newPos,sf::Vector2f(m_buttonSize,m_buttonSize),17,fill,outline);
    void initButtons();
    bool checkButtonsClick(const float &x,const float &y);
    void drawButtons();
    void addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col,const bool &onUILayer);
    void addPopup(const float &x,const float &y,const std::string &text,const sf::Color &col);
    void drawPopups(float &dt);
    void draw(float &dt);
};



#endif //SFMLREVOLUTIONISLE_UI_H
