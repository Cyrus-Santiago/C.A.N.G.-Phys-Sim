
// this code was written by Nate

#include "../include/simulationObject.hpp"

SimulationObject::SimulationObject(glm::vec2 position, glm::vec3 color,
                     glm::vec2 size, Texture2D texture, glm::vec2 velocity,
                     bool destroyed) :
                     Position(position), Size(size), Color(color),
                     Texture(texture), Velocity(velocity), Destroyed(destroyed) { }

void SimulationObject::Draw(SpriteRenderer &spriteRenderer) {
    spriteRenderer.DrawSprite(this->Texture, this->Position,
    this->Size, 0.0f, this->Color);
}