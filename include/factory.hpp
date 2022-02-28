
#include "../include/entt.hpp"
#include "../include/glm/glm.hpp"
#include "../include/spriteRenderer.hpp"
#include "../include/resourceManager.hpp"
#include <glm/fwd.hpp>

struct Renderable {
    //std::string texture;
    int xPos, yPos;
    int xSize, ySize;
    float rotate;
    float colorR, colorG, colorB, colorA;
};

class Factory {
public:
    entt::entity makeParticle(entt::registry &reg, glm::vec2 position =
        glm::vec2(50, 50), glm::vec4 color = glm::vec4(1.0f));

    void drawParticle(entt::registry &reg, entt::entity entity,
        SpriteRenderer &spriteRenderer);
};