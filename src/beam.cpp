/* Beam - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on beam(s) within simulation environment. A beam will be composed of however many light rays are specified by the user. */

#include "../include/beam.hpp"

/* Beam will add an additional ray for each click while in beam mode */
    void Beam::setBeamWidth(){
        beamWidth++;
    }
/* Beam is initially one light ray in width */
    void Beam::init(){
        beamWidth = 1;
    }
