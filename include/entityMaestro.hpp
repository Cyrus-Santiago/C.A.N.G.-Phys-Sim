#ifndef ENTITYMAESTRO_H
#define ENTITYMAESTRO_H

#include "../include/entt.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/game.hpp"

#include "../include/glm/glm.hpp"
#include "../include/texture.hpp"
//Contains everything for physics calculations
typedef struct{
    float mass = 1.00; //Default to 1 kg (Are we using kg or lbs?)
    float area;
    float xVel;
    float yVel;
}physics;

typedef struct{
    float xPos = 50;
    float yPos = 50;
    float xSize = 10;
    float ySize = 10;
}dimensions;
//Can change based on what we want to store
    
typedef struct{
    Texture2D &texture;
    glm::vec2 position;
    glm::vec2 size = glm::vec2(10.f,10.0f);
    float rotate = 0.0f;
    glm::vec4 color = glm::vec4(1.0f);
}renderable;

struct TagComponent{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag){}
};

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
        entt::registry registry;
};

#endif