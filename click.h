//
// Created by Ben on 22/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_CLICK_H
#define SFMLREVOLUTIONISLE_CLICK_H

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
void Click(int button,int _x,int _y,TileMap* tMap);
void drawBrush(int button,TileMap* tMap,sf::Vector2i tileClick);

#endif //SFMLREVOLUTIONISLE_CLICK_H
