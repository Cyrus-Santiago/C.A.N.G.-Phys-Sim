#include "../include/entityMaestro.hpp"
#include "../include/components.hpp"

Maestro::Maestro(entt::entity handle, Game* game)
    : m_EntityHandle(handle), game(game){};

entt::entity Maestro::createEntity(){
    auto entity = game->registry.create();
    return entity;
};

void Maestro::destroyEntity(entt::entity entity){
    game->registry.destroy(entity);
};

//Allows any number of components to be added
template<typename Text, typename... Args>
Text& Maestro::addComponent(Args&&... args){
    assert(!hasComponent<Text>());
    return game->registry.emplace<Text>(m_EntityHandle, std::forward<Args>(args)...);
};

template<typename Text>
Text&  Maestro::getComponent(){
    assert(hasComponent<Text>());
    return game->registry.get<Text>(m_EntityHandle);
}

template<typename Text>
void Maestro::removeComponent(){
    assert(hasComponent<Text>());
    game->registry.remove<Text>(m_EntityHandle);
};
template<typename Text>
bool Maestro::hasComponent(){
    return game->registry.all_of<Text>(m_EntityHandle);
};

void Maestro::setDimensions(entt::entity entity, float xPos, float yPos, float xSize, float ySize){
    game->registry.emplace_or_replace<dimensions>(entity, xPos, yPos, xSize, ySize);
};

void Maestro::setPhysics(entt::entity entity, float mass){
    game->registry.emplace_or_replace<physics>(entity, mass);
};