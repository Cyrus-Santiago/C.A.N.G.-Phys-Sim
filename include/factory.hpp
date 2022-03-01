
#include "../include/entt.hpp"
#include "../include/glm/glm.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <glm/fwd.hpp>

// component for holding all data needed to render an entity
struct Renderable {
    std::string texture;
    float xPos, yPos;
    int xSize, ySize;
    float rotate;
    float colorR, colorG, colorB, colorA;
};

// component for holding all data needed to enable physics properties for an entity
struct Physics {
    int mass;
    float xVel = 0.00, yVel = 0.00;
    float friction = 0.5;
};

class Factory {
public:
    // used to create a new particle entity
    entt::entity makeParticle(entt::registry &reg, glm::vec2 position =
        glm::vec2(50, 50), glm::vec4 color = glm::vec4(1.0f));

    // used to create a new shape entity
    entt::entity makeShape(entt::registry &reg, glm::vec2 position =
        glm::vec2(50, 50), glm::vec4 color = glm::vec4(1.0f));

    //used to create the borders of the simulation area
    void makeBorder(entt::registry &reg, int scrWidth, int scrHeight, 
        glm::vec4 color = glm::vec4(1.0f));

    /* Used to create a new ray entity */
    entt::entity makeRay(entt::registry &reg, glm::vec2 position =
        glm::vec2(250, 10), glm::vec4 color = glm::vec4(1.0f));

    // used to draw an entity of any type
    void draw(entt::registry &reg, entt::entity entity,
        SpriteRenderer &spriteRenderer);
};
