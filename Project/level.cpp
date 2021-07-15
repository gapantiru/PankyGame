#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "gun.h"

#include <SDL.h>

#include "tinyxml2.h"

#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0,0)),
	_bkName("bk/skull.png")
{
	this->loadMap(mapName, graphics);
}

void Level::clearMem() {


	this->_tileList.clear();
	this->_tilesets.clear();
	this->_collisionRects.clear();
	this->_slopes.clear();

	this->_fireRects.clear();

	this->_animatedTileList.clear();
	this->_animatedTileInfos.clear();

	for (int i = 0; i < this->_enemies.size(); i++) {
		delete this->_enemies.at(i);
	}
	for (int i = 0; i < this->_guns.size(); i++) {
		delete this->_guns.at(i);
	}
	for (int i = 0; i < this->_bullets.size(); i++) {
		delete this->_bullets.at(i);
	}
	for (int i = 0; i < this->_cars.size(); i++) {
		delete this->_cars.at(i);
	}
	this->_enemies.clear();
	this->_guns.clear();
	this->_bullets.clear();
	this->_cars.clear();

	this->_enemies.shrink_to_fit();
	this->_guns.shrink_to_fit();
	this->_bullets.shrink_to_fit();
	this->_cars.shrink_to_fit();

}

Level::~Level() {

}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	//TEMPORARY CODE TO LOAD THE BACKGROUND
	if (this->_mapName == "panky2")
		this->_bkName = "bk/skull.png";
	else if (this->_mapName == "map2")
		this->_bkName = "bk/wall.png";
	else if (this->_mapName == "map3")
		this->_bkName = "bk/fireflies.png";
	this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage(this->_bkName));
	this->_size = Vector2(1280, 960);

	//background img goes here!!!-|


	//Parse the .tmx file

	XMLDocument doc;
	std::stringstream ss;
	ss << "maps/" << mapName << ".tmx"; // mapName gets map file name, resulting in smth like maps/mapName.tmx
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
			ss <<"maps/"<<source;	//source of tileset .png
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
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(		//round with ceil from float to int//
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "fire") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_fireRects.push_back(Rectangle(		//round with ceil from float to int//
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "stairs") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_stairs = Rectangle(		//round with ceil from float to int//
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						);

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "movingTiles") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_cars.push_back(new Car(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
							std::floor(y) * globals::SPRITE_SCALE)));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//other object group go here with an else if (ss.str() == "nameofw/e")
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//now we have each of the pairs. loop through list of pairs
							//and split them in Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i : i - 1).y) *globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i + 1 : i).y) *globals::SPRITE_SCALE)
							));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			//add spawn point
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while(pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y)*globals::SPRITE_SCALE);
						}
						if (ss.str() == "razor") {
							this->_spawnPointR = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y)*globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");

					}
				}
			}
			//GET SPAWN BOX FROM MAP FOR ENEMIES
			else if (ss.str() == "boxes") {
				float x, y, w, h;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						w = pObject->FloatAttribute("width");
						h = pObject->FloatAttribute("height");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "health") {
							this->_health=Rectangle(std::ceil(x)*globals::SPRITE_SCALE, std::ceil(y)*globals::SPRITE_SCALE,
								std::ceil(w)*globals::SPRITE_SCALE, std::ceil(h)*globals::SPRITE_SCALE);
						}
						pObject = pObjectGroup->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime, Player &player, Graphics &graphics) {


	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
	for (int i = 0; i < this->_guns.size(); i++) {
		this->_guns.at(i)->update();
		//CHECK GUN COLLISIONS
		if (this->_guns.at(i)->getStop()) {
			delete _guns.at(i);
			_guns.erase(_guns.begin() + i);
		}
	}
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets.at(i)->update();
		if (this->_bullets.at(i)->getStop()) {
			delete _bullets.at(i);
			_bullets.erase(_bullets.begin() + i);
		}
	}
	//CHECK IF THERE ARE ANY ENEMIES WITH 0 HP AND ERASES THEM----------------------------------------------------------------//

	for (int i = 0; i < this->_enemies.size(); i++) {
		if (_enemies.at(i)->getCurrentHealth() == 0) {
			delete _enemies.at(i);
			_enemies.erase(_enemies.begin() + i);
		}
	}
	for (int i = 0; i < this->_cars.size(); i++) {
		this->_cars.at(i)->update(elapsedTime);
	}
	for (int i = 0; i < this->_cars.size(); i++) {
		std::vector<Rectangle> otherCars;
		if ((otherCars = this->checkTileCollisions(this->_cars.at(i)->getBoundingBox())).size() > 0) {
			this->_cars.at(i)->handleTileCollisions(otherCars);
		}
	}
} 

