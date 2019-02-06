//
// Created by Ben on 02/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_TILES_H
#define SFMLREVOLUTIONISLE_TILES_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#define NODE_MAX_COST 500
enum Tile{
    NILTILE=-1,
    WATER=0,
    ISLAND=1,
    ROAD=2,
    WAREHOUSE=3,
    LIGHTHOUSE=4,
    DOCK=5,
    ROADWATER=6
};

Tile brushTile=ISLAND;

struct TileStruct{
    Tile basicTile;
    int displayTile,sx,sy;
};

//TileStruct t=TileStruct{WATER,0,1,1};

int const rowNo=30;
int const colNo=30;

int map[colNo*rowNo];

TileStruct tMap[colNo*rowNo];


struct Node{
public:
    Node* parent=nullptr;
    int parentId=-1;
    int cost,costSoFar,id;
    bool visited=false;
    Node(int _id,int _cost){
        id=_id;
        costSoFar=NODE_MAX_COST*NODE_MAX_COST*rowNo*colNo;
        cost=_cost;
    }
    void Print(){
        int pId;
        /*if(parent!=nullptr){
            pId=parent->id;
        }else{
            pId=-1;
        }*/
        pId=parentId;
        printf("Node %i cost:%i, parentid:%i\n",id,cost,pId);
    }
};
static int moves8[8][2]={
        {0,-1},
        {1,-1},
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
        {-1,0},
        {-1,-1}
};
class Graph{
private:
    int rowNo,colNo;
    std::vector<Node> nodes;

public:
    Graph(int col,int row, TileStruct tiles[]){
        rowNo=row;
        colNo=col;
        //nodes[rowNo*colNo];
        UpdateGraph(tiles);
    }
    void UpdateGraph(TileStruct tiles[]){
        for(int i=0;i<rowNo*colNo;i++){
            int costTemp;
            //printf("%i,",tiles[i].basicTile);
            //if((i+1)%colNo==0){printf("\n");}
            if(tiles[i].basicTile==ISLAND) {
                costTemp = 70;
                //printf("ISLAND\n");
            }else if(tiles[i].basicTile==ROAD||tiles[i].basicTile==ROADWATER){
                costTemp=5;
            }else{
                costTemp=NODE_MAX_COST;
            }
            //printf("NODE %i COST %i\n",i,costTemp);
            nodes.push_back(Node(i,costTemp));
            //printf("%i\n",i);
        }
    }
    void Print(){
        for(int i=0;i<rowNo*colNo;i++){
            if(i%colNo==0){
                printf("\n");
            }
            printf("|%3d|",i);
        }
        printf("\n");
    }
    void PrintParent(){
        for(int i=0;i<rowNo*colNo;i++){
            if(i%colNo==0){
                printf("\n");
            }
            printf("|%3d (%3d)|",i,nodes[i].parentId);
        }
        printf("\n");
    }

