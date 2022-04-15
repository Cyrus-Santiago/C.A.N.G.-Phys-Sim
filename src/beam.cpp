/* Beam - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on beam(s) within simulation environment. A beam will be composed of N light rays are specified by the user. */

#include "../include/beam.hpp"

/* Beam will add an additional ray for each click while in beam mode */
    void Beam::incBeamWidth(double xPos, double yPos){
        if (beamWidth < 5){ /* Maximum of 5 rays in width */
        beamWidth++;
        rays[beamWidth - 1].init(xPos, yPos);
        /* Increase the origin position of next ray by the width of one ray */
        rays[beamWidth - 1].Position[1]+= 10;
        rays[beamWidth - 1].Tail[1] += 10;
        rays[beamWidth - 1].setDimensions(rays[beamWidth - 1].Position, rays[beamWidth - 1].Tail);
        }
        else Beam::init(xPos, yPos);
    }

/* Remove all elements in rays array */
    void Beam::clear(){
        for(int i = 0; i < 5; i++)
            rays[i].clear(); /* Ensure all other rays are cleared from array */
    }

/* Beam is initially one light ray in width */
    void Beam::init(double xPos, double yPos){
        Beam::clear();
        beamWidth = 1;
        rays[0].init(xPos, yPos);
    }

    void Beam::setBeamDirection(){
        for(int i = 0; i < beamWidth; i++)
            rays[i].setDirection();
    }
