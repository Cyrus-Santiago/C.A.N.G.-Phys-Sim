/* Beam - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on beam(s) within simulation environment. A beam will be composed of N light rays are specified by the user. */

#include "../include/beam.hpp"

/* Beam will add an additional ray for each click while in beam mode 
 * ~ Depreciated ~ */
    void Beam::incBeamWidth(){
        if (beamWidth < 3){ /* Maximum of 3 rays in width */
        beamWidth++;
        }
        else beamWidth = 1;
    }

/* Remove all elements in rays array 
 * ~ Depreciated ~ */
    void Beam::clear(){
        for(int i = 0; i < 5; i++)
            rays[i].clear(); /* Ensure all other rays are cleared from array */
    }

/* Beam is initially one light ray in width */
    void Beam::init(double xPos, double yPos){
        beam.setPosition((float)xPos, (float)yPos);
        beam.setTail(805, 45);
        beam.setBeamDimensions(beam.Position, beam.Tail, beamWidth);
        beam.setDirection();
        beam.determineBeamOffset();
        beam.Texture = ResourceManager::GetTexture("laser");
        beam.Velocity = {0, 0};
        beam.Destroyed = false;
    }

/* ~ Depreciated ~ */
    void Beam::setBeamDirection(){
        for(int i = 0; i < beamWidth; i++)
            rays[i].setDirection();
    }
