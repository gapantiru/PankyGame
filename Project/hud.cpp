#include"hud.h"

#include"graphics.h"


HUD::HUD(){}

HUD::HUD(Graphics &graphics, Player &player) {
	this->_player = player;
	this->_healthBarSprite = Sprite(graphics, "char/textBox.png", 0, 40, 16, 8, 0, 0);
	this->_healthNumber1 = Sprite(graphics, "char/textBox.png", 0, 56, 8, 8, 17, 0);
}

void HUD::update(int elapsedTime,int health) {
	this->_healthNumber1.setSourceRectX(8 * health);
}

void HUD::draw(Graphics &graphics) {
	this->_healthBarSprite.draw(graphics, this->_healthBarSprite.getX(), this->_healthBarSprite.getY());
	this->_healthNumber1.draw(graphics, this->_healthNumber1.getX(), this->_healthNumber1.getY());

}