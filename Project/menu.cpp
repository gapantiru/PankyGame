#include "menu.h"

#include "graphics.h"
#include "globals.h"
#include "utils.h"

#include <SDL.h>

#include "tinyxml2.h"

#include<sstream>
#include<algorithm>
#include<cmath>
#include "level.h"

using namespace tinyxml2;

Menu::Menu() {}

Menu::Menu(std::string menuName, Vector2 spawnPoint, Graphics &graphics) :
	_menuName(menuName),
	_spawnPoint(spawnPoint),
	option(0)
{
	this->loadMenu(menuName, graphics);
}

Menu::~Menu(){
	this->_backgroundTexture=NULL;
	this->_optionsTexture=NULL;
	this->_nameTexture=NULL;

	this->_tileList.clear();
	this->_tilesets.clear();
	this->_collisionRects.clear();

	this->_animatedTileList.clear();
	this->_animatedTileInfos.clear();
}

void Menu::loadMenu(std::string menuName, Graphics &graphics) {
	this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("bk/fireflies.png"));
	this->_optionsTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("bk/help.png"));
	this->_nameTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("bk/gameName1.png"));
	this->_size = Vector2(1600, 1216);

	//CODE FOR LOADING MENU TILES FROM TMX

	//Parse the .tmx file

	XMLDocument doc;
	std::stringstream ss;
	ss << "maps/" << menuName << ".tmx"; // mapName gets map file name, resulting in smth like maps/mapName.tmx
	doc.LoadFile(ss.str().c_str()); //converts ss to string and string to c string

	XMLElement* mapNode = doc.FirstChildElement("map");

	//get the width and height of whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//get width and height of tiles and store it _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//loading the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source"); //returns source from child element "image"
			char* path;
			std::stringstream ss;
			ss << "maps/" << source;	//source of tileset .png
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			//----------//get all that animation for that tileset
			XMLElement* ptileA = pTileset->FirstChildElement("tile");
			if (ptileA != NULL) {
				while (ptileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = ptileA->IntAttribute("id") + firstgid;
					ati.TilesetFirstGid = firstgid;
					XMLElement* pAnimation = ptileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}

							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					ptileA = ptileA->NextSiblingElement("tile");
				}
			}

			pTileset = pTileset->NextSiblingElement("tileset"); //moves to next tileset,if does not exist, continue with code
		}
	}

	//loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {

		while (pLayer) {

			//loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {

				while (pData) {

					//loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {

						int tileCounter = 0; //keep track of tiles
						while (pTile) {
							//build each tile here
							//if gid is 0 no tile should be drawn, continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							//get the tileset for this gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1) {
								//no tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//--------------------------//get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy); //tile position on map
																		 //--------------------------//calculate position of tile in tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < _animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i),
										tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.Duration,
									tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}

							else {
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");

						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer"); //same thing as for tilesets
		}
	}

	//parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "options") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						if (ss.str() == "start") {
							this->start = Box(		//round with ceil from float to int//
								std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE,
								std::ceil(width) * globals::SPRITE_SCALE,
								std::ceil(height) * globals::SPRITE_SCALE
							);
						}
						if (ss.str() == "quit") {
							this->quit = Box(		//round with ceil from float to int//
								std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE,
								std::ceil(width) * globals::SPRITE_SCALE,
								std::ceil(height) * globals::SPRITE_SCALE
							);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}



void Menu::update(int elapsedTime, Graphics &graphics) {
		this->_animatedTileList.at(this->option).update(elapsedTime);
}

void Menu::draw(Graphics &graphics) {
	//CODE FOR LOADING A IMAGE FROM A 64x64 BASE IMAGE

	//SDL_Rect sourceRect = { 0, 0, 64, 64 };
	//SDL_Rect destRect;
	//destRect.w = 64;			 //OTPIONAL
	//destRect.h = 64;			//
	//for (int x = 0; x < this->_size.x / 64; x++) {
	//	for (int y = 0; y < this->_size.y / 64; y++) {
	//		destRect.x = x * 64 * globals::SPRITE_SCALE;
	//		destRect.y = y * 64 * globals::SPRITE_SCALE;
	//		destRect.w = 64 * globals::SPRITE_SCALE;
	//		destRect.h = 64 * globals::SPRITE_SCALE;
	//		graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
	//	}
	//}

	//CODE FOR LOADING A FULL SIZE IMAGE

	SDL_Rect sourceRect = { 0,0,800,608 };
	SDL_Rect destRect = {0,0,800,608};
	graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
	graphics.blitSurface(this->_nameTexture, &sourceRect, &destRect);
	if (this->option == 1)
		graphics.blitSurface(this->_optionsTexture, &sourceRect, &destRect);


	//draw tiles
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}

		this->_animatedTileList.at(this->option).draw(graphics);
}

Vector2 Menu::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {

	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = gid % (tilesetWidth / tileWidth) - 1;
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));  // amount
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}