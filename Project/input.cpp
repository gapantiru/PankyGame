/* Input Class
 * Keeps track of keyboard state
 */


#include "input.h"

//reset keys each frame
void Input::beginNewFrame() {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

//key press
void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

//key release
void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}

//certain key was pressed during curent frame
bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];	
}

//certain key was released during curent frame
bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

//certain key is held during curent frame
bool Input::isKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}
