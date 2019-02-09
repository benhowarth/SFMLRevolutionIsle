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

#include "entities.h"

sf::Vector2i oldTileClick;
void Click(int button,int _x,int _y,TileMap* tMap){
    sf::Vector2i tileClick=(*tMap).getTileCoordsFromPos(sf::Vector2f(_x,_y),brushSize);
    int tileId=(*tMap).getTileIdFromPos(sf::Vector2f(_x,_y),brushSize);
    //printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    if((_x>=0 && _x<=colNo*tileSize*scaleFactor.x) && (_y>=0 && _y<=rowNo*tileSize*scaleFactor.y) && tileId>-1 && tileId<rowNo*colNo+1) {




        if(tileClick!=oldTileClick){
            //printf("Click at %i,%i TILE: %f, %f (id %i)\n",_x,_y,tileClick.x,tileClick.y,tileId);
            oldTileClick=tileClick;
            //printf("clicked tile %f,%f\n", tileClick.x, tileClick.y);


            for (int y = 0; y < brushSize; y++) {
                for (int x = 0; x < brushSize; x++) {
                    sf::Vector2i curTileCoords=tileClick;
                    curTileCoords.x+=x;
                    curTileCoords.y+=y;
                    int tileId=(*tMap).getTileIdFromCoords(curTileCoords);
                    Tile tileVal=(*(*tMap).getTileFromId(tileId)).basicTile;
                    //Tile *tile = &tMap[(int) (((tileClick.y + y) * colNo) + tileClick.x + x)].basicTile;
                    if(button==1) {
                        //selectionBoxOn=false;
                        if (brushTile == WATER) {
                            (*tMap).updateTile(tileId,WATER);
                        } else if (brushTile == ISLAND) {
                            (*tMap).updateTile(tileId,ISLAND);
                        } else if (brushTile == ROAD) {
                            if (tileVal == ISLAND) {
                                (*tMap).updateTile(tileId,ROAD);
                            }
                        } else if (brushTile == WAREHOUSE) {
                            if (tileVal == ISLAND || tileVal == ROAD) {
                                (*tMap).updateTile(tileId,WAREHOUSE);
                            } else if (tileVal == WAREHOUSE) {
                                (*tMap).updateTile(tileId,ISLAND);
                            }
                        } else if (brushTile == LIGHTHOUSE) {
                            if (tileVal == ISLAND || tileVal == ROAD) {
                                (*tMap).updateTile(tileId,LIGHTHOUSE);
                            } else if (tileVal == LIGHTHOUSE) {
                                (*tMap).updateTile(tileId,ISLAND);
                            }
                        } else if (brushTile == DOCK) {
                            if (tileVal == WATER) {
                                (*tMap).updateTile(tileId,DOCK);
                            } else if (tileVal == DOCK) {
                                (*tMap).updateTile(tileId,WATER);
                            }

                        } else if (brushTile == ROADWATER) {
                            if (tileVal == WATER) {
                                (*tMap).updateTile(tileId,ROADWATER);
                            } else if (tileVal == ROADWATER) {
                                (*tMap).updateTile(tileId,WATER);
                            }
                        }else if(brushTile==NILTILE) {
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
                    }else if(button==2){

                        if (brushTile == NILTILE) {
                            for (auto f = selectedFriends.begin(); f != selectedFriends.end(); ++f) {
                                friends[*f].tryPath(tileId);
                            }
                        }

                    }
                }

            }
            //end of loops


        }
    }
}




int main(int argc, const char * argv[])
{
    srand(time(NULL));

    TileMap tMap(CIRCLE_ISLAND,colNo,rowNo,&window);

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



    friends.push_back(Entity(&tMap));

    friends.push_back(Entity(&tMap,screenWidth*0.35,screenHeight*0.35,1));

    friends.push_back(Entity(&tMap,screenWidth*0.45,screenHeight*0.45,4));


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




            int secs = clock.getElapsedTime().asSeconds();
            tMap.draw(secs);


            //shape.setRadius(10);
            //shape.setOrigin(10,10);
            //shape.setPosition(mouseX,mouseY);
            //window.drawShape

            for(auto f=friends.begin();f!=friends.end();++f) {
                (*f).moveToTarget();
                (*f).draw(&window);
            }


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



            window.display();

    }

    return 0;
}

