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
#include <cmath>


#define NODE_MAX_COST 500



extern int tileMapNoX,tileMapNoY;
extern sf::Sprite tile;

enum Tile{
    NILTILE=-1,
    WATER=0,
    ISLAND=1,
    ROAD=2,
    WAREHOUSE=3,
    LIGHTHOUSE=4,
    DOCK=5,
    ROADWATER=6,
    METEOR=7
};

extern int tileWeights[];
extern const std::vector<Tile> tMask_island_general;
extern const std::vector<Tile> tMask_island;
extern const std::vector<Tile> tMask_island_roads;
extern const std::vector<Tile> tMask_roads;
extern const std::vector<Tile> tMask_water;
extern const std::vector<Tile> tMask_land;
extern const std::vector<Tile> tMask_meteor;
extern const std::vector<Tile> tMask_all;



extern int tileSize;
extern sf::Vector2f scaleFactor;
extern int brushSize;
extern Tile brushTile;

enum IslandType{SQUARE_ISLAND,CIRCLE_ISLAND,TINY_ISLAND,NOISE_ISLAND};

struct TileStruct{
    Tile basicTile;
    int displayTile;
    int sx,sy;
    int cost;
    //TileStruct* parent=nullptr;
    int parent=-1;
    int id;
    int costSoFar=NODE_MAX_COST*NODE_MAX_COST*1000;
    //std::vector<TileStruct*> ns;
    std::vector<int> ns;
    bool visited=false;
    TileStruct(Tile t_basicTile,int t_id){
        basicTile=t_basicTile;
        id=t_id;
    }

};

class TileMap {
private:
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
    int m_colNo;
    int m_rowNo;
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


    void generateIsland(IslandType t_islandToMake);

    bool areaIsOnly(sf::Vector2i &t_coords,int t_brushSize,std::vector<Tile> const &mask);


    sf::Vector2i getTileCoordsFromId(int t_id);

    sf::Vector2i getTilePosFromId(int t_id);

    int getTileIdFromCoords(sf::Vector2i t_coords);
    TileStruct* getTileFromCoords(sf::Vector2i t_coords);

    sf::Vector2i getTileCoordsFromPos(sf::Vector2f t_pos, int t_brushSize);

    TileStruct* getTileFromId(int t_id);

    int getTileIdFromPos(sf::Vector2f t_pos, int t_brushSize);

    TileStruct* getTileFromPos(sf::Vector2f t_pos, int t_brushSize);

    int getBitmaskFromTileId(int t_id, std::vector<Tile> t_mask);

    void resetTile(int t_id);

    void resetTiles();

    int getMiddleTileId();

    sf::Vector2i getMiddleTileCoords();

    sf::Vector2i getMiddleTilePos();

    sf::Vector2i getNearestTileAt(const sf::Vector2f &t_pos,const Tile &t_tile);

    void updateTile(int t_id);

    void updateTile(int t_id, Tile t_newBasicTile);

    void drawTile(int t_displayTile, int t_x, int t_y, int sx, int sy);

    void drawTileId(int t_id);

    void draw(int secs);

    float getH(int t_id, int t_idStart, int t_idEnd);


    std::vector<int> getPath(int t_idStart,int t_idEnd,std::vector<Tile> const* t_mask);

};


#endif //SFMLREVOLUTIONISLE_TILES_H
