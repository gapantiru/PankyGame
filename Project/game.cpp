/*Game class
 * Main game loop
 */

#include <SDL.h>
#include <algorithm>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "sound.h"

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game():
mapName("panky2")
{
	this->colide = false;
	this->loop = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	this->menuLoop();
	this->_menu.~Menu();
	//while (this->loop) {
	//	this->gameLoop();
	//	this->_level.clearMem();
	//	this->menuLoop();
	//	this->_menu.~Menu();
	//}
}

Game::~Game() {

}

//_________________________________________MENU LOOP__________________________________________//
void Game::menuLoop() {
	
	//LOOP FOR MENU
	
	Graphics graphics;
	Input input;
	SDL_Event event;

	bool start = false;
	bool start2 = false;
	bool quit = false;
	bool quit2 = false;

	this->_menu =Menu("theMenu", Vector2(100, 100),graphics);



	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true) {

		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				this->loop = false;
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			this->loop = false;
			return;
		}

		if (input.wasKeyPressed(SDL_SCANCODE_UP) == true && this->_menu.getOption()==1) {
			this->_menu.optionM(0);
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_UP) == true && this->_menu.getOption()==2) {
			this->_menu.optionM(1);
		}


		if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true && this->_menu.getOption()==0) {
			this->_menu.optionM(1);
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true && this->_menu.getOption()==1) {
			this->_menu.optionM(2);
		}
		if (input.wasKeyPressed(SDL_SCANCODE_1) == true)
			this->mapName = "panky2";
		else if (input.wasKeyPressed(SDL_SCANCODE_2) == true)
			this->mapName = "map2";
		else if (input.wasKeyPressed(SDL_SCANCODE_3) == true)
			this->mapName = "map3";


		if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
			if (this->_menu.getOption() == 0) {
				//--------------------------------------------------------------------------------------------------------------------------------------//
				this->gameLoop(graphics,this->mapName);
				this->_level.clearMem();
				//--------------------------------------------------------------------------------------------------------------------------------------//
			}
			else if (this->_menu.getOption() == 2) {
				this->loop = false;
				return;
			}
		}

		/*
		//get mouse state for start
		if (event.type == SDL_MOUSEMOTION) {
			if (event.button.x >= this->_menu.getStart().x &&
				event.button.x <= (this->_menu.getStart().x + this->_menu.getStart().w) &&
				event.button.y >= this->_menu.getStart().y &&
				event.button.y <= (this->_menu.getStart().y + this->_menu.getStart().h)) {
				start = true;
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			if (event.button.x < this->_menu.getStart().x ||
				event.button.x > (this->_menu.getStart().x + this->_menu.getStart().w) ||
				event.button.y < this->_menu.getStart().y ||
				event.button.y > (this->_menu.getStart().y + this->_menu.getStart().h)) {
				start = false;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT) {
				start2 = true;
			}
		if (start2 && start) {
			start2 = false;
			start = false;
			quit = false;
			quit2 = false;
			return;
		}





		//get mouse state for quit
		if (event.type == SDL_MOUSEMOTION) {
			if (event.button.x >= this->_menu.getQuit().x &&
				event.button.x <= (this->_menu.getQuit().x + this->_menu.getQuit().w) &&
				event.button.y >= this->_menu.getQuit().y &&
				event.button.y <= (this->_menu.getQuit().y + this->_menu.getQuit().h)) {
				quit = true;
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			if (event.button.x < this->_menu.getQuit().x &&
				event.button.x > (this->_menu.getQuit().x + this->_menu.getQuit().w) &&
				event.button.y < this->_menu.getQuit().y &&
				event.button.y > (this->_menu.getQuit().y + this->_menu.getQuit().h)) {
				quit = false;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT) {
				quit2 = true;
			}
		if (quit2 && quit) {
			this->loop = false;
			return;
		}

		if(event.type==SDL_MOUSEBUTTONUP)
			{
				start2 = false;
				quit2 = false;
				start = false;
				quit = false;
			}

		*/



		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->updateMenu(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->drawMenu(graphics);
	}

}