    std::vector<int> getN(int id){
        std::vector<int> ns;
        int idX=id%colNo;
        int idY=id/colNo;
        for(int i=0;i<8;i++){
            int mX=moves8[i][0];
            int mY=moves8[i][1];
            if(idX+mX>-1 &&idX+mX<colNo && idY+mY>-1 && idY+mY<rowNo){
                ns.push_back(id+(mY*colNo)+mX);
            }
        }
        //printf("%i neighbours found\n",ns.size());
        return ns;
    }
    std::vector<int> GetPath(int idStart,int idEnd,TileStruct tiles[]){
        UpdateGraph(tiles);
        float startX=idStart%colNo;
        float startY=idStart/colNo;
        float endX=idEnd%colNo;
        float endY=idEnd/colNo;
        std::vector<int> toVisit;
        toVisit.push_back(idStart);
        nodes[idStart].visited=true;
        nodes[idStart].costSoFar=0;
        while(!toVisit.empty()){

            int id=toVisit.back();
            int idX=id%colNo;
            int idY=id/colNo;
            nodes[id].visited=true;
            //toVisit.erase(toVisit.begin());
            toVisit.pop_back();
            //printf("Checking id: %i (%i more nodes in queue)\n",id,toVisit.size());
            std::vector<int> ns=getN(id);
            for(int i=0;i<ns.size();i++){
                if(!nodes[ns[i]].visited /*&& nodes[ns[i]].cost<NODE_MAX_COST */&& tiles[ns[i]].basicTile!=WATER){
                    toVisit.push_back(ns[i]);

                    int nodeX=ns[i]%colNo;
                    int nodeY=ns[i]/colNo;
                    int nodeXDifEnd=endX-nodeX;
                    int nodeYDifEnd=endY-nodeY;
                    int nodeXDifStart=startX-nodeX;
                    int nodeYDifStart=startY-nodeY;
                    float h=std::sqrt((nodeXDifEnd*nodeXDifEnd)+(nodeYDifEnd*nodeYDifEnd));
                    h+=(std::abs(nodeXDifEnd*nodeYDifEnd - nodeXDifStart*nodeYDifEnd))*0.5;
                    if((nodeX-idX)!=0 && (nodeY-idY)!=0){h*=0.5;}

                    int costTemp=nodes[id].costSoFar+nodes[id].cost*1/(h/(rowNo*colNo));

                    if(costTemp<nodes[ns[i]].costSoFar){
                        nodes[ns[i]].costSoFar=costTemp;
                        //nodes[ns[i]].parent=&nodes[id];
                        nodes[ns[i]].parentId=id;
                    }
                    //nodes[ns[i]].visited=true;
                }
            }

        }
        //nodes[0].Print();
        //nodes[1].Print();
        /*Node* nodePar=nodes[idEnd].parent;
        while(nodePar!=nullptr){
            printf("%i, ",nodePar->id);
            nodePar=nodePar->parent;
        }*/
        std::vector<int> pathRes;
        int nodePar=nodes[idEnd].parentId;
        pathRes.push_back(idEnd);
        std::cout<<"desitnation tile: "<<tMap[idEnd].basicTile<<" node par: "<<nodes[idEnd].parentId<<std::endl;
        while(nodePar!=idStart){
            if(nodePar==-1){
                printf("No path found!\n");
                pathRes.clear();
                break;
            }
            //nodes[nodePar].Print();
            //printf("%i, ",nodes[nodePar].id);
            pathRes.push_back(nodes[nodePar].id);
            nodePar=nodes[nodePar].parentId;
        }
        pathRes.push_back(idStart);
        //printf(" END\n");
        std::reverse(pathRes.begin(),pathRes.end());
        //PrintParent();
        //printf("%i\n",nodes[405].costSoFar);

        //set to unvisited, no parent etc
        for(int i=0;i<nodes.size();i++){
            nodes[i].visited=false;
            //nodes[i].parent=-1;
            nodes[i].parentId=-1;
            nodes[i].costSoFar=NODE_MAX_COST*NODE_MAX_COST*rowNo*colNo;;
        }


        //pathRes.clear();
        //pathRes[0]=idStart;

        return pathRes;
    }



};

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
            Tile *nTile=&tMap[(ny*colNo)+nx].basicTile;

            if((justRoads && (*nTile==ROAD||*nTile==DOCK||*nTile==ROADWATER))||(!justRoads && (*nTile==ISLAND||*nTile==ROAD||*nTile==WAREHOUSE||*nTile==LIGHTHOUSE))){
                bm+=pow(2,m);
            }
        }
    }
    return bm;
}

