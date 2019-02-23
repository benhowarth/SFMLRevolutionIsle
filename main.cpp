#include <iostream>
#include <algorithm>
#include <deque>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>




#include "tools.h"
// x range
int const colNo=50;
//y range
int const rowNo=40;



const int screenWidth=800;
const int screenHeight=600;


namespace vec {
    float Length(sf::Vector2f _vec){
        float _x=_vec.x;
        float _y=_vec.y;
        return std::sqrt((_x*_x)+(_y*_y));
    }

    sf::Vector2f Normalise(sf::Vector2f _vec) {
        sf::Vector2f _norm;
        float len=Length(_vec);
        _norm=_vec/len;
        return _norm;
    }
}

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Revolution Isle",sf::Style::Close);
sf::View view=window.getDefaultView();

#include "tiles.h"
#include "resources.h"
#include "buildings.h"
#include "ui.h"
#include "click.h"




#include "entities.h"

int main(int argc, const char * argv[])
{
    srand(time(NULL));

    TileMap tMap(TINY_ISLAND,colNo,rowNo,&window);
    ResourceBoss res(&tMap,&friends,&enemies);

    //SFML Setup
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Clock clock;
    sf::Clock deltaClock;
    sf::Font font;
    if (!font.loadFromFile("Unique.ttf"))
    {
        // error...
    }
    int mouseX=window.getSize().x/2;
    int mouseY=window.getSize().y/2;
    int mouseXView=window.getSize().x/2;
    int mouseYView=window.getSize().y/2;
    //how close the mouse needs to get to the screen edge to move the view
    int screenMovePixelBuffer=50;
    float viewZoom=1;


    sf::Text timeText;

    std::string tileImageFilePath="img/revolution_tiles.png";
    sf::Texture tileTexture;
    if(!tileTexture.loadFromFile(tileImageFilePath)){
        printf("Can't load lovely looking tiles.");
    }
    tileTexture.setSmooth(false);
    tileTexture.setRepeated(false);

    std::string charImageFilePath="img/char_tiles.png";
    sf::Texture charTexture;
    if(!charTexture.loadFromFile(charImageFilePath)){
        printf("Can't load character.");
    }
    charTexture.setSmooth(false);
    charTexture.setRepeated(false);

    bool printTileIds=false;
    sf::Text tileIdText;


    tileMapNoX=tileTexture.getSize().x/tileSize;
    tileMapNoY=tileTexture.getSize().y/tileSize;
    //printf("%i\n",tileMapNoX*tileMapNoY);


    tile.setTexture(tileTexture);

    charSprite.setTexture(charTexture);


    bool running=true;


    sf::RectangleShape brush(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
    brush.setFillColor(sf::Color::Transparent);
    brush.setOutlineThickness(2.0);
    brush.setOutlineColor(sf::Color::White);



     // run the program as long as the window is open
    while (window.isOpen())
    {

        if(window.hasFocus()) {running=true;}else{running=false;}

            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                    window.close();

                // Escape key: exit
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Return) {
                        //g.generate_grid();
                    }
                    if (event.key.code == sf::Keyboard::N) {
                        newFriend(tMap);
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        mouseMode=DESTROY;
                        //brushTile = WATER;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        //brushTile = ISLAND;
                        mouseMode=PLACE_LAND;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        //brushTile = ROAD;
                        mouseMode=PLACE_ROAD;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num4) {
                        //brushTile = WAREHOUSE;
                        mouseMode=BUILD_WAREHOUSE;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num5) {
                        //brushTile = LIGHTHOUSE;
                        mouseMode=BUILD_LIGHTHOUSE;
                        brushSize = 1;
                    }/*
                    if (event.key.code == sf::Keyboard::Num6) {
                        //brushTile = DOCK;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num7) {
                        brushTile = ROADWATER;
                        brushSize = 1;
                    }*/
                    if (event.key.code == sf::Keyboard::Num0) {
                        //brushTile = NILTILE;
                        mouseMode=CONTROL_FRIENDS;
                        brushSize = 1;
                    }

                }

                if (event.type == sf::Event::MouseMoved) {
                    mouseXView=event.mouseMove.x;
                    mouseYView=event.mouseMove.y;
                    sf::Vector2f worldMouseCoords=window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x,event.mouseMove.y));
                    mouseX = worldMouseCoords.x;
                    mouseY = worldMouseCoords.y;
                }
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        //scaleFactor.x += event.mouseWheelScroll.delta * 0.1;
                        //scaleFactor.y += event.mouseWheelScroll.delta * 0.1;
                        viewZoom-=event.mouseWheelScroll.delta*0.1;
                        view.zoom(1-(event.mouseWheelScroll.delta*0.1));
                        window.setView(view);

                    }
                }

            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && running) {
                Click(1,mouseX, mouseY, &tMap);
            }else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && running) {
                Click(2, mouseX, mouseY, &tMap);
            } else {
                oldTileClick = sf::Vector2i(-100, -100);
                selectionBoxOn=false;
            }

            //move view
            if(running) {
                sf::Vector2f viewOffset = sf::Vector2f(0, 0);
                if (mouseXView < screenMovePixelBuffer) {
                    viewOffset.x = -(screenMovePixelBuffer - mouseXView);
                } else if (mouseXView > screenWidth - screenMovePixelBuffer) {
                    viewOffset.x = screenMovePixelBuffer - (screenWidth - mouseXView);
                }
                if (mouseYView < screenMovePixelBuffer) {
                    viewOffset.y = -(screenMovePixelBuffer - mouseYView);
                } else if (mouseYView > screenHeight - screenMovePixelBuffer) {
                    viewOffset.y = screenMovePixelBuffer - (screenHeight - mouseYView);
                }
                viewOffset.x *= 0.1;
                viewOffset.y *= 0.1;

                view.move(viewOffset);
                window.setView(view);

            }

            //window.clear(sf::Color::Black);
            window.clear(sf::Color(84,118,155));




            float secs = clock.getElapsedTime().asSeconds();
            float dt=deltaClock.restart().asSeconds();
            tMap.draw(secs);


            updateAndDrawFriends(secs);
            res.updateAndDraw(window,secs,dt);

            //draw brush


            brush.setSize(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
            sf::Vector2i brushPos = tMap.getTileCoordsFromPos(sf::Vector2f(mouseX, mouseY),brushSize);
            brushPos.x *= tileSize * scaleFactor.x;
            brushPos.y *= tileSize * scaleFactor.y;
            if(brushPos.x>=0&&brushPos.x<colNo*tileSize*scaleFactor.x&&brushPos.y>=0&&brushPos.y<rowNo*tileSize*scaleFactor.y) {
                brush.setPosition(sf::Vector2f(brushPos.x,brushPos.y));
            }
            if(!selectionBoxOn) {
                window.draw(brush);
            }

            if(selectionBoxOn) {
                sf::RectangleShape selectionBox = sf::RectangleShape(selectionBoxEnd - selectionBoxStart);
                selectionBox.setOutlineThickness(2);
                selectionBox.setPosition(selectionBoxStart);
                selectionBox.setFillColor(sf::Color::Transparent);
                selectionBox.setOutlineColor(sf::Color::White);
                window.draw(selectionBox);
            }

            //draw circles
            sf::CircleShape selectCirc(16);
            selectCirc.setOutlineColor(sf::Color::White);
            selectCirc.setFillColor(sf::Color::Transparent);
            selectCirc.setOutlineThickness(1);
            selectCirc.setOrigin(16,16);
            for (int i = 0; i < selectedFriends.size(); i++) {
                Entity *f = &friends[selectedFriends[i]];
                sf::Vector2f fPos = (*f).m_pos;
                selectCirc.setPosition(fPos);
                window.draw(selectCirc);
            }


            std::string brushText;
            /*
            switch (brushTile) {
                case WATER:
                    brushText = "WATER";
                    break;
                case ISLAND:
                    brushText = "ISLAND";
                    break;
                case ROAD:
                    brushText = "ROAD";
                    break;
                case WAREHOUSE:
                    brushText = "WAREHOUSE";
                    break;
                case LIGHTHOUSE:
                    brushText = "LIGHTHOUSE";
                    break;
                case DOCK:
                    brushText = "DOCK";
                    break;
                case ROADWATER:
                    brushText = "ROADWATER";
                    break;
                default:
                    brushText = "UNKNOWN";
            }*/
            switch(mouseMode){
                case DESTROY: brushText="DESTROY";break;
                case PLACE_LAND: brushText="PLACE_LAND";break;
                case PLACE_ROAD: brushText="PLACE_ROAD";break;
                case BUILD_LIGHTHOUSE: brushText="BUILD_LIGHTHOUSE";break;
                case BUILD_WAREHOUSE: brushText="BUILD_WAREHOUSE";break;
                case CONTROL_FRIENDS: brushText="CONTROL_FRIENDS";break;
                default: brushText="?";
            }
            timeText.setString(brushText + "\n" + std::to_string(clock.getElapsedTime().asSeconds()));
            timeText.setFont(font);
            timeText.setCharacterSize(24);
            timeText.setFillColor(sf::Color::White);
            timeText.setPosition(10, 10);
            window.draw(timeText);



            window.display();

    }

    return 0;
}

