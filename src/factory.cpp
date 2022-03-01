
#include "../include/factory.hpp"

entt::entity Factory::makeParticle(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    // call on the registry for a new entity ID
    auto entity = reg.create();
    
    // insert data passed to method into renderable component of entity
    reg.emplace<Renderable>(entity, "button1", position.x, position.y, 10, 10,
        0.0f, color.x, color.y, color.z, color.w);

    // give the particle a mass of 10 and turn on physics
    reg.emplace<Physics>(entity, 10);

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
    
    // pass the specified texture string from the entity and provide it to the
    // resource manager to get our texture
    Texture2D texture =
    ResourceManager::GetTexture(reg.get<Renderable>(entity).texture);

    // call upon the spriteRenderer to render the entity based on it's
    // renderable component
    spriteRenderer.DrawSprite(
        // texture
        texture,
        
        // position
        glm::vec2(reg.get<Renderable>(entity).xPos,
        reg.get<Renderable>(entity).yPos),

        // size
        glm::vec2(reg.get<Renderable>(entity).xSize,
        reg.get<Renderable>(entity).ySize),
        
        // rotation
        reg.get<Renderable>(entity).rotate,

        // color
        glm::vec4(reg.get<Renderable>(entity).colorR,
        reg.get<Renderable>(entity).colorG, reg.get<Renderable>(entity).colorB,
        reg.get<Renderable>(entity).colorA));
}