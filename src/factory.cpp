
#include "../include/factory.hpp"

entt::entity Factory::makeParticle(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, position.x, position.y, 10, 10,
        0.0f, color.x, color.y, color.z, color.w);
    return entity;
}

void Factory::drawParticle(entt::registry &reg, entt::entity entity,
    SpriteRenderer &spriteRenderer) {
    Texture2D texture = ResourceManager::GetTexture("button1");
    spriteRenderer.DrawSprite(texture,glm::vec2(reg.get<Renderable>(entity).xPos,
        reg.get<Renderable>(entity).yPos));
}