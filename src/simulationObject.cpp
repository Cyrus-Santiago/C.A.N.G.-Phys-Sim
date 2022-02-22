// this code was written by Nate

#define GRAVITY 9.71

#include "../include/simulationObject.hpp"

// constructor, self explanatory
SimulationObject::SimulationObject(glm::vec2 position, glm::vec3 color,
                     glm::vec2 size, Texture2D texture, glm::vec2 velocity,
                     bool destroyed) :
                     Position(position), Size(size), Color(color),
                     Texture(texture), Velocity(velocity), Destroyed(destroyed) { }

void SimulationObject::Draw(SpriteRenderer &spriteRenderer) {
    // we call draw sprite with the intrinsic information of the object
    spriteRenderer.DrawSprite(this->Texture, this->Position,
    this->Size, 0.0f, this->Color);
}

void SimulationObject::Move(float dt) {
    // here we move the object based on it's velocity, and based on gravity
    // dt is the delta frame, which is the time since the last frame was
    // rendered
    this->Position += this->Velocity * dt;
    this->Position.y += GRAVITY * dt * 5;
}