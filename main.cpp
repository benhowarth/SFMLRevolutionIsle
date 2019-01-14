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


int const displayParamNo=3;
enum Tile{
    WATER=0,
    ISLAND=1,
    ROAD=2,
    WAREHOUSE=3,
    LIGHTHOUSE=4,
    DOCK=5,
    ROADWATER=6
};
Tile brushTile=ISLAND;
int const rowNo=30;
int const colNo=30;

int map[colNo*rowNo];

int displayMap[colNo*rowNo*displayParamNo];
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

int GetBitmask(int x, int y,bool justRoads){
    //get bitmask
    int bm=0;
    //printf("looking at %i,%i:\n",tx,ty);
    for(int m=0;m<4;m++){
        int nx=x+moves[m][0];
        int ny=y+moves[m][1];
        //printf("\tmove %i %i,%i:\n",m+1,nx,ny);
        if(nx>=0&&nx<colNo&&ny>=0&&ny<rowNo){
            int *nTile=&map[(ny*colNo)+nx];

            if((justRoads && (*nTile==ROAD||*nTile==DOCK||*nTile==ROADWATER))||(!justRoads && (*nTile==ISLAND||*nTile==ROAD||*nTile==WAREHOUSE||*nTile==LIGHTHOUSE))){
                bm+=pow(2,m);
            }
        }
    }
    return bm;
}

