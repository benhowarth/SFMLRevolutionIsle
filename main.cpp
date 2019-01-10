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
int const rowNo=10;
int const colNo=10;

int map[colNo*rowNo]= {
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,1,1,1,1,0,0,0,
        0,0,0,1,1,1,1,0,0,0,
        0,0,0,1,1,1,1,0,0,0,
        0,0,0,1,1,1,1,0,0,0,
        0,0,0,1,1,1,1,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0
};

int displayMap[colNo*rowNo*3];
static int moves[4][2]={
        {0,-1},
        {1,0},
        {0,1},
        {-1,0}
};
static int cornerMoves[4][2]={
        {-1,-1},
        {1,-1},
        {1,1},
        {-1,1}
};

void CalculateDisplayMap(){

    for(int i=0;i<colNo*rowNo*3;i+=3){
        int ty=(i/3)/colNo;
        int tx=(i/3)%colNo;
        //printf("looking at %i,%i\n",tx,ty);
        int sx=1;
        int sy=1;
        int tileVal=map[i/3];
        if(tileVal==0){
            //get bitmask
            int bm=0;
            //printf("looking at %i,%i:\n",tx,ty);
            for(int m=0;m<4;m++){
                int nx=tx+moves[m][0];
                int ny=ty+moves[m][1];
                //printf("\tmove %i %i,%i:\n",m+1,nx,ny);
                if(nx>=0&&nx<colNo&&ny>=0&&ny<rowNo){
                    if(map[(ny*colNo)+nx]==1){
                        bm+=pow(2,m);
                    }
                }
            }
            switch(bm){
                //shouldn't happen
                case 15:
                    tileVal=0;
                    break;
                    //all but north
                case 14:
                    tileVal=0;
                    break;
                    //all but east
                case 13:
                    tileVal=0;
                    break;
                    //west and south
                case 12:
                    tileVal=5;
                    sx=-1;
                    break;
                    //all but south
                case 11:
                    tileVal=0;
                    break;
                    //west and east
                case 10:
                    tileVal=0;
                    break;
                    //north and west
                case 9:
                    tileVal=22;
                    sx=-1;
                    break;
                    //west
                case 8:
                    tileVal=10;
                    sx=-1;
                    break;
                    //all but east
                case 7:
                    tileVal=0;
                    break;
                    //south and east
                case 6:
                    tileVal=5;
                    break;
                    //north and south
                case 5:
                    tileVal=0;
                    break;
                    //south
                case 4:
                    tileVal=4;
                    break;
                    //north and east
                case 3:
                    tileVal=22;
                    break;
                    //east
                case 2:
                    tileVal=10;
                    break;
                    //north
                case 1:
                    tileVal=20;
                    break;
            }
            //tileVal=bm;
            if(bm>0) {
                //printf("id%i (%i,%i) bitmask : %i  finaltileval:%i\n", i / 3, tx, ty, bm,tileVal);
            }
            if(bm==0){
                for(int m=0;m<4;m++){
                    int nx=tx+cornerMoves[m][0];
                    int ny=ty+cornerMoves[m][1];
                    int angle=0;
                    //printf("\tmove %i %i,%i:\n",m+1,nx,ny);
                    if(nx>=0&&nx<colNo&&ny>=0&&ny<rowNo){
                        if(map[(ny*colNo)+nx]==1){
                            if(cornerMoves[m][1]<0){
                                tileVal=18;
                            }else{
                                tileVal=2;
                            }
                            if(cornerMoves[m][0]<0){
                                sx=-1;
                            }
                        }
                    }
                }
            }

        }else{
            tileVal=11;
        }

        //randomise
        if(tileVal==11){
            if(sin(ty*tx+cos(tx))>0.2){tileVal=12;}
            else if(sin(ty*tx+cos(tx))>0.6){tileVal=13;}
        }
        else if(tileVal==20){
            if(sin(ty*tx+cos(tx))>0.5){tileVal=21;}
        }

        displayMap[i]=tileVal;
        //flipx?
        displayMap[i+1]=sx;
        //flipy?
        displayMap[i+2]=sy;
    }
}


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

    bool printTileIds=false;
    sf::Text tileIdText;

    int tileSize=8;
    int tileMapNoX=tileTexture.getSize().x/tileSize;
    int tileMapNoY=tileTexture.getSize().y/tileSize;
    printf("%i\n",tileMapNoX*tileMapNoY);
    sf::Vector2f scaleFactor=sf::Vector2f(6.5f,6.5f);
    int brushSize=2;


    sf::Sprite tile;
    tile.setTexture(tileTexture);



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

    CalculateDisplayMap();



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
                    //fix 2* thing (scalefactor and gridsize ratio?)
                    int tileClickX=((colNo/scaleFactor.x)*mouseX)/(colNo*tileSize);
                    int tileClickY=((rowNo/scaleFactor.y)*mouseY)/(rowNo*tileSize);
                    tileClickX=round(tileClickX/brushSize)*brushSize;
                    tileClickY=round(tileClickY/brushSize)*brushSize;
                    printf("clicked tile %i,%i\n",tileClickX,tileClickY);
                    //displayMap[3*((tileClickY*colNo)+tileClickX)+1]*=-1;
                    for(int y=0;y<brushSize;y++) {
                        for(int x=0;x<brushSize;x++) {
                            if (map[((tileClickY+y) * colNo) + tileClickX+x] == 0) {
                                map[((tileClickY+y) * colNo) + tileClickX+x] = 1;
                            } else {
                                map[((tileClickY+y) * colNo) + tileClickX+x] = 0;
                            }
                        }
                    }
                    CalculateDisplayMap();
                }
                if(event.mouseButton.button==sf::Mouse::Right) {
                }
            }
        }

        window.clear(sf::Color::Black);

        for(int y=0;y<rowNo;y++){
            for(int x=0;x<colNo;x++){
                int tileVal=displayMap[3*((y*colNo)+x)];
                int secs=clock.getElapsedTime().asSeconds();
                if(secs%2==0){
                    switch(tileVal){
                        case 4:
                            tileVal=3;
                            break;
                    }
                }
                int tileScaleX=displayMap[3*((y*colNo)+x)+1];
                int tileScaleY=displayMap[3*((y*colNo)+x)+2];
                int tileMapX=(int)tileVal%tileMapNoX;
                int tileMapY=tileVal/tileMapNoX;
                if(tileVal!=0) {
                    //printf("Getting tile id:%i (located at %i,%i)\n", tileVal, tileMapX, tileMapY);
                }
                tile.setOrigin(0,0);
                tile.setTextureRect(sf::IntRect(tileMapX*tileSize,tileMapY*tileSize,tileSize,tileSize));
                tile.setOrigin(tileSize/2,tileSize/2);
                tile.setScale(scaleFactor.x*tileScaleX,scaleFactor.y*tileScaleY);
                tile.setOrigin(0,0);
                tile.setPosition((x-(tileScaleX-1)/2)*tileSize*scaleFactor.x,(y-(tileScaleY-1)/2)*tileSize*scaleFactor.y);
                window.draw(tile);
                if(printTileIds){
                    timeText.setString(std::to_string(tileVal));
                    timeText.setString("("+std::to_string(x)+","+std::to_string(y)+")");
                    timeText.setFont(font);
                    timeText.setCharacterSize(10);
                    timeText.setFillColor(sf::Color::Red);
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