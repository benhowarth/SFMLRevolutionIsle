//
// Created by Ben on 22/02/2019.
//

#include "tiles.h"
#include "entities.h"
#include "click.h"

sf::Vector2i oldTileClick;
MouseMode mouseMode=PLACE_LAND;

void drawBrush(Tile tileToDraw,TileMap* tMap,sf::Vector2i tileClick){
    for (int y = 0; y < brushSize; y++) {
        for (int x = 0; x < brushSize; x++) {
            sf::Vector2i curTileCoords = tileClick;
            curTileCoords.x += x;
            curTileCoords.y += y;
            int tileId = tMap->getTileIdFromCoords(curTileCoords);
            Tile tileVal = tMap->getTileFromId(tileId)->basicTile;
            switch(tileToDraw){
                case WATER:
                    if(tileVal==ISLAND){tMap->updateTile(tileId,WATER);}break;
                case ISLAND:
                    if(tileVal==WATER){tMap->updateTile(tileId,ISLAND);}break;
                case ROAD:
                    if(tileVal==ISLAND){tMap->updateTile(tileId,ROAD);}break;
                case LIGHTHOUSE:
                    if(tileVal==ISLAND || tileVal==ROAD){tMap->updateTile(tileId,LIGHTHOUSE);}break;
                case WAREHOUSE:
                    if(tileVal==ISLAND || tileVal==ROAD){tMap->updateTile(tileId,WAREHOUSE);}break;
            }

        }

    }
    //end of loops
}

void Click(int button,int _x,int _y,TileMap* tMap){
    int tileId=(*tMap).getTileIdFromPos(sf::Vector2f(_x,_y),brushSize);
    //printf("old: %f,%f vs new: %f,%f (result:%b)\n",oldTileClick.x,oldTileClick.y,tileClick.x,tileClick.y,(tileClick==oldTileClick));
    int colNo=tMap->m_colNo;
    int rowNo=tMap->m_rowNo;
    if((_x>=0 && _x<=colNo*tileSize*scaleFactor.x) && (_y>=0 && _y<=rowNo*tileSize*scaleFactor.y) && tileId>-1 && tileId<rowNo*colNo+1) {
        sf::Vector2i tileClick=(*tMap).getTileCoordsFromPos(sf::Vector2f(_x,_y),brushSize);




        if(tileClick!=oldTileClick){
            //printf("Click at %i,%i TILE: %f, %f (id %i)\n",_x,_y,tileClick.x,tileClick.y,tileId);
            oldTileClick=tileClick;
            //printf("clicked tile %f,%f\n", tileClick.x, tileClick.y);


            switch(mouseMode) {
                case DESTROY:
                    drawBrush(WATER, tMap, tileClick);break;
                case PLACE_LAND:
                    drawBrush(ISLAND,tMap,tileClick);break;
                case PLACE_ROAD:
                    drawBrush(ROAD,tMap,tileClick);break;
                case BUILD_LIGHTHOUSE:
                    drawBrush(LIGHTHOUSE,tMap,tileClick);break;
                case BUILD_WAREHOUSE:
                    drawBrush(WAREHOUSE,tMap,tileClick);break;
                case CONTROL_FRIENDS:
                    if(button==1){


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
                                    && (fPos.y > selectionBoxStart.y && fPos.y < selectionBoxEnd.y) ||
                                    (fPos.y < selectionBoxStart.y && fPos.y > selectionBoxEnd.y)) {
                                    selectedFriends.push_back(i);
                                }
                            }
                        }


                    }
                    else if(button==2){
                        for (auto f = selectedFriends.begin(); f != selectedFriends.end(); ++f) {
                            friends[*f].tryPath(tileId);
                        }
                    }
                    break;
                }

        }
    }
}