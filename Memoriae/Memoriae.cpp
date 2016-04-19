//
//  Memoriae.cpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/15/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#include "Memoriae.hpp"

int myrandom (int i) { return rand()%i; }

void Memoriae::newBoard() {
    
    int realSize = this->getActualSize();
    printf("Size: %d", realSize);
    int count = realSize + (level * 2);
    int it = 0;
    
    // Getting the random generated solution
    vector<bool> v (realSize * realSize, false);
    for (int i = 0; i < count; i++) { v[i] = true; }
    random_shuffle(v.begin(), v.end(), myrandom);
    
    printf("\nSolution Matrix:");
    for (int i = 0; i < realSize; i++) {
        printf("\n");
        for (int j = 0; j < realSize; j++) {
            if (v[it++]) {
                board[i][j] = _ON;
                count--;
            } else {
                board[i][j] = _OFF;
            }
            printf("%d ", board[i][j]);
        }
    }
    printf("\n");
}

Memoriae::Memoriae() {
    // seeding the random numbers
    srand ( unsigned (time(0)) );
    
    // constructing
	this->newBoard();
	level = 0;
	score = 0;
    tries = 0;
    subLevel = 0;
	gameState = STATE_PLAYING;
}

void Memoriae::restart() {
    this->newBoard();
    level = 0;
    score = 0;
    tries = 0;
    subLevel = 0;
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
    tries = 0;
    if (subLevel < 2) {
        subLevel++;
    } else {
        level++;
        subLevel = 0;
    }
    this->newBoard();
    printf("Level changed to: %d/%d\n", subLevel, level);
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
    tries++;
    if (tries == this->getActualSize() + (level * 2)) {
        this->changeLevel();
    }
}

