//
// Created by Ben on 22/02/2019.
//


#include <vector>
#include <iostream>
#include "tiles.h"
#include "entities.h"
#include "click.h"
#include "buildings.h"
#include "resources.h"

sf::Vector2i oldTileClick;
MouseMode mouseMode=DESTROY;

void drawBrush(Tile tileToDraw,int brushSize,TileMap* tMap,sf::Vector2i tileClick){
    for (int y = 0; y < brushSize; y++) {
        for (int x = 0; x < brushSize; x++) {
            sf::Vector2i curTileCoords = tileClick;
            curTileCoords.x += x;
            curTileCoords.y += y;
            int tileId = tMap->getTileIdFromCoords(curTileCoords);
            Tile tileVal = tMap->getTileFromId(tileId)->basicTile;
            if(tileVal!=METEOR) {
                tMap->updateTile(tileId, tileToDraw);
            }

        }

    }
    //end of loops
}

void Click(int button,int _x,int _y,TileMap* tMap,ResourceBoss* res){
    int tileId = (*tMap).getTileIdFromPos(sf::Vector2f(_x, _y), brushSize);
    //printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    int colNo = tMap->m_colNo;
    int rowNo = tMap->m_rowNo;
    if(mouseMode==CONTROL_FRIENDS){
        if (button == 1) {


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
                    sf::Vector2f fPos = f->m_pos;
                    if (((fPos.x > selectionBoxStart.x && fPos.x < selectionBoxEnd.x) ||
                         (fPos.x < selectionBoxStart.x && fPos.x > selectionBoxEnd.x))
                        && ((fPos.y > selectionBoxStart.y && fPos.y < selectionBoxEnd.y) ||
                        (fPos.y < selectionBoxStart.y && fPos.y > selectionBoxEnd.y))
                        && !f->getSaved()) {
                        selectedFriends.push_back(i);
                    }
                }
            }


        }
    }
    if ((_x >= 0 && _x <= colNo * tileSize * scaleFactor.x) &&
        (_y >= 0 && _y <= rowNo * tileSize * scaleFactor.y) && tileId > -1 && tileId < rowNo * colNo + 1) {
        sf::Vector2i tileClick = tMap->getTileCoordsFromPos(sf::Vector2f(_x, _y), brushSize);

        Tile tileVal = tMap->getTileFromId(tileId)->basicTile;


        if (tileClick != oldTileClick) {
            //printf("Click at %i,%i TILE: %f, %f (id %i)\n",_x,_y,tileClick.x,tileClick.y,tileId);
            oldTileClick = tileClick;
            //printf("clicked tile %f,%f\n", tileClick.x, tileClick.y);

            //getting if there is any other tile than island in a 2x2 grid from this coord
            bool justIsland = tMap->areaIsOnly(tileClick, 2, tMask_island);
            bool justWater = tMap->areaIsOnly(tileClick, 2, tMask_water);
            if (button == 2) {
                for (auto f = selectedFriends.begin(); f != selectedFriends.end(); ++f) {
                    if(!friends[*f].getSaved()){
                        friends[*f].tryPath(tileId);
                    }
                }
            } else {
                switch (mouseMode) {
                    case DESTROY:
                        if (justIsland && res->hasPlayerFinishedWave()) {
                            drawBrush(WATER, 2, tMap, tileClick);
                            res->addResources(ISLAND_PRICE);
                        } else if (tileVal != WATER) {
                            res->destroyWarehouses(tileClick.x, tileClick.y, 2);
                            res->destroyLighthouses(tileClick.x, tileClick.y, 2);
                            drawBrush(ISLAND, 2, tMap, tileClick);
                        }
                        break;
                    case PLACE_LAND:
                        if (justWater && res->hasPlayerFinishedWave()) {
                            if (res->addResources(-ISLAND_PRICE)) {
                                drawBrush(ISLAND, 2, tMap, tileClick);
                            }
                        }
                        break;
                    case PLACE_ROAD:
                        if (tileVal == ISLAND && res->hasPlayerFinishedWave()) {
                            drawBrush(ROAD, 1, tMap, tileClick);
                        }
                        break;
                    case BUILD_LIGHTHOUSE:
                        if (tileVal == ISLAND && res->addResources(-Lighthouse::cost)) {
                            res->buildLighthouse(tileClick.x, tileClick.y);
                            drawBrush(LIGHTHOUSE, 1, tMap, tileClick);
                        }
                        break;
                    case BUILD_WAREHOUSE:
                        if (justIsland && res->addResources(-Warehouse::cost)) {
                            res->buildWarehouse(tileClick.x, tileClick.y);
                            drawBrush(WAREHOUSE, 2, tMap, tileClick);
                        }
                        break;
                    case CONTROL_FRIENDS:


                        break;
                }
            }


        }
    }

}

sf::Vector2i oldTileHover=sf::Vector2i(-100,-100);
void Hover(int _x,int _y,TileMap* tMap,ResourceBoss* res){
    int tileId=(*tMap).getTileIdFromPos(sf::Vector2f(_x,_y),1);
    //printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    int colNo=tMap->m_colNo;
    int rowNo=tMap->m_rowNo;
    if((_x>=0 && _x<=colNo*tileSize*scaleFactor.x) && (_y>=0 && _y<=rowNo*tileSize*scaleFactor.y) && tileId>-1 && tileId<rowNo*colNo+1) {
        sf::Vector2i tileHover = tMap->getTileCoordsFromPos(sf::Vector2f(_x, _y), 1);

        Tile tileVal = tMap->getTileFromId(tileId)->basicTile;


        if (tileHover != oldTileHover) {
            //printf("new hover!\n");
            //printf("Click at %i,%i TILE: %f, %f (id %i)\n",_x,_y,tileClick.x,tileClick.y,tileId);
            Building* oldBuildingHover=res->getBuildingByCoords(oldTileHover);
            if(oldBuildingHover){
                //printf("(old building)\n");
                oldBuildingHover->m_showUI=false;
            }

            Building* buildingHover=res->getBuildingByCoords(tileHover);
            if(buildingHover){
                //printf("(building)\n");
                buildingHover->m_showUI=true;
            }

            oldTileHover = tileHover;
            //printf("clicked tile %f,

        }
    }
}