void Level::draw(Graphics &graphics) {
/*	//draw bkg
	//img coord.
	SDL_Rect sourceRect = {0, 0, 64, 64};
	SDL_Rect destRect;
	//destRect.w = 64;			 //OTPIONAL
	//destRect.h = 64;			//
	for (int x = 0; x < this->_size.x / 64; x++) {
		for (int y = 0; y < this->_size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE;
			destRect.y = y * 64 * globals::SPRITE_SCALE;
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
		}
	}


*/ //get rid of drawing the background 
	SDL_Rect sourceRect = { 0,0,800,608 };
	SDL_Rect destRect;
	destRect.w = 800;
	destRect.h = 608;
	destRect.x = 0;
	destRect.y = 0;
	graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);

	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_cars.size(); i++) {
		this->_cars.at(i)->draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}
	for (int i = 0; i < this->_guns.size(); i++) {
		this->_guns.at(i)->draw(graphics);
	}

	for (int i = 0; i < this->_bullets.size(); i++) {
		this->_bullets.at(i)->draw(graphics);
	}
}

//returns colisions in vector others;
std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}
//return cars collisions in vector others
std::vector<Rectangle> Level::checkCarsCollisions(const Rectangle &other,Player &player) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_cars.size(); i++) {
		if (this->_cars.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_cars.at(i)->getBoundingBox());
				
		}
	}
	return others;
}

//returns fire collisions in vector others;
std::vector<Rectangle> Level::checkFireCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_fireRects.size(); i++) {
		if (this->_fireRects.at(i).collidesWith(other)) {
			others.push_back(this->_fireRects.at(i));
		}
	}
	return others;
}


//std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
//	std::vector<Slope> others;
//	for (int i = 0; i < this->_slopes.size(); i++) {
//		if (this->_slopes.at(i).collidesWith(other)) {
//			others.push_back(this->_slopes.at(i));
//		}
//	}
//	return others;
//}
std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
	std::vector<Enemy*> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}
//DELETES ANY PROJECTILES THAT COLLIDE WITH AN ENEMY OR WITH THE UPPER WALL-----------------------------------------//
std::vector<Gun*> Level::checkGunCollisions(const Rectangle &other) {
	std::vector<Gun*> others;
	for (int i = 0; i < this->_guns.size(); i++) {

		if (this->_guns.at(i)->getBoundingBox().collidesWithProj(other)) {
			others.push_back(this->_guns.at(i));
			delete _guns.at(i);
			_guns.erase(_guns.begin() + i);
			printf("%d ", _guns.size());
		}
	}
	return others;
}

//DELETE ANY PROJECTILES THAT COLLIDE WITH THE PLAYER -----------------------------------------//
bool Level::checkBulletCollisions(const Rectangle &other) {
	bool a = false;
	for (int i = 0; i < this->_bullets.size(); i++) {
		if (this->_bullets.at(i)->getBoundingBox().collidesWithProj(other)) {
			delete _bullets.at(i);
			_bullets.erase(_bullets.begin() + i);
			a = true;
		}
	}
	return a;
}
//CHECK PLAYER COLLISIONS WITH HEALTH BOX
bool Level::checkHealthCollisions(Player &player) {
	if (player.getBoundingBox().collidesWith(this->_health))
		return true;
	else 
		return false;
}
//CHECK STAIRS COLLISION

