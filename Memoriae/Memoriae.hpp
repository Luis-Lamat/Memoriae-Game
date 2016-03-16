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
} State;

// Class definition
class Memoriae {
    
private:
    // Attributes
    int board[matrixSize][matrixSize];
    int level;
    int score;
    State gameState;
    
    // Helper functions
    void cleanBoard();
    
public:
    Memoriae();      // Constructor
    void start();    // Uses shuffle
    void pause();    // Setter for the gameState
    void getLevel(); // Getter for the level
    void selectObjectAt(int row, int col); // Game mechanic
    
    int getActualSize() { return level + startingMatrixSize; };
    
    State getState();// Getter for the gameState
};