void Game::drawMenu(Graphics &graphics) {
graphics.clear();
this->_menu.draw(graphics);
graphics.flip();
}

void Game::updateMenu(float elapsedTime, Graphics &graphics) {
	this->_menu.update(elapsedTime, graphics);
}

//__________________________________________________GAME LOOP______________________________________________//

void Game::gameLoop(Graphics &graphics,std::string mapName) {
	Input input;
	SDL_Event event;


	this->_level = Level(mapName, Vector2(100, 100), graphics);

	this->_player = Player(graphics, this->_level.getPlayerSpawnpoint());

	this->_razor = Razor(graphics, this->_level.getRazorSpawnPoint());

	this->_hud = HUD(graphics, this->_player);


	this->_sound = Sound();

	this->_sound.loadMusic();



	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start game loop
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				this->loop = false;
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight();
		}
		if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			this->_player.jump();
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_P) == true) {
			this->_sound.playSound(2);
		}

		//enemy
		if (input.wasKeyPressed(SDL_SCANCODE_E) == true) {
			if (this->_level.getGuns().size() == 0) {
				this->_level.createGun(graphics, Vector2(_player.getX() + 8, _player.getY()),UP);
				int n = this->_level.getEnemy().size();
				this->_sound.sNumber(2);
				if (_level.getEnemy().size() != 0) {
					srand(time(NULL));
					int i = rand() % n + 0;
					this->_level.createBullet(graphics, *_level.getEnemy().at(i), this->_player);
				}
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_Q) == true) {
			if (this->_level.getGuns().size() == 0) {
				this->_level.createGun(graphics, Vector2(_player.getX() + 8, _player.getY()), this->_player.getFacing());
				int n = this->_level.getEnemy().size();
				this->_sound.sNumber(2);
				if (_level.getEnemy().size() != 0) {
					srand(time(NULL));
					int i = rand() % n + 0;
					this->_level.createBullet(graphics, *_level.getEnemy().at(i), this->_player);
				}
			}
		}
		//LOOP TROUGH PAUSE MENU IF PLAYER DIES
		if (this->_player.getCurrentHealth() == 0) {
			this->_level.loadPause(graphics);
			while (true) {
				input.beginNewFrame();
				if (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYUP) {
						input.keyUpEvent(event);
					}
					else if (event.type == SDL_KEYDOWN) {

						input.keyDownEvent(event);
					}
					else if (event.type == SDL_KEYUP) {
						input.keyUpEvent(event);
					}
					else if (event.type == SDL_QUIT) {
						this->loop = false;
						return;
					}
				}
					if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
						return;
					if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true)
						return;


					const int CURRENT_TIME_MS = SDL_GetTicks();
					int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
					LAST_UPDATE_TIME = CURRENT_TIME_MS;

					this->_level.drawPause(graphics);


			}
		}
		// WINNER CONTENT
		else if (this->_level.getEnemy().size() == 0) {
			this->_level.loadWin(graphics);
			while (true) {
				input.beginNewFrame();
				if (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYUP) {
						input.keyUpEvent(event);
					}
					else if (event.type == SDL_KEYDOWN) {

						input.keyDownEvent(event);
					}
					else if (event.type == SDL_KEYUP) {
						input.keyUpEvent(event);
					}
					else if (event.type == SDL_QUIT) {
						this->loop = false;
						return;
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
					return;
				if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true)
					return;


				const int CURRENT_TIME_MS = SDL_GetTicks();
				int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
				LAST_UPDATE_TIME = CURRENT_TIME_MS;

				this->_level.drawWin(graphics);


			}
		}
		//-----------------------------------------//
			const int CURRENT_TIME_MS = SDL_GetTicks();
			int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
			this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
			LAST_UPDATE_TIME = CURRENT_TIME_MS;

			this->_sound.playSound(this->_sound.getNumber());

			this->_sound.sNumber(0);
			this->draw(graphics);


		}
	}


