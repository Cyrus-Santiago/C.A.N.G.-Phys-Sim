#include "../include/entityMaestro.hpp"

Maestro::Maestro(entt::entity handle)
    : m_EntityHandle(handle) {};

entt::entity Maestro::createEntity(){
    auto entity = this->registry.create();
    return entity;
};

void Maestro::destroyEntity(entt::entity entity){
    this->registry.destroy(entity);
};

//Allows any number of components to be added
template<typename Text, typename... Args>
Text& Maestro::addComponent(Args&&... args){
    assert(!hasComponent<Text>());
    return this->registry.emplace<Text>(std::forward<Args>(args)...);
};

template<typename Text>
Text&  Maestro::getComponent(){
    assert(hasComponent<Text>());
    return this->registry.get<Text>(m_EntityHandle);
}

template<typename Text>
void Maestro::removeComponent(){
    assert(hasComponent<Text>());
    this->registry.remove<Text>(m_EntityHandle);
};

template<typename Text>
bool Maestro::hasComponent(){
    return this->registry.all_of<Text>(m_EntityHandle);
};

void Maestro::setDimensions(entt::entity entity, float xSize, float ySize){
    this->registry.emplace_or_replace<Dimensions>(entity, xSize, ySize);
};

void Maestro::setPhysics(entt::entity entity, float mass){
    this->registry.emplace_or_replace<Physics>(entity, mass);
};

// This code written by Nate

void Maestro::setRenderable(entt::entity entity, std::string texture,
    glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color) {
    // passes in values for the Renderable component that entity maps to
    this->registry.emplace<Renderable>(entity, position.x,
    position.y, size.x, size.y, rotate, color.x, color.y, color.z, color.w);
};

void Maestro::Draw(entt::entity entity, SpriteRenderer &spriteRenderer) {
    Texture2D texture =
    ResourceManager::GetTexture("button1");
    spriteRenderer.DrawSprite(texture,
    glm::vec2(this->registry.get<Renderable>(entity).xPos,
    this->registry.get<Renderable>(entity).yPos),
    glm::vec2(this->registry.get<Renderable>(entity).xSize,
    this->registry.get<Renderable>(entity).ySize),
    this->registry.get<Renderable>(entity).rotate,
    glm::vec4(this->registry.get<Renderable>(entity).colorR,
              this->registry.get<Renderable>(entity).colorG,
              this->registry.get<Renderable>(entity).colorB,
              this->registry.get<Renderable>(entity).colorA));
}
