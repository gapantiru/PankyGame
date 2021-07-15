#ifndef CAR_H
#define CAR_H

#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"



class Graphics;

class Car : public Sprite {
public:
	Car();
	Car(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void moveLeft();
	void moveRight();

	void handleTileCollisions(std::vector<Rectangle> &others);

	const float getX() const;
	const float getY() const;

	Direction getDirection(){ return this->direction; }



private:
	float _dx, _dy;

	Direction _facing;

	Direction direction;

};


#endif // !CAR_H