void CalculateDisplayMap(){

    for(int i=0;i<colNo*rowNo;i++){
        int ty=i/colNo;
        int tx=i%colNo;
        //printf("looking at %i,%i\n",tx,ty);
        int sx=1;
        int sy=1;
        int tileVal=tMap[i].basicTile;
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
                        if(tMap[(ny*colNo)+nx].basicTile==ISLAND||tMap[(ny*colNo)+nx].basicTile==ROAD||tMap[(ny*colNo)+nx].basicTile==WAREHOUSE||tMap[(ny*colNo)+nx].basicTile==LIGHTHOUSE){
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
                if(tileVal==0 && ty-1>=0 && tMap[((ty-1)*colNo)+tx].basicTile==ROADWATER){
                    bool evenX=tx%2==0;
                    if(evenX){
                        //be a bridge support
                        tileVal=1;
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
                tMap[((ty-1)*colNo)+tx].displayTile=31;
                //displayMap[(((ty-1)*colNo)+tx)*displayParamNo]=31;
            }
            if(ty-2>=0){
                tMap[((ty-2)*colNo)+tx].displayTile=23;
                //displayMap[(((ty-2)*colNo)+tx)*displayParamNo]=23;
            }
            if(ty-3>=0){
                tMap[((ty-3)*colNo)+tx].displayTile=15;
                //displayMap[(((ty-3)*colNo)+tx)*displayParamNo]=15;
            }
        }else if(tileVal==DOCK){

            int bmr=GetBitmask(tx,ty,true);
            if(ty-1>=0&&tMap[((ty-1)*colNo)+tx].basicTile==ISLAND){
                tileVal=51;
            }
            else if(ty<rowNo&&tMap[((ty+1)*colNo)+tx].basicTile==ISLAND){
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
            if(ty-1>=0&&tMap[((ty-1)*colNo)+tx].basicTile==ISLAND){
                tileVal=52;
            }
            else if(ty<rowNo&&tMap[((ty+1)*colNo)+tx].basicTile==ISLAND){
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
            float noise=(sin(ty*tx+cos(tx))+1.0)/2.0;
            if(noise<0.2){tileVal=12;}
            else if(noise<0.3){tileVal=6;}
            else if(noise<0.4){tileVal=13;}
            else if(noise<0.5){tileVal=14;}
        }
        else if(tileVal==20){
            if(sin(ty*tx+cos(tx))>0.5){tileVal=21;}
        }

        tMap[i].displayTile=tileVal;
        //flipx?
        tMap[i].sx=sx;
        //flipy?
        tMap[i].sy=sy;
    }

}

int tileSize=8;
sf::Vector2f scaleFactor=sf::Vector2f(2.0f,2.0f);
int brushSize=2;

sf::Vector2f GetTileFromPos(sf::Vector2f posIn){
    sf::Vector2f posOut;
    //posOut.x=((colNo/scaleFactor.x)*posIn.x)/(colNo*tileSize);
    //posOut.y=((rowNo/scaleFactor.y)*posIn.y)/(rowNo*tileSize);
    posOut.x=((posIn.x/tileSize)/scaleFactor.x);
    posOut.y=((posIn.y/tileSize)/scaleFactor.y);

    posOut.x=floor(posOut.x/brushSize)*brushSize;
    posOut.y=floor(posOut.y/brushSize)*brushSize;

    return posOut;
}
int GetTileIdFromPos(sf::Vector2f posIn){
    int idOut;
    sf::Vector2f posOut=GetTileFromPos(posIn);
    /*posOut.x=((colNo/scaleFactor.x)*posIn.x)/(colNo*tileSize);
    posOut.y=((rowNo/scaleFactor.y)*posIn.y)/(rowNo*tileSize);
    posOut.x=floor(posOut.x/brushSize)*brushSize;
    posOut.y=floor(posOut.y/brushSize)*brushSize;*/

    idOut=posOut.x+(colNo*posOut.y);

    return idOut;
}

int tileMapNoX,tileMapNoY;
sf::Sprite tile;

void DrawTileAt(sf::RenderWindow &win,int tileId,int x,int y,float scaleX,float scaleY){
    int tileMapX=tileId%tileMapNoX;
    int tileMapY=tileId/tileMapNoX;
    tile.setOrigin(0,0);
    tile.setTextureRect(sf::IntRect(tileMapX*tileSize,tileMapY*tileSize,tileSize,tileSize));
    tile.setOrigin(tileSize/2,tileSize/2);
    tile.setScale(scaleFactor.x*scaleX,scaleFactor.y*scaleY);
    tile.setOrigin(0,0);
    tile.setPosition((x-(scaleX-1)/2)*tileSize*scaleFactor.x,(y-(scaleY-1)/2)*tileSize*scaleFactor.y);
    win.draw(tile);
}



#endif //SFMLREVOLUTIONISLE_TILES_H