void CalculateDisplayMap(){

    for(int i=0;i<colNo*rowNo*displayParamNo;i+=displayParamNo){
        int ty=(i/displayParamNo)/colNo;
        int tx=(i/displayParamNo)%colNo;
        //printf("looking at %i,%i\n",tx,ty);
        int sx=1;
        int sy=1;
        int tileVal=map[i/displayParamNo];
        if(tileVal==WATER){
            int bm=GetBitmask(tx,ty,false);

            switch(bm){
                //surrounded
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
                    //all but west
                case 7:
                    tileVal=49;
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
                        if(map[(ny*colNo)+nx]==ISLAND||map[(ny*colNo)+nx]==ROAD||map[(ny*colNo)+nx]==WAREHOUSE||map[(ny*colNo)+nx]==LIGHTHOUSE){
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

        }else if(tileVal==ISLAND){
            tileVal=11;
        }else if(tileVal==ROAD){
            //tileVal=26;
            //do bitmask for road
            int bmr=GetBitmask(tx,ty,true);
            switch(bmr){
                //surrounded
                case 15:
                    tileVal=50;
                    break;
                    //all but north
                case 14:
                    tileVal=48;
                    break;
                    //all but east
                case 13:
                    tileVal=47;
                    break;
                    //west and south
                case 12:
                    tileVal=33;
                    break;
                    //all but south
                case 11:
                    tileVal=46;
                    break;
                    //west and east
                case 10:
                    tileVal=26;
                    break;
                    //north and west
                case 9:
                    tileVal=41;
                    break;
                    //west
                case 8:
                    tileVal=26;
                    break;
                    //all but west
                case 7:
                    tileVal=49;
                    break;
                    //south and east
                case 6:
                    tileVal=32;
                    break;
                    //north and south
                case 5:
                    tileVal=27;
                    break;
                    //south
                case 4:
                    tileVal=27;
                    break;
                    //north and east
                case 3:
                    tileVal=40;
                    break;
                    //east
                case 2:
                    tileVal=26;
                    break;
                    //north
                case 1:
                    tileVal=27;
                    break;
                default:
                    tileVal=26;
            }
        }
        else if(tileVal==WAREHOUSE){
            //tileVal=42;
            bool evenY=ty%2==0;
            bool evenX=tx%2==0;
            if(evenY){
                if(evenX){
                    //top left
                    tileVal=34;
                }else{
                    //top right
                    tileVal=35;
                }
            }else{
                if(evenX){
                    //bottom left
                    tileVal=42;
                }else{
                    //bottom right
                    tileVal=43;
                }
            }

        }else if(tileVal==LIGHTHOUSE){
            tileVal=39;
            if(ty-1>=0){
                displayMap[(((ty-1)*colNo)+tx)*displayParamNo]=31;
            }
            if(ty-2>=0){
                displayMap[(((ty-2)*colNo)+tx)*displayParamNo]=23;
            }
            if(ty-3>=0){
                displayMap[(((ty-3)*colNo)+tx)*displayParamNo]=15;
            }
        }else if(tileVal==DOCK){
            tileVal=24;

            int bmr=GetBitmask(tx,ty,true);
            if(ty-1>=0&&map[((ty-1)*colNo)+tx]==ISLAND){
                tileVal=51;
            }
            else if(ty<rowNo&&map[((ty+1)*colNo)+tx]==ISLAND){
                tileVal=51;
            }else{
                switch(bmr){
                    case 5:
                        tileVal=51;
                        break;
                    case 4:
                        tileVal=51;
                        break;
                    case 1:
                        tileVal=51;
                        break;
                    default:
                        tileVal=24;
                }
            }
        }else if(tileVal==ROADWATER){
            tileVal=25;

            int bmr=GetBitmask(tx,ty,true);
            if(ty-1>=0&&map[((ty-1)*colNo)+tx]==ISLAND){
                tileVal=52;
            }
            else if(ty<rowNo&&map[((ty+1)*colNo)+tx]==ISLAND){
                tileVal=52;
            }else{
                switch(bmr){
                    case 5:
                        tileVal=52;
                        break;
                    case 4:
                        tileVal=52;
                        break;
                    case 1:
                        tileVal=52;
                        break;
                    default:
                        tileVal=25;
                }
            }
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

int tileSize=8;
sf::Vector2f scaleFactor=sf::Vector2f(2.0f,2.0f);
int brushSize=2;

sf::Vector2f GetTileFromPos(sf::Vector2f posIn){
    sf::Vector2f posOut;
    posOut.x=((colNo/scaleFactor.x)*posIn.x)/(colNo*tileSize);
    posOut.y=((rowNo/scaleFactor.y)*posIn.y)/(rowNo*tileSize);
    posOut.x=floor(posOut.x/brushSize)*brushSize;
    posOut.y=floor(posOut.y/brushSize)*brushSize;

    return posOut;
}

sf::Vector2f oldTileClick;
void Click(int x,int y){
    printf("Click at %i,%i\n",x,y);
    sf::Vector2f tileClick=GetTileFromPos(sf::Vector2f(x,y));
    printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    if(tileClick!=oldTileClick) {
        oldTileClick=tileClick;
        printf("clicked tile %f,%f\n", tileClick.x, tileClick.y);
        //displayMap[3*((tileClickY*colNo)+tileClickX)+1]*=-1;
        for (int y = 0; y < brushSize; y++) {
            for (int x = 0; x < brushSize; x++) {
                int *tile = &map[(int) (((tileClick.y + y) * colNo) + tileClick.x + x)];
                if (brushTile == WATER) {
                    *tile = WATER;
                } else if (brushTile == ISLAND) {
                    *tile = ISLAND;
                } else if (brushTile == ROAD) {
                    if (*tile == ISLAND) {
                        *tile = ROAD;
                    }
                }else if(brushTile==WAREHOUSE){
                    if (*tile == ISLAND||*tile == ROAD) {
                        *tile = WAREHOUSE;
                    } else if (*tile == WAREHOUSE) {
                        *tile = ISLAND;
                    }
                }else if(brushTile==LIGHTHOUSE){
                    if (*tile == ISLAND||*tile == ROAD) {
                        *tile=LIGHTHOUSE;
                    }else if(*tile==LIGHTHOUSE){
                        *tile=ISLAND;
                    }
                }else if(brushTile==DOCK){
                    if(*tile==WATER){
                        *tile=DOCK;
                    }else if(*tile==DOCK){
                        *tile=WATER;
                    }

                }else if(brushTile==ROADWATER){
                    if(*tile==WATER){
                        *tile=ROADWATER;
                    }else if(*tile==ROADWATER){
                        *tile=WATER;
                    }
                }
            }
        }
        CalculateDisplayMap();
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

    bool printTileIds=true;
    sf::Text tileIdText;


    int tileMapNoX=tileTexture.getSize().x/tileSize;
    int tileMapNoY=tileTexture.getSize().y/tileSize;
    //printf("%i\n",tileMapNoX*tileMapNoY);


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
    for(int i=0;i<colNo*rowNo;i++){
        map[i]=0;
    }

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
                if(event.key.code==sf::Keyboard::Num1){
                    brushTile=WATER;
                    brushSize=2;
                }
                if(event.key.code==sf::Keyboard::Num2){
                    brushTile=ISLAND;
                    brushSize=2;
                }
                if(event.key.code==sf::Keyboard::Num3){
                    brushTile=ROAD;
                    brushSize=1;
                }
                if(event.key.code==sf::Keyboard::Num4){
                    brushTile=WAREHOUSE;
                    brushSize=2;
                }
                if(event.key.code==sf::Keyboard::Num5){
                    brushTile=LIGHTHOUSE;
                    brushSize=1;
                }
                if(event.key.code==sf::Keyboard::Num6){
                    brushTile=DOCK;
                    brushSize=1;
                }
                if(event.key.code==sf::Keyboard::Num7){
                    brushTile=ROADWATER;
                    brushSize=1;
                }

            }

            if(event.type==sf::Event::MouseMoved){
                mouseX=event.mouseMove.x;
                mouseY=event.mouseMove.y;
            }
            if(event.type==sf::Event::MouseWheelScrolled){
                if(event.mouseWheelScroll.wheel==sf::Mouse::VerticalWheel){
                    scaleFactor.x+=event.mouseWheelScroll.delta*0.1;
                    scaleFactor.y+=event.mouseWheelScroll.delta*0.1;
                }
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            Click(mouseX,mouseY);
        }else{
            oldTileClick=sf::Vector2f(-100,-100);
        }

        window.clear(sf::Color::Black);


        for(int y=0;y<rowNo;y++){
            for(int x=0;x<colNo;x++){
                int mapVal=map[((y*colNo)+x)];
                int tileVal=displayMap[displayParamNo*((y*colNo)+x)];
                int secs=clock.getElapsedTime().asSeconds();
                if(secs%2==0){
                    switch(tileVal){
                        case 4:
                            tileVal=3;
                            break;
                    }
                }
                int tileScaleX=displayMap[displayParamNo*((y*colNo)+x)+1];
                int tileScaleY=displayMap[displayParamNo*((y*colNo)+x)+2];
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
                if(tileVal==24||tileVal==25||tileVal==51||tileVal==52){
                    int newTileVal=17;
                    int newX=x;
                    int newY=y-1;
                    if(tileVal==51||tileVal==52){
                        newTileVal=16;
                        newX=x-1;
                        newY=y;
                    }
                    tileMapX=(int)newTileVal%tileMapNoX;
                    tileMapY=newTileVal/tileMapNoX;
                    tile.setTextureRect(sf::IntRect(tileMapX*tileSize,tileMapY*tileSize,tileSize,tileSize));
                    tile.setPosition((newX-(tileScaleX-1)/2)*tileSize*scaleFactor.x,(newY-(tileScaleY-1)/2)*tileSize*scaleFactor.y);
                    window.draw(tile);
                }
                if(printTileIds){
                    tileIdText.setString(std::to_string(mapVal));
                    //tileIdText.setString("("+std::to_string(x)+","+std::to_string(y)+")");
                    tileIdText.setFont(font);
                    tileIdText.setCharacterSize(10);
                    tileIdText.setFillColor(sf::Color::Red);
                    tileIdText.setPosition(x*tileSize*scaleFactor.x,y*tileSize*scaleFactor.y);
                    window.draw(tileIdText);
                }


            }
        }


        //shape.setRadius(10);
        //shape.setOrigin(10,10);
        //shape.setPosition(mouseX,mouseY);
        //window.drawShape




        //draw brush

        sf::RectangleShape brush(sf::Vector2f(tileSize*brushSize*scaleFactor.x,tileSize*brushSize*scaleFactor.y));
        sf::Vector2f brushPos=GetTileFromPos(sf::Vector2f(mouseX,mouseY));
        brushPos.x*=tileSize*scaleFactor.x;
        brushPos.y*=tileSize*scaleFactor.y;
        brush.setPosition(brushPos);
        brush.setFillColor(sf::Color::Transparent);
        brush.setOutlineThickness(2.0);
        brush.setOutlineColor(sf::Color::White);
        window.draw(brush);

        std::string brushText;
        switch(brushTile){
            case WATER:
                brushText="WATER";
                break;
            case ISLAND:
                brushText="ISLAND";
                break;
            case ROAD:
                brushText="ROAD";
                break;
            case WAREHOUSE:
                brushText="WAREHOUSE";
                break;
            case LIGHTHOUSE:
                brushText="LIGHTHOUSE";
                break;
            case DOCK:
                brushText="DOCK";
                break;
            case ROADWATER:
                brushText="ROADWATER";
                break;
            default:
                brushText="UNKNOWN";
        }
        timeText.setString(brushText+"\n"+std::to_string(clock.getElapsedTime().asSeconds()));
        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10,10);
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