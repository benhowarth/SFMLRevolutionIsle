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
int const colNo=60;
//y range
int const rowNo=60;


sf::Font font;

/*
const int screenWidth=800;
const int screenHeight=600;
*/

const int screenWidth=1000;
const int screenHeight=800;


namespace vec {
    float Length(sf::Vector2f t_vec){
        float _x=t_vec.x;
        float _y=t_vec.y;
        return std::sqrt((_x*_x)+(_y*_y));
    }
    float Length(sf::Vector2i t_vec){
        float _x=t_vec.x;
        float _y=t_vec.y;
        return std::sqrt((_x*_x)+(_y*_y));
    }

    sf::Vector2f Normalise(sf::Vector2f t_vec) {
        sf::Vector2f norm;
        float len=Length(t_vec);
        norm=t_vec/len;
        return norm;
    }
    sf::Vector2f Normalise(sf::Vector2i t_vec) {
        sf::Vector2f norm;
        float len=Length(t_vec);
        norm=sf::Vector2f(t_vec)/len;
        return norm;
    }

    sf::Vector2f Lerp(sf::Vector2f t_vec1,sf::Vector2f t_vec2,float t_ratio){
        /*sf::Vector2f norm=Normalise(t_vec2-t_vec1);
        float len=Length(t_vec2-t_vec1)*t_ratio;
        return t_vec1+(norm*len);*/
        return ((1-t_ratio)*t_vec1+t_ratio*t_vec2);
    }
}

float getRandom(float min,float max){
    return (((float) rand() / ((float) RAND_MAX))*(max-min))+min;
}

float getRandom(){
    return getRandom(0,1);
}

sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Revolution Isle",sf::Style::Close);
sf::View view=window.getDefaultView();
sf::View viewUI=sf::View(sf::FloatRect(0,0,screenWidth,screenHeight));

