/* Beam - Class Declaration
   Written by Amethyst Skye */

#ifndef BEAM_H
#define BEAM_H

#include "../include/ray.hpp"

class Beam{
    /* Fields */
public: int beamWidth = 0;
        Ray rays[];

    /* Constructor */
    Beam(){}

    /* Methods */
    void incBeamWidth();
    void clear();
    void init();
    void setBeamDirection();
};
#endif
