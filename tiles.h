//
// Created by Ben on 02/02/2019.
//

#ifndef SFMLREVOLUTIONISLE_TILES_H
#define SFMLREVOLUTIONISLE_TILES_H



#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "noise.h"

#include<vector>
#include<algorithm>


#define NODE_MAX_COST 500



int tileMapNoX,tileMapNoY;
sf::Sprite tile;

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

int tileWeights[]={1000,500,5,100,100,5,5};
const std::vector<Tile> tMask_island_general={ISLAND,ROAD,WAREHOUSE,LIGHTHOUSE};
const std::vector<Tile> tMask_island_roads={ISLAND,ROAD,ROADWATER};
const std::vector<Tile> tMask_roads={ROAD,ROADWATER};
const std::vector<Tile> tMask_water={WATER};
const std::vector<Tile> tMask_land={ISLAND,ROAD,WAREHOUSE,LIGHTHOUSE,DOCK,ROADWATER};



int tileSize=8;
sf::Vector2f scaleFactor=sf::Vector2f(1.0f,1.0f);
int brushSize=2;
Tile brushTile=ISLAND;

enum IslandType{SQUARE_ISLAND,CIRCLE_ISLAND,NOISE_ISLAND};

struct TileStruct{
    Tile basicTile;
    int displayTile;
    int sx,sy;
    int cost;
    //TileStruct* parent=nullptr;
    int parent=-1;
    int costSoFar=NODE_MAX_COST*NODE_MAX_COST*1000;
    //std::vector<TileStruct*> ns;
    std::vector<int> ns;
    bool visited=false;
    TileStruct(Tile t_basicTile){
        basicTile=t_basicTile;
    }

};

class TileMap {
private:
    int m_colNo;
    int m_rowNo;
    sf::RenderWindow *m_win;
    std::vector<TileStruct> m_tiles;

    const int movesManhattan[4][2] = {
            {0,  -1},
            {1,  0},
            {0,  1},
            {-1, 0}
    };
    const int movesDiagonal[4][2] = {
            {-1, -1},
            {1,  -1},
            {1,  1},
            {-1, 1}
    };
    const int moves[8][2] = {
            {0,  -1},
            {1,  -1},
            {1,  0},
            {1,  1},
            {0,  1},
            {-1, 1},
            {-1, 0},
            {-1, -1}
    };

public:
    TileMap(int t_colNo, int t_rowNo, sf::RenderWindow *t_win) {
        TileMap(SQUARE_ISLAND, t_colNo, t_rowNo, t_win);
    }

    TileMap(IslandType t_islandToMake, int t_colNo, int t_rowNo, sf::RenderWindow *t_win) {
        m_colNo = t_colNo;
        m_rowNo = t_rowNo;
        m_win = t_win;
        m_tiles.reserve(m_colNo * m_rowNo);
        generateIsland(t_islandToMake);
    }


    void generateIsland(IslandType t_islandToMake) {
        if (t_islandToMake == SQUARE_ISLAND) {
            int islandBuffer = 10;
            for (int i = 0; i < m_colNo * m_rowNo; i++) {
                sf::Vector2i coords = sf::Vector2i(i % m_colNo, i / m_colNo);
                if ((coords.x < islandBuffer) || (coords.x > (m_colNo - 1) - islandBuffer) ||
                    (coords.y < islandBuffer) || (coords.y > (m_rowNo - 1) - islandBuffer)) {
                    m_tiles.emplace_back(WATER);
                } else {
                    m_tiles.emplace_back(ISLAND);
                }
            }
        } else if (t_islandToMake == CIRCLE_ISLAND) {
            int islandRad = 10;
            for (int i = 0; i < m_colNo * m_rowNo; i++) {
                sf::Vector2i coords = sf::Vector2i(i % m_colNo, i / m_colNo);
                coords.x -= (m_colNo / 2);
                coords.y -= (m_rowNo / 2);
                if ((coords.x * coords.x + coords.y * coords.y) > islandRad * islandRad) {
                    m_tiles.emplace_back(WATER);
                } else {
                    m_tiles.emplace_back(ISLAND);
                }
            }
        } else if (t_islandToMake == NOISE_ISLAND) {
            int islandRad = 15;
            for (int i = 0; i < m_colNo * m_rowNo; i++) {
                sf::Vector2i coords = sf::Vector2i(i % m_colNo, i / m_colNo);
                coords.x -= (m_colNo / 2);
                coords.y -= (m_rowNo / 2);
                if ((coords.x * coords.x + coords.y * coords.y) > islandRad * islandRad &&
                    getNoise(coords.x, coords.y) > 0.6) {
                    m_tiles.emplace_back(WATER);
                } else {
                    m_tiles.emplace_back(ISLAND);
                }
            }
        }


        for (int i = 0; i < m_colNo * m_rowNo; i++) {
            updateTile(i);
        }
    }


