/* Beam - Class Declaration
   Written by Amethyst Skye */

#ifndef BEAM_H
#define BEAM_H

#include "../include/ray.hpp"
#include <vector>

class Beam{
    /* Fields */
public: int beamWidth = 0;
        std::vector<Ray> rays; /* Depreciated */
        Ray beam;

    /* Constructor */
    Beam(double xPos, double yPos){
        rays[0].init(xPos, yPos);
    }

    /* Methods */
    void incBeamWidth(double, double);
    void clear();
    void init(double, double);
    void setBeamDirection();
};
#endif
