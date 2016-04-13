//
//  Memoriae.hpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/15/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#ifndef Memoriae_hpp
#define Memoriae_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Error.hpp"
using namespace std;

#endif /* Memoriae_hpp */

/* --------------------------- Global Variables ----------------------------- */
# define _ON 1
# define _OFF 0

const int matrixSize = 10;
const int startingMatrixSize = 3;

typedef enum {
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER,
    STATE_WON
} State;

// Class definition
class Memoriae {
	
private:
    // Attributes
	bool board[matrixSize][matrixSize];
	int level;
	int score;
	State gameState;
	
    // Helper functions
	void cleanBoard();
	
public:
    Memoriae();         // Constructor
    void restart();     // Uses shuffle
    void pause();       // Setter for the gameState (pause)
    void unpause();     // Setter for the gameState (play)
    void gameOver();    // Ending the game
    void changeLevel(); // Changes level for the game
    void newBoard();    // Resets board and creates new pattern
    
    int getLevel()      { return level; };
    int getActualSize() { return level + startingMatrixSize; };
    int getScore()      { return score; };
    State getState()    { return gameState; };
    
    // Method to check if an object is ON while painting them
    bool isSet(int row, int col) { return board[row][col]; };
    
    // Method to choose a board object
    void selectSphereAt(int row, int col);
};