    sf::Vector2i getTileCoordsFromId(int t_id) {
        return sf::Vector2i(t_id % m_colNo, t_id / m_colNo);
    }

    int getTileIdFromCoords(sf::Vector2i t_coords) {
        return (t_coords.y * m_colNo) + t_coords.x;
    }

    sf::Vector2i getTileCoordsFromPos(sf::Vector2f t_pos, int t_brushSize) {

        sf::Vector2i coordsOut;
        coordsOut.x = ((t_pos.x / tileSize) / scaleFactor.x);
        coordsOut.y = ((t_pos.y / tileSize) / scaleFactor.y);

        coordsOut.x = floor(coordsOut.x / t_brushSize) * t_brushSize;
        coordsOut.y = floor(coordsOut.y / t_brushSize) * t_brushSize;

        return coordsOut;

    }

    TileStruct *getTileFromId(int t_id) {
        return &m_tiles[t_id];
    }

    int getTileIdFromPos(sf::Vector2f t_pos, int t_brushSize) {
        return getTileIdFromCoords(getTileCoordsFromPos(t_pos, t_brushSize));
    }

    int getBitmaskFromTileId(int t_id, std::vector<Tile> t_mask) {
        //get bitmask
        int bm = 0;
        sf::Vector2i coords = getTileCoordsFromId(t_id);
        int x = coords.x;
        int y = coords.y;
        for (int m = 0; m < 4; m++) {
            int nx = x + movesManhattan[m][0];
            int ny = y + movesManhattan[m][1];
            //printf("\tmove %i %i,%i:\n",m+1,nx,ny);
            if (nx >= 0 && nx < m_colNo && ny >= 0 && ny < m_rowNo) {
                Tile neighbourTileVal = m_tiles[getTileIdFromCoords(sf::Vector2i(nx, ny))].basicTile;
                for (int k = 0; k < t_mask.size(); k++) {
                    if (neighbourTileVal == t_mask[k]) {
                        bm += pow(2, m);
                        break;
                    }
                }
            }
        }
        return bm;

    }

    void resetTile(int t_id) {
        //m_tiles[t_id].parent= nullptr;
        m_tiles[t_id].parent = -1;
        m_tiles[t_id].costSoFar = NODE_MAX_COST * NODE_MAX_COST * m_rowNo * m_colNo;
        m_tiles[t_id].visited = false;
    }

    void resetTiles() {
        for (int i = 0; i < m_tiles.size(); i++) {
            resetTile(i);
        }
    }


