/* Audio Class Methods
 * Written by Amethyst Skye */

#include "../include/audio.hpp"
#include <string.h>

#pragma comment(lib, "irrKlang.lib") /* link with irrKlang.so */

/* Initializes audio engine */
    int Audio::engineStartup(){
        engine = createIrrKlangDevice();
        if (!engine){
            std::cout << "Could not start up audio engine" << std::endl;
            return -1;
        }
        return 0;
    }

/* Will play audio for one cycle */
    int Audio::playAudio(const char *audioClip){
        if(engineStartup() == -1) return -1;
        engine->play2D(audioClip);
        return 0;
    }

/* Plays audio in a loop - need to work on this more (DON'T USE THIS YET) */
    int Audio::playAudioLoop(const char *audioClip){
    if(engineStartup() == -1) return -1;
        while(true)
            engine->play2D(audioClip);
    return 0;
    }

/* Removes audio engine from use */
    void Audio::dropAudioEngine(){
        engine->drop();
    }
