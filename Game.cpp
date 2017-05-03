#include "Game.h"
#include<iostream>
#include "Board.h"
#include "CoOr.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<cstdlib>
#include<ctime>
using namespace std;

Game::Game(){}


Game::~Game(){}

const int WIDTH = 800, HEIGHT = 600;

SDL_Surface *OptimizedSurface(string filePath, SDL_Surface *windowSurface,SDL_Window *window);


void Game::play() {
	SDL_Window *window = NULL;
	SDL_Surface *windowSurface = NULL;
	SDL_Surface *gameBoard = NULL;
	SDL_Rect playingRect;
	SDL_Surface *playerPiece = NULL;
	SDL_Surface *opposingPiece = NULL;
	Board board;
	char currentPlayer;
	char AI;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "SDL ERROR: " << SDL_GetError() << endl;
	}
	window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	windowSurface = SDL_GetWindowSurface(window);
	if (window == NULL) {
		cout << "Window Error: " << SDL_GetError() << endl;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cout << "Error: " << Mix_GetError() << endl;
	}
	int x = 0, y = 0;
	board.setBoard();
	gameBoard = OptimizedSurface("gameBoard.png", windowSurface, window);
	SDL_BlitScaled(gameBoard, NULL, windowSurface, NULL);
	SDL_UpdateWindowSurface(window);
	playingRect.x = playingRect.y = 0;
	playingRect.w = 3 * gameBoard->w / 16;
	playingRect.h = gameBoard->h / 4;
	int turn = 1;
	int r, c, i, j;
	srand(time(0));
	int track = rand() % 3;
	Mix_Music *background;
	Mix_Chunk *voice;
	switch (track) {
	case 0:
		background = Mix_LoadMUS("background.mp3");
		voice = Mix_LoadWAV("Mako.wav");
		break;
	case 1:
		background = Mix_LoadMUS("background1.mp3");
		voice = Mix_LoadWAV("Pegasus.wav");
		break;
	case 2:
		background = Mix_LoadMUS("background2.mp3");
		voice = Mix_LoadWAV("joey voice.wav");
		break;
	}
	SDL_Surface *win = OptimizedSurface("win.png", windowSurface, window);
	SDL_Surface *lose = OptimizedSurface("lose.png", windowSurface, window);
	SDL_Surface *draw = OptimizedSurface("draw.png", windowSurface, window);
	SDL_Rect winningRect;
	Mix_Chunk *winLine = Mix_LoadWAV("win line.wav");
	Mix_Chunk *drawLine = Mix_LoadWAV("draw line.wav");
	Mix_Chunk *loseLine = Mix_LoadWAV("lose line.wav");
	Mix_Music *winEffect = Mix_LoadMUS("win.mp3");
	winningRect.x = 0;
	winningRect.y = 3 * gameBoard->h / 4;
	winningRect.w = gameBoard->w;
	winningRect.h = gameBoard->h / 4;
	while (true) {
		SDL_Event windowEvent;
		if (SDL_PollEvent(&windowEvent)) {
			if (SDL_QUIT == windowEvent.type) {
				break;
			}
		}
		if (SDL_WaitEvent(&windowEvent) == 0) {
			continue;
		}
		if (windowEvent.type == SDL_KEYDOWN) {
			switch (windowEvent.key.keysym.sym) {
			case SDLK_x:
				currentPlayer = 'X';
				AI = 'O';
				playerPiece = OptimizedSurface("X.png", windowSurface, window);
				opposingPiece = OptimizedSurface("O.png", windowSurface, window);
				break;
			case SDLK_o:
				currentPlayer = 'O';
				AI = 'X';
				playerPiece = OptimizedSurface("O.png", windowSurface, window);
				opposingPiece = OptimizedSurface("X.png", windowSurface, window);
				break;
			}
		}
		if (!Mix_PlayingMusic()) {
			Mix_PlayMusic(background, -1);
		}
		if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
			if (windowEvent.button.button == SDL_BUTTON_LEFT) {
				x = windowEvent.button.x;
				y = windowEvent.button.y;
				if ((0 <= x) && (x < playingRect.w)) {
					c = 0;
				}
				else if (x < 2 * playingRect.w) {
					c = 1;
				}
				else {
					c = 2;
				}
				if ((0 <= y) && (y < playingRect.h)) {
					r = 0;
				}
				else if (y < 2 * playingRect.h) {
					r = 1;
				}
				else {
					r = 2;
				}
				if (turn == 1) {
					board.getHMove(r, c, currentPlayer);
					if (board.winCon(currentPlayer)) {
						Mix_HaltMusic();
						for (i = 0; i < 3; i++) {
							for (j = 0; j < 3; j++) {
								if (board.board[i][j] == currentPlayer) {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(playerPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
								else if (board.board[i][j] != '_') {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(opposingPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
							}
						}
						SDL_BlitScaled(win, NULL, windowSurface, &winningRect);
						SDL_UpdateWindowSurface(window);
						SDL_Delay(2000);
						Mix_PlayMusic(winEffect, 0);
						SDL_Delay(2000);
						Mix_PlayChannel(-1, loseLine, 0);
						return;
					}
					if (board.drawCon()) {
						Mix_HaltMusic();
						for (i = 0; i < 3; i++) {
							for (j = 0; j < 3; j++) {
								if (board.board[i][j] == currentPlayer) {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(playerPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
								else if (board.board[i][j] != '_') {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(opposingPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
							}
						}
						SDL_BlitScaled(draw, NULL, windowSurface, &winningRect);
						SDL_UpdateWindowSurface(window);
						Mix_PlayMusic(winEffect, 0);
						SDL_Delay(2000);
						Mix_PlayChannel(-1, drawLine, 0);
						return;
					}
					turn = 2;
				}
				if (turn == 2) {
					if (Mix_PlayingMusic()) {
						Mix_PauseMusic();
					}
					Mix_PlayChannel(-1, voice, 0);
					board.getOppMove(AI);
					if (Mix_PausedMusic()) {
						Mix_ResumeMusic();
					}
					if (board.winCon(AI)) {
						Mix_HaltMusic();
						for (i = 0; i < 3; i++) {
							for (j = 0; j < 3; j++) {
								if (board.board[i][j] == currentPlayer) {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(playerPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
								else if (board.board[i][j] != '_') {
									playingRect.x = j * playingRect.w;
									playingRect.y = i * playingRect.h;
									SDL_BlitScaled(opposingPiece, NULL, windowSurface, &playingRect);
									SDL_UpdateWindowSurface(window);
									playingRect.x = playingRect.y = 0;
								}
							}
						}
						SDL_BlitScaled(lose, NULL, windowSurface, &winningRect);
						SDL_UpdateWindowSurface(window);
						SDL_Delay(2000);
						Mix_PlayChannel(-1, winLine, 0);
						return;
					}
					turn = 1;
				}
			}
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (board.board[i][j] == currentPlayer) {
						playingRect.x = j * playingRect.w;
						playingRect.y = i * playingRect.h;
						SDL_BlitScaled(playerPiece, NULL, windowSurface, &playingRect);
						SDL_UpdateWindowSurface(window);
						playingRect.x = playingRect.y = 0;
					}
					else if (board.board[i][j] != '_') {
						playingRect.x = j * playingRect.w;
						playingRect.y = i * playingRect.h;
						SDL_BlitScaled(opposingPiece, NULL, windowSurface, &playingRect);
						SDL_UpdateWindowSurface(window);
						playingRect.x = playingRect.y = 0;
					}
				}
			}
		}
	}
	SDL_FreeSurface(windowSurface);
	SDL_FreeSurface(gameBoard);
	SDL_FreeSurface(playerPiece);
	SDL_FreeSurface(opposingPiece);
	SDL_DestroyWindow(window);
	window = NULL;
	windowSurface = NULL;
	gameBoard = NULL;
	playerPiece = NULL;
	opposingPiece = NULL;
	IMG_Quit();
	SDL_Quit();
}
SDL_Surface *OptimizedSurface(string filePath, SDL_Surface *windowSurface, SDL_Window *window){
	SDL_Surface *optimizedSurface = NULL;
	SDL_Surface *imageSurface = IMG_Load(filePath.c_str());
	windowSurface = SDL_GetWindowSurface(window);
	if (imageSurface == NULL) {
		cout << "Load image Error: " << IMG_GetError() << endl;
	}
	else {
		optimizedSurface = SDL_ConvertSurface(imageSurface, windowSurface->format, NULL);
		if (optimizedSurface == NULL) {
			cout << "Optimizing error: " << SDL_GetError() << endl;
		}
	}
	SDL_FreeSurface(imageSurface);
	return optimizedSurface;
}