    void updateTile(int t_id) {
        TileStruct *t = &m_tiles[t_id];
        //update ns
        (*t).ns.clear();
        std::vector<int> ns;
        sf::Vector2i coords = getTileCoordsFromId(t_id);
        for (int i = 0; i < 8; i++) {
            sf::Vector2i nCoords = coords;
            nCoords.x += moves[i][0];
            nCoords.y += moves[i][1];

            if (nCoords.x > -1 && nCoords.x < m_colNo && nCoords.y > -1 && nCoords.y < m_rowNo) {
                (*t).ns.push_back(getTileIdFromCoords(nCoords));
            }
        }

        //update cost
        (*t).cost = tileWeights[t->basicTile];
        //update display map
        sf::Vector2i tCoords = getTileCoordsFromId(t_id);
        //printf("looking at %i,%i\n",tx,ty);
        int sx = 1;
        int sy = 1;
        int tileVal = t->basicTile;
        if (tileVal == WATER) {


            int bm = getBitmaskFromTileId(t_id, tMask_island_general);

            switch (bm) {
                //surrounded
                case 15:
                    tileVal = 0;
                    break;
                    //all but north
                case 14:
                    tileVal = 0;
                    break;
                    //all but east
                case 13:
                    tileVal = 0;
                    break;
                    //west and south
                case 12:
                    tileVal = 5;
                    sx = -1;
                    break;
                    //all but south
                case 11:
                    tileVal = 0;
                    break;
                    //west and east
                case 10:
                    tileVal = 0;
                    break;
                    //north and west
                case 9:
                    tileVal = 22;
                    sx = -1;
                    break;
                    //west
                case 8:
                    tileVal = 10;
                    sx = -1;
                    break;
                    //all but west
                case 7:
                    tileVal = 49;
                    break;
                    //south and east
                case 6:
                    tileVal = 5;
                    break;
                    //north and south
                case 5:
                    tileVal = 0;
                    break;
                    //south
                case 4:
                    tileVal = 4;
                    break;
                    //north and east
                case 3:
                    tileVal = 22;
                    break;
                    //east
                case 2:
                    tileVal = 10;
                    break;
                    //north
                case 1:
                    tileVal = 20;
                    break;
            }
            if (bm == 0) {
                for (int m = 0; m < 4; m++) {
                    sf::Vector2i nCoords = tCoords;
                    nCoords.x += movesDiagonal[m][0];
                    nCoords.y += movesDiagonal[m][1];
                    int angle = 0;
                    Tile nT = m_tiles[getTileIdFromCoords(nCoords)].basicTile;
                    //printf("\tmove %i %i,%i:\n",m+1,nx,ny);
                    if (nCoords.x >= 0 && nCoords.x < m_colNo && nCoords.y >= 0 && nCoords.y < m_rowNo) {
                        if (nT == ISLAND || nT == ROAD || nT == WAREHOUSE || nT == LIGHTHOUSE) {
                            if (movesDiagonal[m][1] < 0) {
                                tileVal = 18;
                            } else {
                                tileVal = 2;
                            }
                            if (movesDiagonal[m][0] < 0) {
                                sx = -1;
                            }
                        }
                    }
                }
                //place structures
                sf::Vector2i aboveTCoords = tCoords;
                aboveTCoords.y--;
                Tile aboveTBasicTile = m_tiles[getTileIdFromCoords(aboveTCoords)].basicTile;
                if (tileVal == 0 && tCoords.y - 1 >= 0 && aboveTBasicTile == ROADWATER) {
                    bool evenX = tCoords.x % 2 == 0;
                    if (evenX) {
                        //be a bridge support
                        tileVal = 1;
                    }
                }
            }

        } else if (tileVal == ISLAND) {
            tileVal = 11;
        } else if (tileVal == ROAD) {
            //tileVal=26;
            //do bitmask for road
            int bmr = getBitmaskFromTileId(t_id, tMask_roads);
            switch (bmr) {
                //surrounded
                case 15:
                    tileVal = 50;
                    break;
                    //all but north
                case 14:
                    tileVal = 48;
                    break;
                    //all but east
                case 13:
                    tileVal = 47;
                    break;
                    //west and south
                case 12:
                    tileVal = 33;
                    break;
                    //all but south
                case 11:
                    tileVal = 46;
                    break;
                    //west and east
                case 10:
                    tileVal = 26;
                    break;
                    //north and west
                case 9:
                    tileVal = 41;
                    break;
                    //west
                case 8:
                    tileVal = 26;
                    break;
                    //all but west
                case 7:
                    tileVal = 49;
                    break;
                    //south and east
                case 6:
                    tileVal = 32;
                    break;
                    //north and south
                case 5:
                    tileVal = 27;
                    break;
                    //south
                case 4:
                    tileVal = 27;
                    break;
                    //north and east
                case 3:
                    tileVal = 40;
                    break;
                    //east
                case 2:
                    tileVal = 26;
                    break;
                    //north
                case 1:
                    tileVal = 27;
                    break;
                default:
                    tileVal = 26;
            }
        } else if (tileVal == WAREHOUSE) {
            //tileVal=42;
            bool evenY = tCoords.y % 2 == 0;
            bool evenX = tCoords.x % 2 == 0;
            if (evenY) {
                if (evenX) {
                    //top left
                    tileVal = 34;
                } else {
                    //top right
                    tileVal = 35;
                }
            } else {
                if (evenX) {
                    //bottom left
                    tileVal = 42;
                } else {
                    //bottom right
                    tileVal = 43;
                }
            }

        } else if (tileVal == LIGHTHOUSE) {
            tileVal = 39;

        } else if (tileVal == DOCK) {

            int bmr = getBitmaskFromTileId(t_id, tMask_roads);
            TileStruct *aboveT = &m_tiles[getTileIdFromCoords(sf::Vector2i(tCoords.x, tCoords.y - 1))];
            TileStruct *belowT = &m_tiles[getTileIdFromCoords(sf::Vector2i(tCoords.x, tCoords.y + 1))];
            if (tCoords.y - 1 >= 0 && (*aboveT).basicTile == ISLAND) {
                tileVal = 51;
            } else if (tCoords.y < m_rowNo && (*belowT).basicTile == ISLAND) {
                tileVal = 51;
            } else {
                switch (bmr) {
                    case 5:
                        tileVal = 51;
                        break;
                    case 4:
                        tileVal = 51;
                        break;
                    case 1:
                        tileVal = 51;
                        break;
                    default:
                        tileVal = 24;
                }
            }
        } else if (tileVal == ROADWATER) {
            tileVal = 25;

            int bmr = getBitmaskFromTileId(t_id, tMask_roads);
            TileStruct *aboveT = &m_tiles[getTileIdFromCoords(sf::Vector2i(tCoords.x, tCoords.y - 1))];
            TileStruct *belowT = &m_tiles[getTileIdFromCoords(sf::Vector2i(tCoords.x, tCoords.y + 1))];

            if (tCoords.y - 1 >= 0 && (*aboveT).basicTile == ISLAND) {
                tileVal = 52;
            } else if (tCoords.y < m_rowNo && (*belowT).basicTile == ISLAND) {
                tileVal = 52;
            } else {
                switch (bmr) {
                    case 5:
                        tileVal = 52;
                        break;
                    case 4:
                        tileVal = 52;
                        break;
                    case 1:
                        tileVal = 52;
                        break;
                    default:
                        tileVal = 25;
                }
            }
        }

        //randomise
        if (tileVal == 11) {
            float noise = (sin(tCoords.y * tCoords.x + cos(tCoords.x)) + 1.0) / 2.0;
            if (noise < 0.2) { tileVal = 12; }
            else if (noise < 0.3) { tileVal = 6; }
            else if (noise < 0.4) { tileVal = 13; }
            else if (noise < 0.5) { tileVal = 14; }
        } else if (tileVal == 20) {
            if (sin(tCoords.y * tCoords.x + cos(tCoords.x)) > 0.5) { tileVal = 21; }
        }

        m_tiles[t_id].displayTile = tileVal;
        //flipx?
        m_tiles[t_id].sx = sx;
        //flipy?
        m_tiles[t_id].sy = sy;


    }