void Game::draw(Graphics &graphics) {
	graphics.clear();

	//draw level before player //
	this->_level.draw(graphics);

	this->_razor.draw(graphics);

	this->_player.draw(graphics);

	this->_hud.draw(graphics);



	graphics.flip();
}

void Game::update(float elapsedTime, Graphics &graphics) {
	this->_player.update(elapsedTime);
	this->_player.handleCarCollisions(this->_level.getCars(), elapsedTime);
	this->_level.update(elapsedTime, this->_player, graphics);
	this->_razor.update(elapsedTime);
	this->_hud.update(elapsedTime,this->_player.getCurrentHealth());

	//check player collision with health box
	if (this->_level.checkHealthCollisions(this->_player) == true &&
		(this->_player.getCurrentHealth() < this->_player.getHealth()) &&
		this->_player.getGain()) {
		this->_player.gainHealth(+1);
		this->_player.doGain(false);
	}
	if (this->_player.getX() < 200)
		this->_player.doGain(true);
	//check collision with stairs
	//if (this->_level.checkStairsCollisions(this->_player) == true) {
	//	this->_player.doStairs(true);
	//	this->_player.moveUp();
	//}
	//else this->_player.doStairs(false);
	//check razor collisions


	std::vector<Rectangle> otherRazor;
	if ((otherRazor = this->_level.checkTileCollisions(this->_razor.getBoundingBox())).size() > 0) {
		this->_razor.handleTileCollisions(otherRazor);
	}
	//check bullet collision with tiles
	this->_level.checkProCollisions();

	//check razor collision with player
	if (this->_level.checkPlayerCollisions(this->_razor, this->_player.getBoundingBox())) {
		this->_razor.touchPlayer(this->_player);
		this->colide = true;
	}

	//check fire collisions
	std::vector<Rectangle> othersFire;
	if ((othersFire = this->_level.checkFireCollisions(this->_player.getBoundingBox())).size() > 0){
		if(this->_player.getCurrentHealth()>0)
		this->_player.gainHealth(-1);
}

	//check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//player colided with at least one tile. handle it
		this->_player.handleTileCollisions(others);
	}
	//check car collisions
		this->_player.handleCarCollisions(this->_level.getCars(),elapsedTime);

	//check enemy collision with tiles
	//and change directions
	std::vector<Rectangle> otherTiles;
	for (int i = 0; i < this->_level.getEnemy().size(); i++) {
		if ((otherTiles = this->_level.checkTileCollisions(this->_level.getEnemy().at(i)->getBoundingBox())).size() > 0) {
			this->_level.getEnemy().at(i)->handleTileCollisions(otherTiles);
		}
	}

	////check slopes
	//std::vector<Slope> otherSlopes;
	//if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
	//	this->_player.handleSlopeCollisions(otherSlopes);
	//}

	//check enemies
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
		//DELETES PROJECTILES
		this->_player.handleEnemyCollsions(otherEnemies);
	}

	//check guns/projectiles
	std::vector<Gun*> otherGuns;
	for (int i = 0; i < _level.getEnemy().size(); i++) {
		if ((otherGuns = this->_level.checkGunCollisions(_level.getEnemy().at(i)->getBoundingBox())).size() > 0) {
			//DROP ENEMIES HEALTH
			_level.getEnemy().at(i)->dropHealth();
			otherGuns.erase(otherGuns.begin(), otherGuns.end());
			this->_sound.sNumber(2);
		}
	}
	std::vector<Gun*> otherBullets;
	for (int i = 0; i < this->_level.getBullet().size(); i++) {
		if (this->_level.checkBulletCollisions(this->_player.getBoundingBox())) {
			this->_player.gainHealth(-1);
			this->_sound.sNumber(3);
		}
	}

	
}