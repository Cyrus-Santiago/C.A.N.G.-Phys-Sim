#ifndef FACTORY
#define FACTORY

#include "../include/entt.hpp"
#include "../include/glm/glm.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <glm/fwd.hpp>
#include <string>

// component for holding all data needed to render an entity
struct Renderable {
    std::string type;
    std::string texture;
    float xPos, yPos;
    int xSize, ySize;
    float rotate;
    float colorR, colorG, colorB, colorA;
};

// component for holding all data needed to enable physics properties for an entity
struct Physics {
    float mass;
    float xVel = 0.00, yVel = 0.00;
    float xAccel = 0.00, yAccel = 0.00;
    float friction = 0.5;
};

// component used for explosions
struct Forcewave{
    float xVel, yVel;
};

// component used for explosions and for the triangle shape
struct Triangle{
    std::vector<glm::vec2> points;  //Vertice positions of a triangle
};

struct Border {
    std::string position;
};

struct Liquid {
    float viscosity = 1.0f;
};

struct Lava {};

struct Stone {};

struct Water {};

struct Ice {};

struct Animated{
    float maxTime;
    float dR;  //Rate of change for resizing
    float timeActive=0.0f;
};

struct Fire {};

struct Gas {};

struct Flammable {};
struct Reflective{};

struct Shape{};

struct Light{};

class Factory {
public:
    // used to create a new particle entity
    entt::entity makeParticle(entt::registry &reg, std::string type, glm::vec2 position =
        glm::vec2(1, 1), glm::vec4 color = glm::vec4(1.0f));

    // used to create a new shape entity
    entt::entity makeShape(entt::registry &reg, glm::vec2 position =
        glm::vec2(50, 50), glm::vec4 color = glm::vec4(1.0f), glm::vec2 dimensions=
        glm::vec2(30), std::string type="shape");

    //used to create the borders of the simulation area
    void makeBorder(entt::registry &reg, int scrWidth, int scrHeight, 
        glm::vec4 color = glm::vec4(1.0f));

    /* Used to create a new ray entity */
    entt::entity makeRay(entt::registry &reg, glm::vec2 position =
        glm::vec2(250, 10), glm::vec4 color = glm::vec4(1.0f));

    /* Used to create a new beam entity */
    entt::entity makeBeam(entt::registry &reg, glm::vec2 position =
        glm::vec2(250, 10), glm::vec4 color = glm::vec4(1.0f));
    //Creates a force vector entity
    entt::entity makeForceVector(entt:: registry &reg, glm::vec2 position =
        glm::vec2(1, 1), float rotation=0.0f, glm::vec4 color = glm::vec4(1.0f),
        glm::vec2 velocity = glm::vec2(100.0f));

    entt::entity makeAnimation(entt::registry &reg, glm::vec2 position =
        glm::vec2(1,1), glm::vec4 color = glm::vec4(1.0f),
        glm::vec2 size= glm::vec2(30.0f), std::string texture="button1", 
        std::string type="NO_NAME", float maxTime=1.0f, float dR=0.0f);

    // used to draw an entity of any type
    void draw(entt::registry &reg, entt::entity entity,
        SpriteRenderer &spriteRenderer);
};

#endif