    void updateTile(int t_id, Tile t_newBasicTile) {
        m_tiles[t_id].basicTile = t_newBasicTile;
        sf::Vector2i coords = getTileCoordsFromId(t_id);
        //updateTile(t_id);
        for (int y = coords.y - 1; y < coords.y + 2; y++) {
            for (int x = coords.x - 1; x < coords.x + 2; x++) {
                if (x > 0 && x < m_colNo - 1 && y > 0 && y < m_rowNo - 1) {
                    updateTile(getTileIdFromCoords(sf::Vector2i(x, y)));
                }
            }
        }
    }

    void drawTile(int t_displayTile, int t_x, int t_y, int sx, int sy) {
        int tileMapX = t_displayTile % tileMapNoX;
        int tileMapY = t_displayTile / tileMapNoX;
        tile.setOrigin(0, 0);
        tile.setTextureRect(sf::IntRect(tileMapX * tileSize, tileMapY * tileSize, tileSize, tileSize));
        tile.setOrigin(tileSize / 2, tileSize / 2);
        tile.setScale(scaleFactor.x * (sx), scaleFactor.y * (sy));
        tile.setOrigin(0, 0);
        tile.setPosition((t_x - ((sx) - 1) / 2) * tileSize * scaleFactor.x,
                         (t_y - ((sy) - 1) / 2) * tileSize * scaleFactor.y);
        (*m_win).draw(tile);
    }

    void drawTileId(int t_id) {
        TileStruct *tileToDraw = &m_tiles[t_id];
        sf::Vector2i coords = getTileCoordsFromId(t_id);
        drawTile(tileToDraw->displayTile, coords.x, coords.y, tileToDraw->sx, tileToDraw->sy);
    }

