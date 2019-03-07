//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_CLICK_H
#define SFMLREVOLUTIONISLE_CLICK_H

#include "resources.h"
#include "entities.h"

class Entity;

enum MouseMode{
    DESTROY,
    PLACE_LAND,
    PLACE_ROAD,
    BUILD_LIGHTHOUSE,
    BUILD_WAREHOUSE,
    CONTROL_FRIENDS
};

extern MouseMode mouseMode;

extern sf::Vector2i oldTileClick;
extern sf::Vector2i oldTileHover;
void Click(int button,int _x,int _y,TileMap* tMap,ResourceBoss* res);
void drawBrush(int button,TileMap* tMap,sf::Vector2i tileClick);
void Hover(int _x,int _y,TileMap* tMap,ResourceBoss* res);

#endif //SFMLREVOLUTIONISLE_CLICK_H
