//
//  Error.hpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/31/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#ifndef Error_hpp
#define Error_hpp

#include <stdio.h>
#include <stdlib.h>
using namespace std;

#endif /* Error_hpp */

class Error {
    
public:
    static void outOfBounds(int r, int c){
        printf("Error: index [%d, %d] is out of bounds\n", r, c);
        exit(EXIT_FAILURE);
    };
    
    static void notPlaying(){
        printf("Error: game is not int the PLAY state\n");
    };
    
    static void soundLoadError(string sound, const char *error){
        printf("Failed to load %s sound! SDL_mixer Error: %s\n", sound.c_str(), error);
    };
};