#include <iostream>
#include "Board.h"
#include "Game.h"
#include "CoOr.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
using namespace std;
int main(int argc, char* argv[]) {
	Game game;
	game.play();
	system("PAUSE");
	return 0;
}
