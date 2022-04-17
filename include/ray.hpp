/* Ray (Extends Simulation Object) - Class Declaration
   Written by Amethyst Skye */

#ifndef RAY_H
#define RAY_H

//#include "../include/simulationObject.hpp"
#include "../include/resourceManager.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

class Ray{
public: glm::vec2 Position, Tail, Size, Velocity, Direction;
        glm::vec4 Color;
        bool Destroyed;
        float Mass, Angle;
        Texture2D Texture;
        /* Default Constructor */
        Ray(glm::vec2 position, glm::vec4 color = {0.9f, 0.9f, 0.9f, 0.5f}, glm::vec2 size = {250,10},
            Texture2D texture = ResourceManager::GetTexture("laser"),
            glm::vec2 velocity = glm::vec2(0.0f),
            float mass = 0, bool Destroyed = 0){};

        /* Methods */
        void printRayStats();
        glm::vec2 setPosition(double, double);
        glm::vec2 setTail(double, double);
        void setSize(glm::vec2, glm::vec2);
        void successfulDraw(int);
        void init(double, double);
        void clear();
        void setDirection();
        Ray incident();
        Ray refractive();
        float rotateLeft();
        float rotateRight();
};
#endif
