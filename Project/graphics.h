#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>
#include<SDL.h>

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();


	/* SDL_Surface* loadImage
	 * loads an image into the _spriteSheets map
	 * each image will be only loaded once
	 * returns the image from map 
	 */
	SDL_Surface* loadImage(const std::string &filePath);


	/* void blitSurface
	 * draws a texture to a certain point on screen
	 */
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectange, SDL_Rect* destinationRectangle);


	/* void flip
	 * renders everything to the screen
	 */
	void flip();


	/* void clear
	 * clears the screen
	 */
	void clear();


	/* SDL_Renderer* getRenderer
	 * returns the renderer
	 */

	SDL_Renderer* getRenderer() const;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	std::map<std::string, SDL_Surface* > _spriteSheets;
};

#endif 