//CHECK RAZOR COLLISION WITH PLAYER
bool Level::checkPlayerCollisions(Razor &razor, const Rectangle &player) {
	if (razor.getBoundingBox().collidesWith(player))
		return true;
	else
		return false;
}
//CHECK PRO COLLISIONS WITH TILES
void Level::checkProCollisions() {
	if(this->_guns.size()!=0)
	for (int i = 0; i < this->_guns.size(); i++) {
		for (int j = 0; j < this->_collisionRects.size(); j++) {
			if (this->_collisionRects.at(j).collidesWith(this->_guns.at(i)->getBoundingBox())) {
				delete _guns.at(i);
				_guns.erase(_guns.begin() + i);
				break;
			}
		}
	}
	if(this->_bullets.size()!=0)
	for (int i = 0; i < this->_bullets.size(); i++) {
		for (int j = 0; j < this->_collisionRects.size(); j++) {
			if (this->_collisionRects.at(j).collidesWith(this->_bullets.at(i)->getBoundingBox())) {
				delete _bullets.at(i);
				_bullets.erase(_bullets.begin() + i);
				break;
			}
		}
	}
}


const Vector2 Level::getPlayerSpawnpoint() const {
	return this->_spawnPoint;
}

const Vector2 Level::getRazorSpawnPoint() const {
	return this->_spawnPointR;
}

const Rectangle Level::getHealthRect() const {
	return this->_health;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {

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

void Level::createGun(Graphics &graphics, Vector2 spawnPoint,Direction direction) {
	_guns.push_back(new Projectile(graphics, spawnPoint,direction));
}

void Level::createBullet(Graphics &graphics,Enemy &enemy,Player &player) {
	this->_bullets.push_back(new Bullet(graphics, Vector2(enemy.getX(), enemy.getY()), player.getX(), player.getY()));
}

std::vector<Gun*> Level::getBullet() {
	return this->_bullets;
}

//return enemy
std::vector<Enemy*> Level::getEnemy() {
	return this->_enemies;
}
//return guns
std::vector<Gun*> Level::getGuns() {
	return this->_guns;
}

//handle gun collisions
//void Level::handleGunCollisions(std::vector<Gun*> &others) {
//	for (int i = 0; i < others.size(); i++) {
//		//others.at(i)->startQ();
//		////!!!!!!--------------------------------temporary code to delete projectiles-------------------------------------------------!!!!!!//
//		//delete _guns.at(i);
//		//_guns.erase(_guns.begin() + i);
//		//printf("%d", _guns.size());
//	}
//}


//_________________________________________LOADING THE PAUSE MENU_________________________________________//

void Level::loadPause(Graphics &graphics) {
	this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("bk/looseM.png"));
	this->_size = Vector2(1600, 1216);
}

void Level::updatePause(int elapsedTime, Graphics &graphics) {

}

void Level::drawPause(Graphics &graphics) {
	graphics.clear();
	SDL_Rect sourceRect = { 0,0,800,608 };
	SDL_Rect destRect = { 0,0,800,608 };
	graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
	graphics.flip();
}
//__________________________________________________LOADING THE WIN MENU________________________________________________________//
void Level::loadWin(Graphics &graphics) {
	this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(),
		graphics.loadImage("bk/winM.png"));
	this->_size = Vector2(1600, 1216);
}

void Level::updateWin(int elapsedTime, Graphics &graphics) {

}
void Level::drawWin(Graphics &graphics) {
	graphics.clear();
	SDL_Rect sourceRect = { 0,0,800,608 };
	SDL_Rect destRect = { 0,0,800,608 };
	graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
	graphics.flip();
}