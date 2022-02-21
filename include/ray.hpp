/* Title: Ray - Class Declaration
   Author: Amethyst Skye
 */

#ifndef RAY_H
#define RAY_H

#include "../include/input.hpp"
#include "../include/spriteRenderer.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Ray{
        /* Fields */
public: glm::vec2 Origin, End, Size;
        glm::vec3 Color;
        //Texture2D rayTexture;
        bool isActive;

        /* Constructor */
        Ray(double x, double y){
            Origin[0] = x;
            Origin[1] = y;
        }
        /* Methods */
        void printRayCoords();
        glm::vec2 setOrigin(double, double);
        glm::vec2 setEnd(double, double);
        glm::vec2 setSize(glm::vec2, glm::vec2);
        void deleteRay();
        void placeRay(Click);
        int drawRay();
};
#endif
