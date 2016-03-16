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

const int matrixSize = 10;

typedef enum {
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER,
} State;

// Class definition
class Memoriae {
    
private:
    int board[matrixSize][matrixSize];
    int level;
    int score;
    State gameState;
    
public:
    Memoriae();      // Constructor
    void start();    // Uses shuffle
    void pause();    // Setter for the gameState
    void getState(); // Getter for the gameState
    void selectObjectAt(int row, int col); // Game mechanic
};