/* Audio Class Methods
 * Written by Amethyst Skye and Griffen Agnello */

#include "../include/audio.hpp"

#pragma comment(lib, "irrKlang.lib") /* link with irrKlang.so */
bool paused=false;

/* Constructor */
    Audio::Audio() {
        engineStartup();
    }

/* Arguments: none
 * Returns:   int
 * Purpose:   Initializes an audio engine using the irrKlang audio engine. 
 *            Returns -1 upon failure, and 0 upon success. */
    int Audio::engineStartup(){
        engine = createIrrKlangDevice();
        if (!engine){
            std::cout << "Could not start up audio engine" << std::endl;
            return -1;
        }
        return 0;
    }

/* Arguments: const char * audioClip
 * Returns:   int
 * Purpose:   Using the audio engine, plays an audio clip in 2D
 *            for a single cycle. Returns 0 upon success. */
    int Audio::playAudio(const char *audioClip){
        ISound * sound = engine->play2D(audioClip,false,false,false,ESM_AUTO_DETECT,true);
        Sounds.push_back(sound);
        return 0;
    }

/* Arguments: const char * audioClip
 * Returns:   int
 * Purpose:   Using the audio engine, plays an audio clip in 2D
 *            in a loop until paused or program is terminated. 
 *            Returns 0 upon success. */
    int Audio::playAudioLoop(const char *audioClip){
        ISound * sound = engine->play2D(audioClip,true,false,false,ESM_AUTO_DETECT,true);
        Sounds.push_back(sound);
        return 0;
    }

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Removes the audio engine from use. We usually use this
 *            upon termination of the program to free memory. */
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
        ik_f32 volume=engine->getSoundVolume();
        //If true, increase volume
        if(IncOrDec)    {
            if(volume>=1.0f)   {
                return 1;
            }
            else    volume+=0.1f;
        }
        else    {
            if(volume<=0.0f)   {
                return 2;
            }    
            else    volume-=0.1f;
        }
        engine->setSoundVolume(volume);
        //std::cout<<"volume="<<engine->getSoundVolume()<<std::endl;
        engine->update();
        return 0;
    }
    //Pauses all audio when the pressing 'p'. A boolean bit is flipped
    //after pausing or resuming the audio. //TODO does not work yet
    int Audio::pauseResumeAudio() {
        //if(engineStartup() == -1) return -1;
        //If passed true, pause all sounds. If false, resume
        engine->setAllSoundsPaused(paused);
        paused=!paused; //flip paused bit
        engine->update();
        return 0;
    }