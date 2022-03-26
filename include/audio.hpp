/* Audio Class
 * Written by Amethyst Skye and Griffen Agnello */

#ifndef AUDIO_H
#define AUDIO_H

#include "../include/irrKlang/irrKlang.h"
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#pragma comment(lib, "irrKlang.lib") /* link with irrKlang.so */

using namespace irrklang;

class Audio{

public: ISoundEngine* engine;

        Audio();
        int engineStartup();
        int playAudio(const char *);
        int playAudioLoop(const char *);
        void dropAudioEngine();
        int adjustVolume(bool IncOrDec);
        int pauseResumeAudio();

private:
        bool paused;
        std::vector<ISound *> Sounds;
};

#endif
