//
//  Memoriae.cpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/15/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#include "Memoriae.hpp"

void Memoriae::cleanBoard() {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			board[i][j] = _OFF;
		}
	}
}

Memoriae::Memoriae() {
	this->cleanBoard();
	level = 0;
	score = 0;
	gameState = STATE_PLAYING;
}

void Memoriae::start() {
	this->cleanBoard();
}

void Memoriae::pause() {
	this->gameState = STATE_PAUSED;
}

State Memoriae::getState() {
	return this->gameState;
}

void Memoriae::selectObjectAt(int row, int col) {
	int mSize = this->getActualSize();
	if (row < 0 || row >= mSize || col < 0 || col >= mSize) {
		printf("Error: matrix selection index out of bounds\n");
		return;
	}
	board[row][col] = _ON;
}