int const ISLAND_PRICE=10;

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
    UIBoss ui(&window,&view,&viewUI);
    ResourceBoss res(&tMap,&friends,&enemies,&ui);

    //SFML Setup
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Clock clock;
    sf::Clock deltaClock;
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
        printf("Can't load lovely looking tiles.\n");
    }
    tileTexture.setSmooth(false);
    tileTexture.setRepeated(false);

    std::string charImageFilePath="img/char_tiles.png";
    sf::Texture charTexture;
    if(!charTexture.loadFromFile(charImageFilePath)){
        printf("Can't load character texture.\n");
    }
    charTexture.setSmooth(false);
    charTexture.setRepeated(false);

    std::string enemyImageFilePath="img/char_tiles_debug.png";
    sf::Texture enemyTexture;
    if(!enemyTexture.loadFromFile(enemyImageFilePath)){
        printf("Can't load enemy texture.\n");
    }
    enemyTexture.setSmooth(false);
    enemyTexture.setRepeated(false);

    bool printTileIds=false;
    sf::Text tileIdText;


    tileMapNoX=tileTexture.getSize().x/tileSize;
    tileMapNoY=tileTexture.getSize().y/tileSize;
    //printf("%i\n",tileMapNoX*tileMapNoY);


    tile.setTexture(tileTexture);

    charSprite.setTexture(charTexture);
    enemySprite.setTexture(enemyTexture);


    bool running=true;
    sf::Vector2f viewPos=sf::Vector2f(screenWidth/2,screenHeight/2);


    sf::RectangleShape brush(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
    brush.setFillColor(sf::Color::Transparent);
    brush.setOutlineThickness(1.0);
    sf::Color brushCol=sf::Color::White;



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

                bool shiftPressed=false;
                if (event.type == sf::Event::KeyPressed) {
                    if(event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift ){shiftPressed=true;}

                    if (event.key.code == sf::Keyboard::Return) {
                        res.tryToGoToNextWave();
                    }
                    if (event.key.code == sf::Keyboard::Delete) {
                        res.endWave();
                    }

                    if (event.key.code == sf::Keyboard::N) {
                        newFriend(&tMap,&res);
                    }
                    if (event.key.code == sf::Keyboard::M) {
                        newEnemy(&tMap,&res);
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        toggleSelectFriend(0,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        toggleSelectFriend(1,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        toggleSelectFriend(2,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num4) {
                        toggleSelectFriend(3,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num5) {
                        toggleSelectFriend(4,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num6) {
                        toggleSelectFriend(5,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num7) {
                        toggleSelectFriend(6,!shiftPressed);
                    }
                    if(event.key.code == sf::Keyboard::Num8){
                        toggleSelectFriend(7,!shiftPressed);
                    }
                    if(event.key.code == sf::Keyboard::Num9){
                        toggleSelectFriend(8,!shiftPressed);
                    }
                    if (event.key.code == sf::Keyboard::Num0) {
                        toggleSelectFriend(9,!shiftPressed);
                    }

                    if(event.key.code == sf::Keyboard::E){
                        mouseMode=CONTROL_FRIENDS;
                        brushSize = 1;
                    }
                    if(event.key.code == sf::Keyboard::Q){
                        mouseMode=DESTROY;
                        brushSize = 2;
                    }
                    if(event.key.code == sf::Keyboard::W){
                        mouseMode=PLACE_LAND;
                        brushSize = 2;
                    }
                    if(event.key.code == sf::Keyboard::A){
                        mouseMode=PLACE_ROAD;
                        brushSize = 1;
                    }
                    if(event.key.code == sf::Keyboard::S){
                        mouseMode=BUILD_WAREHOUSE;
                        brushSize = 2;
                    }
                    if(event.key.code == sf::Keyboard::D){
                        mouseMode=BUILD_LIGHTHOUSE;
                        brushSize = 1;
                    }

                    if(event.key.code == sf::Keyboard::Up){enemySpawnDir=N;}
                    if(event.key.code == sf::Keyboard::Right){enemySpawnDir=E;}
                    if(event.key.code == sf::Keyboard::Down){enemySpawnDir=S;}
                    if(event.key.code == sf::Keyboard::Left){enemySpawnDir=W;}


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
                        float zoomToAdd=-event.mouseWheelScroll.delta*0.1;
                        if(viewZoom+zoomToAdd>0.2 && viewZoom+zoomToAdd<1.7) {
                            viewZoom += zoomToAdd;
                            view.zoom(1 + zoomToAdd);
                            window.setView(view);
                        }

                    }
                }

            }


            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && running) {
                Click(1,mouseX, mouseY, &tMap,&res);
            }else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && running) {
                //res.printBuildings();
                Click(2, mouseX, mouseY, &tMap,&res);
            } else {
                oldTileClick = sf::Vector2i(-100, -100);
                /*for(int x=0;x<2;x++){
                    for(int y=0;y<2;y++){
                        Hover(mouseX+x, mouseY+y, &tMap,&res);
                    }
                }*/
                Hover(mouseX, mouseY, &tMap,&res);
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

                if(viewPos.x+viewOffset.x<0 || viewPos.x+viewOffset.x>screenWidth){viewOffset.x=0;}
                if(viewPos.y+viewOffset.y<0 || viewPos.y+viewOffset.y>screenHeight){viewOffset.y=0;}

                view.move(viewOffset);
                viewPos+=viewOffset;
                window.setView(view);

            }



            float secs = clock.getElapsedTime().asSeconds();
            float dt=deltaClock.restart().asSeconds();

            //window.clear(sf::Color::Black);
            //window.clear(sf::Color(84,118,155));
            window.clear(colorMult(sf::Color(84,118,155),0.7));

            sf::Vector2f playAreaSize=sf::Vector2f(colNo*tileSize,rowNo*tileSize);
            float bgRectGradInterval=40+(sin(secs)*4);
            sf::RectangleShape bgRectGrad;
            for(int i=5;i>0;i--){
                bgRectGrad.setFillColor(colorMult(sf::Color(84,118,155),1-(0.05*i)));
                sf::Vector2f thisGradSize=sf::Vector2f(playAreaSize.x+(i*bgRectGradInterval),playAreaSize.y+(i*bgRectGradInterval));
                bgRectGrad.setSize(thisGradSize);
                bgRectGrad.setOrigin(thisGradSize.x/2,thisGradSize.y/2);
                bgRectGrad.setPosition(playAreaSize.x/2,playAreaSize.y/2);
                window.draw(bgRectGrad);
            }


            tMap.draw(secs);


            updateAndDrawFriends(secs,dt,&res);
            updateAndDrawEnemies(secs,dt,&res);
            res.updateAndDraw(window,secs,dt);

            //draw brush


            brush.setSize(sf::Vector2f(tileSize * brushSize * scaleFactor.x, tileSize * brushSize * scaleFactor.y));
            sf::Vector2i brushPos = tMap.getTileCoordsFromPos(sf::Vector2f(mouseX, mouseY),brushSize);
            brushPos.x *= tileSize * scaleFactor.x;
            brushPos.y *= tileSize * scaleFactor.y;
            if(brushPos.x>=0&&brushPos.x<colNo*tileSize*scaleFactor.x&&brushPos.y>=0&&brushPos.y<rowNo*tileSize*scaleFactor.y) {
                brush.setPosition(sf::Vector2f(brushPos.x,brushPos.y));
            }
            if(!selectionBoxOn && mouseMode!=CONTROL_FRIENDS) {
                brushCol=sf::Color::White;
                if(mouseMode==PLACE_LAND) {
                    if (!res.hasPlayerFinishedWave() || res.getResources()<ISLAND_PRICE) {
                        brushCol = sf::Color::Red;
                    }
                }
                if((mouseMode==BUILD_WAREHOUSE && res.getResources()<Warehouse::cost)||(mouseMode==BUILD_LIGHTHOUSE && res.getResources()<Lighthouse::cost)){

                    brushCol = sf::Color::Red;
                }
                brush.setOutlineColor(brushCol);
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
            timeText.setString(brushText + "\nResources: "+std::to_string(res.getResources())+ "\nHP: "+std::to_string(res.getHP())+ "\n" + std::to_string(clock.getElapsedTime().asSeconds()));
            timeText.setFont(font);
            timeText.setCharacterSize(24);
            timeText.setFillColor(sf::Color::White);
            timeText.setPosition(10, 50);
            //ui.drawOnUI(timeText);

            timeText.setFillColor(sf::Color::Blue);
            timeText.setPosition(10,80);
            timeText.setString(std::to_string(res.getResources())+"R");
            ui.drawOnUI(timeText);

            std::string waveBarString="Wave: "+std::to_string(res.getCurrentWave())+" ("+std::to_string(res.getWaveFactor())+")";
            sf::Color waveBarCol=colorMult(sf::Color::Green,0.7);
            if(res.getWavePercentageComplete()>=1 && enemies.size()==0){
                if(res.getCurrentWave()==0){
                    waveBarString="Press Enter to start Wave "+std::to_string(res.getCurrentWave()+1);
                }else {
                    waveBarString ="Wave "+std::to_string(res.getCurrentWave())+" complete! Press Enter to start Wave "+std::to_string(res.getCurrentWave() + 1);
                }
            }else{
                waveBarCol=colorMult(sf::Color::Blue,1.2);
            }


            ui.drawHealthBar(res.getHP(),100,sf::Vector2f(screenWidth/2,20),sf::Vector2f(screenWidth-20,30),sf::Color::White,sf::Color::Red,true,"METEOR HEALTH");
            ui.drawHealthBar(res.getWavePercentageComplete(),1,sf::Vector2f(screenWidth/2,50),sf::Vector2f(screenWidth-20,30),sf::Color::White,waveBarCol,true,waveBarString);

            ui.draw(dt);


        window.display();

    }

    return 0;
}