    void draw(int secs) {
        for (int i = 0; i < m_tiles.size(); i++) {
            //int mapVal=map[((y*colNo)+x)];
            Tile basicTile = m_tiles[i].basicTile;
            //int tileVal=displayMap[displayParamNo*((y*colNo)+x)];
            int displayTile = m_tiles[i].displayTile;
            if (secs % 2 == 0) {
                switch (displayTile) {
                    case 4:
                        displayTile = 3;
                        break;
                }
            }
            sf::Vector2i coords = getTileCoordsFromId(i);
            int sx = m_tiles[i].sx;
            int sy = m_tiles[i].sy;
            drawTileId(i);
            if (displayTile == 24 || displayTile == 25 || displayTile == 51 || displayTile == 52) {
                int newDisplayTile = 17;
                int newX = coords.x;
                int newY = coords.y - 1;
                if (displayTile == 51 || displayTile == 52) {
                    newDisplayTile = 16;
                    newX = coords.x - 1;
                    newY = coords.y;
                    drawTile(newDisplayTile, newX, newY, sx, sy);
                } else {
                    drawTile(newDisplayTile, newX, newY, sx, sy);
                    newDisplayTile = 1;
                    newX = coords.x;
                    newY = coords.y + 1;
                    drawTile(newDisplayTile, newX, newY, sx, sy);
                }


            } else if (basicTile == LIGHTHOUSE) {
                int newX = coords.x;
                int newY = coords.y - 1;
                drawTile(31, newX, newY, 1, 1);
                newY--;
                drawTile(23, newX, newY, 1, 1);
                newY--;
                drawTile(15, newX, newY, 1, 1);
            }
        }
    }

    float getH(int t_id, int t_idStart, int t_idEnd) {
        sf::Vector2i startCoords = getTileCoordsFromId(t_idStart);
        sf::Vector2i endCoords = getTileCoordsFromId(t_idEnd);
        sf::Vector2i nCoords = getTileCoordsFromId(t_id);
        sf::Vector2i nDistFromEnd = endCoords - nCoords;
        sf::Vector2i nDistFromStart = startCoords - nCoords;
        float h=(nDistFromEnd.x * nDistFromEnd.x) + (nDistFromEnd.y * nDistFromEnd.y);
        //float h = std::sqrt((nDistFromEnd.x * nDistFromEnd.x) + (nDistFromEnd.y * nDistFromEnd.y));
        h += (std::abs(nDistFromEnd.x * nDistFromStart.y - nDistFromStart.x * nDistFromEnd.y)) * 0.1;
        return h;
    }


    std::vector<int> getPath(int t_idStart,int t_idEnd,std::vector<Tile> t_mask){
        sf::Vector2i startCoords=getTileCoordsFromId(t_idStart);
        sf::Vector2i endCoords=getTileCoordsFromId(t_idEnd);


        std::vector<int> toVisit;
        toVisit.push_back(t_idStart);
        m_tiles[t_idStart].visited=true;
        m_tiles[t_idStart].costSoFar=0;
        while(!toVisit.empty()){


            int id=toVisit.back();



            for(int possibleId:toVisit){
                if(id!=possibleId && !m_tiles[possibleId].visited){
                    float costId= m_tiles[id].costSoFar+m_tiles[id].cost+getH(id,t_idStart,t_idEnd);
                    float costPossibleId= m_tiles[possibleId].costSoFar+m_tiles[possibleId].cost+getH(possibleId,t_idStart,t_idEnd);

                    if(costPossibleId<costId){
                        id=possibleId;
                        continue;
                    }
                }
            }


            sf::Vector2i coords=getTileCoordsFromId(id);
            m_tiles[id].visited=true;

            toVisit.pop_back();

            std::vector<int> &ns=m_tiles[id].ns;
            for(int i=0;i<ns.size();i++){
                int n=ns[i];
                if(!(m_tiles[n].visited)){
                    bool maskOk=false;
                    for(int j=0;j<t_mask.size();j++) {
                        //check if in mask
                        if(m_tiles[n].basicTile==t_mask[j]) {
                            //if so break
                            maskOk=true;
                            break;
                        }
                    }
                    if(maskOk) {

                        toVisit.push_back(n);
                        float h=getH(n,t_idStart,t_idEnd);
                        //if ((nCoords.x - coords.x) != 0 && (nCoords.y - coords.y) != 0) { h *= 0.5; }

                        int costTemp = m_tiles[id].costSoFar + m_tiles[id].cost;

                        if (costTemp < m_tiles[n].costSoFar) {
                            m_tiles[n].costSoFar = costTemp;
                            m_tiles[n].parent = id;
                        }
                    }
                }
            }

        }

        //try to trace path
        std::vector<int> pathRes;
        int nodePar=m_tiles[t_idEnd].parent;
        pathRes.push_back(t_idEnd);
        while(nodePar!=t_idStart){
            if(nodePar==-1){
                printf("No path found!\n");
                pathRes.clear();
                break;
            }
            pathRes.push_back(nodePar);
            nodePar=m_tiles[nodePar].parent;
        }
        pathRes.push_back(t_idStart);
        std::reverse(pathRes.begin(),pathRes.end());


        resetTiles();


        return pathRes;
    }

};


#endif //SFMLREVOLUTIONISLE_TILES_H
