/* Audio Class Methods
 * Written by Amethyst Skye and Griffen Agnello */

#include "../include/audio.hpp"
#include <string.h>

#pragma comment(lib, "irrKlang.lib") /* link with irrKlang.so */
bool paused=false;

/* Constructor */
    Audio::Audio() {
        engineStartup();
    }

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
        //if(engineStartup() == -1) return -1;
        ISound * sound = engine->play2D(audioClip,false,false,false,ESM_AUTO_DETECT,true);
        Sounds.push_back(sound);
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
        if (!Sounds.empty()) {
            for (ISound * sound : Sounds) {
                sound->drop();
            }
        }
        if (engine) {
            engine->drop();
        }
    }
    //Adjusts the volume based on the pressing '-' or '+'. //TODO does not work yet
    //A "true" (+) passed increments the volume. "false" (-) decrements it.
    int Audio::adjustVolume(bool IncOrDec)   {
        //if(engineStartup() == -1) return -1;
        std::cout<<"yup"<<std::endl;
        ik_f32 volume=engine->getSoundVolume();
        //If true, increase volume
        if(IncOrDec)    {
            if(volume==1)   {
                std::cout<<"Volume already at maximum (1.0)"<<std::endl;
                return 0;
            }
            else    volume+=0.1f;
        }
        else    {
            if(volume==0)   {
                std::cout<<"Volume already at minimum (0.0)"<<std::endl;
                return 0;
            }    
            else    volume-=0.1f;
        }
        engine->setSoundVolume(volume);
        std::cout<<"volume="<<engine->getSoundVolume()<<std::endl;
        engine->update();
        std::cout<<"done"<<std::endl;
        return 0;
    }
    //Pauses all audio when the pressing 'p'. A boolean bit is flipped
    //after pausing or resuming the audio. //TODO does not work yet
    int Audio::pauseResumeAudio() {
        //if(engineStartup() == -1) return -1;
        //If passed true, pause all sounds. If false, resume
        engine->setAllSoundsPaused(paused);
        paused=!paused; //flip paused bit
        std::cout<<"done"<<std::endl;
        return 0;
    }