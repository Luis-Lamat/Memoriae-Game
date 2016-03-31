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
    // TODO: paint pattern with getActualSize() + level squares
}

Memoriae::Memoriae() {
	this->cleanBoard();
	level = 0;
	score = 0;
	gameState = STATE_PLAYING;
}

void Memoriae::restart() {
    this->cleanBoard();
    level = 0;
    score = 0;
    gameState = STATE_PLAYING;
}

void Memoriae::pause() {
    gameState = STATE_PAUSED;
}

void Memoriae::unpause() {
    gameState = STATE_PLAYING;
}

void Memoriae::gameOver() {
    gameState = STATE_GAMEOVER;
    printf("Game over :(\n");
}

void Memoriae::changeLevel() {
    if (gameState != STATE_PLAYING) Error::notPlaying();
    this->cleanBoard();
    level++;
}

void Memoriae::selectSphereAt(int row, int col) {
    if (gameState != STATE_PLAYING) Error::notPlaying();
    int mSize = this->getActualSize();
    if (row < 0 || row >= mSize || col < 0 || col >= mSize) {
        Error::outOfBounds(row, col);
    }
    if (board[row][col] == _OFF){
        this->gameOver();
        return;
    }
    score += (level + 1) * 5;
}

