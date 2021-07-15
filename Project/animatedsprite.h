#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <string>
#include <vector>

#include "sprite.h"
#include "globals.h"

class Graphics;

//Animated Sprite class

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);

	/* void playAnimation
	 *plays the information provided
	 */
	void playAnimation(std::string animation, bool once = false);

	/*void update
	 *updates the animated sprite (timer)
	 */
	void update(int elapsedTime);

	/*void draw
	 *drawns the sprite to the screen
	 */
	void draw(Graphics &graphics, int x, int y);

	virtual ~AnimatedSprite() {
		this->_animations.erase(_animations.begin(), _animations.end());
		this->_offsets.erase(_offsets.begin(), _offsets.end());
		Sprite::~Sprite();
	}


protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/* void addAnimation
	 * adds an animation to the map of animations for the sprite
	 */
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/* void resetAnimation
	 * resets all animations associated with this sprite
	 */
	void resetAnimations(); 

	/* void stopAnimation
	 * stops current animation
	 */
	void stopAnimation();

	/* void setVisible
	 * changes the visibility of the animated sprite
	 */
	void setVisible(bool visible);

	/* void animationDone
	 * logic that happnes when an animation ends
	 */
	virtual void animationDone(std::string currentAnimation) = 0;

	/* void setuuAnimation
	* sets up all animations for a sprite
	*/
	virtual void setupAnimations() = 0;


private:
	std::map<std::string, std::vector<SDL_Rect> > _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed = 0.0;
	bool _visible;
};


#endif