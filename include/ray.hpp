/* Ray - Class Declaration
   Written by Amethyst Skye */

#ifndef RAY_H
#define RAY_H

#include "../include/input.hpp"
#include "../include/simulationObject.hpp"
#include "../include/simulation.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Ray{
        /* Fields */
public: glm::vec2 Origin, End, Size;
        glm::vec3 Color;
        Texture2D rayTexture;
        bool isActive;

        /* Constructor */
        Ray(){}
        /* Methods */
        void printRayCoords();
        glm::vec2 setOrigin(double, double);
        glm::vec2 setEnd(double, double);
        glm::vec2 setSize(glm::vec2, glm::vec2);
        void deleteRay();
        void successfulDraw(int);
        void placeRay(Click);
        void drawRay(SpriteRenderer &renderer);
};
#endif
