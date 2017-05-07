#include "Board.h"
#include "CoOr.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;


Board::Board(){}

Board::~Board(){}


void Board::setBoard() {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			board[i][j] = '_';
		}
	}
}

void Board::getOppMove(char AI) {
	CoOr moveWin = twoOutOfThree(AI);
	char human;
	if (AI == 'X') {
		human = 'O';
	}
	else {
		human = 'X';
	}
	CoOr moveLose = twoOutOfThree(human);
	if ((moveWin.x != -1) && (moveWin.y != -1)) {
		board[moveWin.y][moveWin.x] = AI;
	}
	else if ((moveLose.x != -1) && (moveLose.y != -1)) {
		board[moveLose.y][moveLose.x] = AI;
	}
	else if (board[1][1] == '_') {
		board[1][1] = AI;
	}
	else if ((corners().x != -1) && (corners().y != -1)) {
		if (special(human, AI)) {
			srand(time(0));
			int sub = rand() % 4;
			switch (sub) {
			case 0:
				board[0][1] = AI;
				break;
			case 1:
				board[1][0] = AI;
				break;
			case 2:
				board[1][2] = AI;
				break;
			case 3:
				board[2][1] = AI;
				break;
			}
		}
		else {
			board[corners().y][corners().x] = AI;
		}
	}
	else {
		int oppRow, oppCol;
		srand(time(0));
		oppRow = rand() % 3;
		oppCol = rand() % 3;
		while (board[oppRow][oppCol] != '_') {
			oppRow = rand() % 3;
			oppCol = rand() % 3;
		}
		board[oppRow][oppCol] = AI;
	}
} 

bool Board::winCon(char currentPlayer) {
	int i;
	for (i = 0; i < 3; i++) {
		if ((board[i][0] == currentPlayer) && (board[i][0] == board[i][1]) && (board[i][1] == board[i][2])) {
			return true;
		}
	}
	for (i = 0; i < 3; i++) {
		if ((board[0][i] == currentPlayer) && (board[1][i] == board[0][i]) && (board[1][i] == board[2][i])) {
			return true;
		}
	}
	if ((board[0][0] == currentPlayer) && (board[0][0] == board[1][1]) && (board[1][1] == board[2][2])) {
		return true;
	}
	if ((board[2][0] == currentPlayer) && (board[2][0] == board[1][1]) && (board[1][1] == board[0][2])) {
		return true;
	}
	return false;
}
CoOr Board::twoOutOfThree(char player) {
	CoOr move;
	int i;
	for (i = 0; i < 3; i++) {
		if ((board[i][0] == player) && (board[i][1] == '_') && (board[i][0] == board[i][2])) {
			move.x = 1;
			move.y = i;
			return move;
		}
		if ((board[i][1] == player) && (board[i][0] == '_') && (board[i][1] == board[i][2])) {
			move.x = 0;
			move.y = i;
			return move;
		}
		if ((board[i][0] == player) && (board[i][2] == '_') && (board[i][0] == board[i][1])) {
			move.x = 2;
			move.y = i;
			return move;
		}
	}
	for (i = 0; i < 3; i++) {
		if ((board[0][i] == player) && (board[1][i] == '_') && (board[0][i] == board[2][i])) {
			move.x = i;
			move.y = 1;
			return move;
		}
		if ((board[1][i] == player) && (board[0][i] == '_') && (board[1][i] == board[2][i])) {
			move.x = i;
			move.y = 0;
			return move;
		}
		if ((board[0][i] == player) && (board[2][i] == '_') && (board[0][i] == board[1][i])) {
			move.x = i;
			move.y = 2;
			return move;
		}
	}
	if ((board[0][0] == board[1][1]) && (board[0][0] == player) && (board[2][2] == '_')) {
		move.x = 2;
		move.y = 2;
		return move;
	}
	if ((board[2][2] == board[0][0]) && (board[2][2] == player) && (board[1][1] == '_')) {
		move.x = 1;
		move.y = 1;
		return move;
	}
	if ((board[2][2] == board[1][1]) && (board[2][2] == player) && (board[0][0] == '_')) {
		move.x = 0;
		move.y = 0;
		return move;
	}
	if ((board[0][2] == board[1][1]) && (board[1][1] == player) && (board[2][0] == '_')) {
		move.x = 0;
		move.y = 2;
		return move;
	}
	if ((board[0][2] == board[2][0]) && (board[0][2] == player) && (board[1][1] == '_')) {
		move.x = 1;
		move.y = 1;
		return move;
	}
	if ((board[2][0] == board[1][1]) && (board[1][1] == player) && (board[0][2] == '_')) {
		move.x = 2;
		move.y = 0;
		return move;
	}
	move.x = -1;
	move.y = -1;
	return move;
}
CoOr Board::corners() {
	CoOr move;
	if (board[0][0] == '_') {
		move.x = 0;
		move.y = 0;
		return move;
	}
	else if (board[2][0] == '_') {
		move.x = 0;
		move.y = 2;
		return move;
	}
	else if (board[0][2] == '_') {
		move.x = 2;
		move.y = 0;
		return move;
	}
	else if (board[2][2] == '_') {
		move.x = 2;
		move.y = 2;
		return move;
	}
	else {
		move.x = -1;
		move.y = -1;
		return move;
	}
}

bool Board::special(char player, char AI) {
	if (board[1][1] == AI) {
		if ((board[0][0] == board[2][2]) && (board[2][2] == player)) {
			return true;
		}
		else if ((board[0][2] == board[2][0]) && (board[0][2] == player)) {
			return true;
		}
	}
	return false;
}

bool Board::drawCon() {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (board[i][j] == '_') {
				return false;
			}
		}
	}
	return true;
}

void Board::getHMove(int x, int y, char player) {
	board[x][y] = player;
}
