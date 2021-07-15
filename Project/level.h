#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"
#include "animatedtile.h"
#include "gun.h"
#include "razor.h"
#include "car.h"

#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <memory>

class Graphics;
class Enemy;
class Player;
class Razor;

struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics);
	void clearMem();
	~Level();

	void update(int elapsedTime, Player &player, Graphics &graphics);
	void draw(Graphics &graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	std::vector<Rectangle> checkFireCollisions(const Rectangle &other);
	std::vector<Rectangle> checkCarsCollisions(const Rectangle &other,Player &player);

	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);
	std::vector<Gun*> checkGunCollisions(const Rectangle &other);

	virtual bool checkPlayerCollisions(Razor &razor, const Rectangle &player);
	virtual bool checkBulletCollisions(const Rectangle &other);
	virtual bool checkHealthCollisions(Player &player);
	virtual void checkProCollisions();

	void handleGunCollisions(std::vector<Gun*> &others);

	void createBullet(Graphics &graphics, Enemy &enemy, Player &player);

	//return vectors of enemy/guns
	std::vector<Enemy*> getEnemy();
	std::vector<Gun*> getGuns();
	std::vector<Car*>& getCars() { return this->_cars; }

	const Vector2 getPlayerSpawnpoint() const;
	const Vector2 getRazorSpawnPoint() const;
	const Rectangle getHealthRect() const;

	void createGun(Graphics &graphics,Vector2 spawnPoint,Direction direction);

	std::vector<Gun*> Level::getBullet();

	
	//for loading the pause

	void loadPause(Graphics &graphics);
	void updatePause(int elapsedTime, Graphics &graphics);
	void drawPause(Graphics &graphics);

	void loadWin(Graphics &graphics);
	void updateWin(int elapsedTime, Graphics &graphics);
	void drawWin(Graphics &graphics);


private:
	std::string _mapName;
	std::string _bkName;
	Vector2 _spawnPoint;
	Vector2 _spawnPointR;


	
	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope>_slopes;

	std::vector<Rectangle> _fireRects;
	Rectangle _health;
	Rectangle _stairs;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	std::vector<Enemy*> _enemies;
	std::vector<Gun*> _guns;
	std::vector<Gun*> _bullets;
	std::vector<Car*> _cars;

	Box spawnBox;

	//loads a map
	void loadMap(std::string mapName, Graphics &graphics);

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);

	bool topPause;

};

//Tileset Structure

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset() {
		this->FirstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};



#endif