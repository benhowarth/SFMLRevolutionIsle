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
//#include <aStar.h>







#include "tiles.h"

//Graph g;


int screenWidth=800;
int screenHeight=600;
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

#include "entities.h"

sf::Vector2f oldTileClick;
void Click(int button,int _x,int _y,Graph *g){
    sf::Vector2f tileClick=GetTileFromPos(sf::Vector2f(_x,_y));
    int tileId=GetTileIdFromPos(sf::Vector2f(_x,_y));
    //printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    if((_x>=0 && _x<=colNo*tileSize*scaleFactor.x) && (_y>=0 && _y<=rowNo*tileSize*scaleFactor.y) && tileId>-1 && tileId<rowNo*colNo+1) {

        if(brushTile==NILTILE) {
            if (!selectionBoxOn) {
                selectionBoxOn = true;
                selectionBoxStart = sf::Vector2f(_x, _y);
                selectionBoxEnd = sf::Vector2f(_x, _y);
            } else {
                selectionBoxEnd = sf::Vector2f(_x, _y);
                selectedFriends.clear();
                //check box
                for (int i = 0; i < friends.size(); i++) {
                    Entity *f = &friends[i];
                    sf::Vector2f fPos = (*f).m_pos;
                    if (((fPos.x > selectionBoxStart.x && fPos.x < selectionBoxEnd.x) ||
                         (fPos.x < selectionBoxStart.x && fPos.x > selectionBoxEnd.x))
                        && (fPos.y > selectionBoxStart.y && fPos.y < selectionBoxEnd.y) ||
                        (fPos.y < selectionBoxStart.y && fPos.y > selectionBoxEnd.y)) {
                        selectedFriends.push_back(i);
                    }
                }
            }
        }


        if(tileClick!=oldTileClick){
            printf("Click at %i,%i TILE: %f, %f (id %i)\n",_x,_y,tileClick.x,tileClick.y,tileId);
            oldTileClick=tileClick;
            printf("clicked tile %f,%f\n", tileClick.x, tileClick.y);
            //displayMap[3*((tileClickY*colNo)+tileClickX)+1]*=-1;


            for (int y = 0; y < brushSize; y++) {
                for (int x = 0; x < brushSize; x++) {
                    //int *tile = &map[(int) (((tileClick.y + y) * colNo) + tileClick.x + x)];
                    Tile *tile = &tMap[(int) (((tileClick.y + y) * colNo) + tileClick.x + x)].basicTile;
                    if(button==1) {
                        //selectionBoxOn=false;
                        if (brushTile == WATER) {
                            *tile = WATER;
                        } else if (brushTile == ISLAND) {
                            *tile = ISLAND;
                        } else if (brushTile == ROAD) {
                            if (*tile == ISLAND) {
                                *tile = ROAD;
                            }
                        } else if (brushTile == WAREHOUSE) {
                            if (*tile == ISLAND || *tile == ROAD) {
                                *tile = WAREHOUSE;
                            } else if (*tile == WAREHOUSE) {
                                *tile = ISLAND;
                            }
                        } else if (brushTile == LIGHTHOUSE) {
                            if (*tile == ISLAND || *tile == ROAD) {
                                *tile = LIGHTHOUSE;
                            } else if (*tile == LIGHTHOUSE) {
                                *tile = ISLAND;
                            }
                        } else if (brushTile == DOCK) {
                            if (*tile == WATER) {
                                *tile = DOCK;
                            } else if (*tile == DOCK) {
                                *tile = WATER;
                            }

                        } else if (brushTile == ROADWATER) {
                            if (*tile == WATER) {
                                *tile = ROADWATER;
                            } else if (*tile == ROADWATER) {
                                *tile = WATER;
                            }
                        } else if (brushTile == NILTILE) {



                        }
                    }else if(button==2){
                        if (brushTile == NILTILE) {

                            /*
                            for (auto f = friends.begin(); f != friends.end(); ++f) {
                                (*f).TryPath(tileId, g);
                            }
                            */
                            for (auto f = selectedFriends.begin(); f != selectedFriends.end(); ++f) {
                                friends[*f].TryPath(tileId, g);
                            }
                        }
                        //selectionBoxOn=false;
                    }
                    (*g).UpdateGraph(tMap);
                }

            }
            CalculateDisplayMap();
        }
    }
}




