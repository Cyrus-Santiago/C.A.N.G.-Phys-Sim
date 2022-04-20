/* Beam - Class Declaration
   Written by Amethyst Skye */

#ifndef BEAM_H
#define BEAM_H

#include "../include/ray.hpp"
#include <vector>

class Beam{
    /* Fields */
public: int beamWidth = 1;
        std::vector<Ray> rays; /* Depreciated */
        Ray beam;

    /* Constructor */
    Beam(){};

    /* Methods */
    void printBeamStats();
    void incBeamWidth();
    void clear();
    void init(double, double);
    void setBeamDirection();
};
#endif
