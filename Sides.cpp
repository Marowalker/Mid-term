#include "Sides.h"
#include<iostream>
#include <SDL.h>
#include<SDL_image.h>
using namespace std;
Sides::Sides(){}
Sides::~Sides(){}
char Sides::choose() {
	cout << "choose your side: ";
	char choice;
	cin >> choice;
	while ((choice != 'X') && (choice != 'O')) {
		cout << "Wrong input. Try again: ";
		cin >> choice;
	}
	return choice;
}
char Sides::oppChoose(char choice) {
	if (choice == 'X') {
		return 'O';
	}
	else {
		return 'X';
	}
}
