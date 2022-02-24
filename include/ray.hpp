/* Ray (Extends Simulation Object) - Class Declaration
   Written by Amethyst Skye */

#ifndef RAY_H
#define RAY_H

#include "../include/simulationObject.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Ray : public SimulationObject{
public: glm::vec2 Position, Tail, Size, Velocity;
        glm::vec4 Color;
        bool Destroyed;
        float Mass;
        Texture2D Texture;

        /* Default Constructor */
        Ray(glm::vec2 position, glm::vec4 color = {0.9f, 0.9f, 0.9f, 0.5f}, glm::vec2 size = {500,10},
            Texture2D texture = ResourceManager::GetTexture("laser"),
            glm::vec2 velocity = glm::vec2(0.0f),
            float mass = 0, bool Destroyed = 0) :

            SimulationObject(position, color, size,
            texture, velocity, mass, Destroyed) {
                Position = position;
                Color = color;
                Size = size;
            }

        /* Methods */
        void printRayCoords();
        glm::vec2 setPosition(double, double);
        glm::vec2 setTail(double, double);
        void setSize(glm::vec2, glm::vec2);
        void successfulDraw(int);
        void init();
};
#endif