int main(int argc, const char * argv[])
{
    srand(time(NULL));



    //SFML Setup
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("Unique.ttf"))
    {
        // error...
    }
    int mouseX=window.getSize().x/2;
    int mouseY=window.getSize().y/2;
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


    /*
    int map[colNo*rowNo]= {
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,15,0,0,0,
            0,0,2,4,4,4,23,2,0,0,
            0,0,10,11,11,11,31,10,0,0,
            0,0,10,11,11,11,39,10,0,0,
            0,0,10,11,11,11,11,10,0,0,
            0,0,18,19,20,21,21,18,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    */
    /*for(int i=0;i<tileMapNoX*tileMapNoY;i++){
        map[i]=i;
    }*/
    const int islandBuffer=6;
    for(int i=0;i<colNo*rowNo;i++){
        map[i]=0;
        if(i%colNo<islandBuffer || i%colNo>(colNo-1)-islandBuffer || i/colNo<islandBuffer || i/colNo>(colNo-1)-islandBuffer){
            tMap[i]=TileStruct{WATER,0,1,1};
        }else{
            tMap[i]=TileStruct{ISLAND,0,1,1};
        }
        //tMap[i]=TileStruct{ISLAND,0,1,1};

    }

    CalculateDisplayMap();
    Graph g(colNo,rowNo,tMap);
    bool running=true;



    friends.push_back(Entity());

    friends.push_back(Entity(screenWidth*0.35,screenHeight*0.35,1));

    friends.push_back(Entity(screenWidth*0.45,screenHeight*0.45,4));

    sf::RectangleShape brush(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
    brush.setFillColor(sf::Color::Transparent);
    brush.setOutlineThickness(2.0);
    brush.setOutlineColor(sf::Color::White);

    sf::CircleShape shape(10);
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

                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        brushTile = WATER;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        brushTile = ISLAND;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        brushTile = ROAD;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num4) {
                        brushTile = WAREHOUSE;
                        brushSize = 2;
                    }
                    if (event.key.code == sf::Keyboard::Num5) {
                        brushTile = LIGHTHOUSE;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num6) {
                        brushTile = DOCK;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num7) {
                        brushTile = ROADWATER;
                        brushSize = 1;
                    }
                    if (event.key.code == sf::Keyboard::Num0) {
                        brushTile = NILTILE;
                        brushSize = 1;
                    }

                }

                if (event.type == sf::Event::MouseMoved) {
                    mouseX = event.mouseMove.x;
                    mouseY = event.mouseMove.y;
                }
                if (event.type == sf::Event::MouseWheelScrolled) {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        scaleFactor.x += event.mouseWheelScroll.delta * 0.1;
                        scaleFactor.y += event.mouseWheelScroll.delta * 0.1;
                    }
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && running) {
                Click(1,mouseX, mouseY, &g);
            }else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && running) {
                Click(2, mouseX, mouseY, &g);
            } else {
                oldTileClick = sf::Vector2f(-100, -100);
                selectionBoxOn=false;
            }

            //window.clear(sf::Color::Black);
        window.clear(sf::Color(84,118,155));

            for (int y = 0; y < rowNo; y++) {
                for (int x = 0; x < colNo; x++) {
                    //int mapVal=map[((y*colNo)+x)];
                    Tile mapVal = tMap[((y * colNo) + x)].basicTile;
                    //int tileVal=displayMap[displayParamNo*((y*colNo)+x)];
                    int tileVal = tMap[((y * colNo) + x)].displayTile;
                    int secs = clock.getElapsedTime().asSeconds();
                    if (secs % 2 == 0) {
                        switch (tileVal) {
                            case 4:
                                tileVal = 3;
                                break;
                        }
                    }
                    //int tileScaleX=displayMap[displayParamNo*((y*colNo)+x)+1];
                    //int tileScaleY=displayMap[displayParamNo*((y*colNo)+x)+2];
                    int tileScaleX = tMap[(y * colNo) + x].sx;
                    int tileScaleY = tMap[(y * colNo) + x].sy;
                    DrawTileAt(window,tileVal, x, y, tileScaleX, tileScaleY);
                    if (tileVal == 24 || tileVal == 25 || tileVal == 51 || tileVal == 52) {
                        int newTileVal = 17;
                        int newX = x;
                        int newY = y - 1;
                        if (tileVal == 51 || tileVal == 52) {
                            newTileVal = 16;
                            newX = x - 1;
                            newY = y;
                            DrawTileAt(window,newTileVal, newX, newY, tileScaleX, tileScaleY);
                        } else {
                            DrawTileAt(window,newTileVal, newX, newY, tileScaleX, tileScaleY);
                            newTileVal = 1;
                            newX = x;
                            newY = y + 1;
                            DrawTileAt(window,newTileVal, newX, newY, tileScaleX, tileScaleY);
                        }


                    }
                    if (printTileIds) {
                        tileIdText.setString(std::to_string(mapVal));
                        //tileIdText.setString("("+std::to_string(x)+","+std::to_string(y)+")");
                        tileIdText.setFont(font);
                        tileIdText.setCharacterSize(10);
                        tileIdText.setFillColor(sf::Color::Red);
                        tileIdText.setPosition(x * tileSize * scaleFactor.x, y * tileSize * scaleFactor.y);
                        window.draw(tileIdText);
                    }


                }
            }


            //shape.setRadius(10);
            //shape.setOrigin(10,10);
            //shape.setPosition(mouseX,mouseY);
            //window.drawShape

            for(auto f=friends.begin();f!=friends.end();++f) {
                (*f).moveToTarget();
                (*f).Draw(window);
            }


            //draw brush


            brush.setSize(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
            sf::Vector2f brushPos = GetTileFromPos(sf::Vector2f(mouseX, mouseY));
            brushPos.x *= tileSize * scaleFactor.x;
            brushPos.y *= tileSize * scaleFactor.y;
            if(brushPos.x>=0&&brushPos.x<colNo*tileSize*scaleFactor.x&&brushPos.y>=0&&brushPos.y<rowNo*tileSize*scaleFactor.y) {
                brush.setPosition(brushPos);
            }
            window.draw(brush);

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
            }
            timeText.setString(brushText + "\n" + std::to_string(clock.getElapsedTime().asSeconds()));
            timeText.setFont(font);
            timeText.setCharacterSize(24);
            timeText.setFillColor(sf::Color::White);
            timeText.setPosition(10, 10);
            window.draw(timeText);

            /*
            timeText.setString(std::to_string());
            timeText.setFont(font);
            timeText.setCharacterSize(24);
            timeText.setFillColor(sf::Color::White);
            timeText.setPosition(10,10);
            window.draw(timeText);
            */





        window.display();

    }

    return 0;
}

