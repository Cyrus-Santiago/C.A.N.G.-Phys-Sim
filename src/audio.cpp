/* Audio Class Methods
 * Written by Amethyst Skye */

#include "../include/audio.hpp"

#pragma comment(lib, "irrKlang.lib") /* link with irrKlang.so */

int Audio::playAudio(){
    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine){
        std::cout << "Could not start up audio engine" << std::endl;
        return -1;
    }

    engine->play2D("audio/blip.wav");
    //engine->drop();
    return 0;
}
