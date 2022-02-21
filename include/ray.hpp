/* Title: Ray - Class Declaration
   Author: Amethyst Skye
 */

#ifndef RAY_H
#define RAY_H

#include "../include/input.hpp"
#include <iostream>

class Ray{
        /* Fields */
public: float rayOrigin[3]; /* origin coordinates */
        float rayEnd[3]; /* end point coordinates */
        bool isActiveFlag = 1; /* idicates if ray is to be used in enviroment */
        /* Constructor */
        Ray(float x, float y, float z){
            rayOrigin[0] = x;
            rayOrigin[1] = y;
            rayOrigin[2] = z;
        }
        /* Methods */
        void printRayCoords();
        void setOrigin(float x, float y, float z);
        void setEnd(float x, float y, float z);
        void deleteRay();
        void placeRay(Click mouseClick);
        std::string isActive();
};
#endif
