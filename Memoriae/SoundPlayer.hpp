//
//  SoundPlayer.hpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 4/19/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#ifndef SoundPlayer_hpp
#define SoundPlayer_hpp

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "Error.hpp"
using namespace std;

#endif /* SoundPlayer_hpp */

class SoundPlayer {
    
    string path = "";
    string soundFilePath = __FILE__;
    Mix_Music *music;
    Mix_Chunk *clickSound;
    Mix_Chunk *wrongSound;
    Mix_Chunk *transitionSound;
    Mix_Chunk *gameOverSound;
    
public:
    
    SoundPlayer();
    void playClickSound();
    void playTransitionSound();
    void playGameOverSound();
    void playBadClickSound();
    void closeSounds();
    string getAssetsPath();
    
};