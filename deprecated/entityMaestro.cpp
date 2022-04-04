/*
//DEPCRECATED, PLACE IN "SRC" DIRECTORY TO COMPILE
//Code written by Cyrus
#include "../include/entityMaestro.hpp"

Maestro::Maestro(entt::entity handle, Game* game)
    : m_EntityHandle(handle), game(game){};

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
    return this->registry.emplace<Text>(m_EntityHandle, std::forward<Args>(args)...);
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

void Maestro::setDimensions(entt::entity entity, float xPos, float yPos, float xSize, float ySize){
    this->registry.emplace_or_replace<dimensions>(entity, xPos, yPos, xSize, ySize);
};

void Maestro::setPhysics(entt::entity entity, float mass){
    this->registry.emplace_or_replace<physics>(entity, mass);
};
*/