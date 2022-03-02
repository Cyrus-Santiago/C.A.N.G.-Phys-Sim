
#include "../include/factory.hpp"

entt::entity Factory::makeParticle(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    // call on the registry for a new entity ID
    auto entity = reg.create();
    
    // insert data passed to method into renderable component of entity
    reg.emplace<Renderable>(entity, "button1", position.x, position.y, 10, 10,
        0.0f, color.x, color.y, color.z, color.w);

    // give the particle a mass of 10 and turn on physics, setting it's initial height
    reg.emplace<Physics>(entity, 10, position.y+10);

    return entity;
}

entt::entity Factory::makeShape(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, "button1", position.x, position.y, 30, 30,
        0.0f, color.x, color.y, color.z, color.w);

    reg.emplace<Physics>(entity, 10, position.y+30);
    return entity;
}

entt::entity Factory::makeRay(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    auto entity = reg.create();

    reg.emplace<Renderable>(entity, "laser", position.x, position.y, 250, 10,
        0.0f, color.x, color.y, color.z, color.w);
    return entity;
}

void Factory::makeBorder(entt::registry &reg, int scrWidth, int scrHeight, glm::vec4 color){
    int areaWidth=scrWidth*0.9, areaHeight=scrHeight*0.4;
    int xPos=scrWidth*0.05, yPos=scrHeight*0.05;
    auto entity1 = reg.create(); //Top Line
    reg.emplace<Renderable>(entity1, "button1", xPos+1, yPos, 
        areaWidth-2, 3, 0.0f, color.x, color.y, color.z, color.w);
    auto entity2 = reg.create(); //Bottom Line
    reg.emplace<Renderable>(entity2, "button1", xPos+1, yPos+areaHeight, 
        areaWidth-2, 3, 0.0f, color.x, color.y, color.z, color.w);
    auto entity3 = reg.create(); //Left Line
    reg.emplace<Renderable>(entity3, "button1", xPos-1, yPos, 
        3, areaHeight+3, 0.0f, color.x, color.y, color.z, color.w);
    auto entity4 = reg.create(); //Right Line
    reg.emplace<Renderable>(entity4, "button1", (xPos+areaWidth)-1, yPos, 
        3, areaHeight+3, 0.0f, color.x, color.y, color.z, color.w);
    return;
};

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
