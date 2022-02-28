
#include "../include/factory.hpp"

entt::entity Factory::makeParticle(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, "button1", position.x, position.y, 10, 10,
        0.0f, color.x, color.y, color.z, color.w);
    return entity;
}

entt::entity Factory::makeShape(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, "skyBackground", position.x, position.y, 30, 30,
        0.0f, color.x, color.y, color.z, color.w);
    return entity;
}

void Factory::draw(entt::registry &reg, entt::entity entity,
    SpriteRenderer &spriteRenderer) {
    Texture2D texture =
    ResourceManager::GetTexture(reg.get<Renderable>(entity).texture);
    spriteRenderer.DrawSprite(
        texture,glm::vec2(reg.get<Renderable>(entity).xPos,
        reg.get<Renderable>(entity).yPos),
        glm::vec2(reg.get<Renderable>(entity).xSize,
        reg.get<Renderable>(entity).ySize), reg.get<Renderable>(entity).rotate,
        glm::vec4(reg.get<Renderable>(entity).colorR,
        reg.get<Renderable>(entity).colorG, reg.get<Renderable>(entity).colorB,
        reg.get<Renderable>(entity).colorA));
}