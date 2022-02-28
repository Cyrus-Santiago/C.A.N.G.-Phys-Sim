#ifndef ENTITYMAESTRO_H
#define ENTITYMAESTRO_H

#include "../include/entt.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/game.hpp"

class Game;

class Maestro{
public:
    //constructors
    Maestro() = default;
    Maestro(entt::entity handle, Game* game);
    Maestro(const Maestro& other) = default;

    //create and destroy entities
    entt::entity createEntity();
    void destroyEntity(entt::entity entity);
    
    //add or remove components to an entity
    template<typename Text, typename... Args>
    Text& addComponent(Args&&... args);


    template<typename Text>
    Text& getComponent();

    template<typename Text>
    void removeComponent();

    template<typename Text>
    bool hasComponent();

    //Modify Specific Components
    void setDimensions(entt::entity entity, float xPos, float yPos, float xSize, float ySize);
    void setPhysics(entt::entity entity, float mass);
    //void setRenderable(entt::entity entity, Texture2D &texture);    

    private:
        entt::entity m_EntityHandle{0};
        Game* game = nullptr;
};

#endif