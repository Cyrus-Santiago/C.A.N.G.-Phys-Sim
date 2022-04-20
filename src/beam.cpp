/* Beam - Class Methods
   Written by Amethyst Skye
   Description: Handles all operations performed on beam(s) within simulation environment. 
   A beam will be N-rays wide as specified by the user. */

#include "../include/beam.hpp"

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Prints data associated with rays, used for debugging. */
    void Beam::printBeamStats(){
        std::cout <<"Head: (" << beam.Position[0] << ", " << beam.Position[1] << ")" << std::endl;
        std::cout <<"Tail: (" << beam.Tail[0] << ", " << beam.Tail[1] << ")" << std::endl;
        std::cout <<"Dimensions: " << beam.Dimensions[0] << "," << beam.Dimensions[1] << std::endl;
        std::cout <<"Angle: " << beam.Angle << std::endl;
        std::cout <<"Beam Width: " << beamWidth << std::endl;
    }

/* Arguments: none
 * Returns:   N/A
 * Purpose:   Increases the width of a beam by 10 pixels with each click.
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

/* Arguments: x, y coordinates of mouse click
 * Returns:   N/A
 * Purpose:   This will initialize a beam with previously specified beam width.
 *            Handles declaration of head, tail, and dimensions of beam with 
 *            beam being sourced from the top right corner of the play area. */
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
