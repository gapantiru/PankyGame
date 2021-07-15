/* Main.cpp
 *
 */

#include <SDL.h>
#include "game.h"

//hide console
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/STACK:1")
#pragma comment(linker, "/HEAP:1")

int main(int argc, char* argv[]){
	Game game;

	return 0;
}