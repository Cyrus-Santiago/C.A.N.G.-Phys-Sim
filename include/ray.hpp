/* Ray - Class Declaration
   Written by Amethyst Skye */

#ifndef RAY_H
#define RAY_H

#include "../include/resourceManager.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

class Ray{
    /* Fields */
public: glm::vec2 Position, Tail, Offset, Dimensions, Velocity, Direction;
        glm::vec4 Color;
        bool Destroyed;
        bool offsetFlag;
        float Mass, Angle;
        Texture2D Texture;

    /* Constructor */
        Ray(glm::vec2 position = {45,45}, glm::vec4 color = {0.9f, 0.9f, 0.9f, 0.5f}, glm::vec2 size = {250,10},
            Texture2D texture = ResourceManager::GetTexture("laser"),
            glm::vec2 velocity = glm::vec2(0.0f),
            float mass = 0, bool Destroyed = 0){};

    /* Methods */
        void printRayStats();
        void setPosition(double, double);
        void setTail(double, double);
        void setDimensions(glm::vec2, glm::vec2);
        void setBeamDimensions(glm::vec2, glm::vec2, int);
        void determineOffset();
        void determineBeamOffset();
        void successfulDraw(bool);
        void init(double, double);
        void clear();
        void setDirection();
        Ray incident();
        Ray refractive();
        float rotateLeft();
        float rotateRight();
};
#endif
