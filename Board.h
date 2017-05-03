#pragma once
#include "CoOr.h"
#include <SDL.h>
#include <SDL_Image.h>
using namespace std;
class Board
{
public:
	Board();
	~Board();
	char board[3][3];
	void setBoard();
	void getHMove(int x, int y, char player);
	void getOppMove(char AI);
	bool winCon(char currentPlayer);
	CoOr twoOutOfThree(char player);
	CoOr Board::corners();
	bool special(char player, char AI);
	bool drawCon();
};

