//
//  SoundPlayer.cpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 4/19/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer(){
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Mix_AllocateChannels(8);
    path = this->getAssetsPath();
    
    char clickSoundPath[300];
    sprintf(clickSoundPath,"%s%s", path.c_str() , "floop.wav");
    clickSound = Mix_LoadWAV(clickSoundPath);
    if(clickSound == NULL){
        Error::soundLoadError("floop", Mix_GetError());
    }
    
    char transitionSoundPath[300];
    sprintf(transitionSoundPath,"%s%s", path.c_str() , "chime.wav");
    transitionSound = Mix_LoadWAV(transitionSoundPath);
    if(transitionSound == NULL){
        Error::soundLoadError("chime", Mix_GetError());
    }
    
    music = NULL;
    gameOverSound = NULL;
}

void SoundPlayer::playClickSound(){
    Mix_PlayChannel(2, clickSound, 0);
}

void SoundPlayer::playTransitionSound(){
    Mix_PlayChannel(3, transitionSound, 0);
}

void SoundPlayer::closeSounds(){
    Mix_FreeChunk( clickSound );
    clickSound = NULL;
}

string SoundPlayer::getAssetsPath(){
    if (path.length()) {
        return path;
    }
    // remove "SoundPlayer" from path
    for (int i = (int)soundFilePath.length()-1; i>=0 && soundFilePath[i] != '/'; i--) {
        soundFilePath.erase(i,1);
    }
    path = soundFilePath;
    return path;
}