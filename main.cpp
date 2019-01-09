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

//changes map in place
//int[]


int main(int argc, const char * argv[])
{

    srand(time(NULL));
    int screenWidth=800;
    int screenHeight=600;


    //SFML Setup
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Revolution Isle",sf::Style::Close);
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

    bool printTileIds=true;
    sf::Text tileIdText;

    int tileSize=8;
    int tileMapNoX=tileTexture.getSize().x/tileSize;
    int tileMapNoY=tileTexture.getSize().y/tileSize;
    printf("%i",tileMapNoX*tileMapNoY);
    sf::Vector2f scaleFactor=sf::Vector2f(5.0f,5.0f);

    sf::Sprite tile;
    tile.setTexture(tileTexture);

    int rowNo=20;
    int colNo=10;
    /*
    int map[colNo*rowNo]= {
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,15,0,0,
            0,0,0,11,11,11,11,23,0,0,
            0,0,0,11,11,11,11,31,0,0,
            0,0,0,11,11,11,11,39,0,0,
            0,0,0,11,11,11,11,11,0,0,
            0,0,0,11,11,11,11,11,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
     */
    float map[colNo*rowNo]= {
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,15,0,0,0,
            0,0,2,4,4,4,23,2.75,0,0,
            0,0,10,11,11,11,31,10.5,0,0,
            0,0,10,11,11,11,39,10.5,0,0,
            0,0,10,11,11,11,11,10.5,0,0,
            0,0,18,19,20,21,21,18.25,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };

    /*for(int i=0;i<tileMapNoX*tileMapNoY;i++){
        map[i]=i;
    }*/

    int displayMap[colNo*rowNo];

    for(int i=0;i<colNo*rowNo;i++){
        displayMap[i]=map[i];
    }

    sf::CircleShape shape(10);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Return){
                    //g.generate_grid();
                }
                if(event.key.code==sf::Keyboard::N){

                }
            }

            if(event.type==sf::Event::MouseMoved){
                mouseX=event.mouseMove.x;
                mouseY=event.mouseMove.y;
            }
            if(event.type==sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button==sf::Mouse::Left){
                }
                if(event.mouseButton.button==sf::Mouse::Right) {
                }
            }
        }

        window.clear(sf::Color::Black);

        for(int y=0;y<rowNo;y++){
            for(int x=0;x<colNo;x++){
                int tileVal=map[(y*colNo)+x];
                float tileRot=tileVal-map[(y*colNo)+x];
                int tileMapX=(int)tileVal%tileMapNoX;
                int tileMapY=tileVal/tileMapNoX;
                if(tileVal!=0) {
                    //printf("Getting tile id:%i (located at %i,%i)\n", tileVal, tileMapX, tileMapY);
                }
                tile.setOrigin(0,0);
                tile.setRotation(0.0f);
                tile.setTextureRect(sf::IntRect(tileMapX*tileSize,tileMapY*tileSize,tileSize,tileSize));
                tile.setScale(scaleFactor.x,scaleFactor.y);
                tile.setPosition(x*tileSize*scaleFactor.x,y*tileSize*scaleFactor.y);
                tile.setOrigin(tileSize/2,tileSize/2);
                tile.setRotation(tileRot*360.0f);
                window.draw(tile);
                if(printTileIds){
                    timeText.setString(std::to_string(tileVal));
                    timeText.setFont(font);
                    timeText.setCharacterSize(10);
                    timeText.setFillColor(sf::Color::White);
                    timeText.setPosition(x*tileSize*scaleFactor.x,y*tileSize*scaleFactor.y);
                    window.draw(timeText);
                }


            }
        }


        shape.setRadius(10);
        shape.setOrigin(10,10);


        shape.setPosition(mouseX,mouseY);
        window.draw(shape);


        timeText.setString(std::to_string(clock.getElapsedTime().asSeconds()));
        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10,10);
        window.draw(timeText);




        window.display();

    }

    return 0;
}