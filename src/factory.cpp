
#include "../include/factory.hpp"
#include "../include/ray.hpp"
#include "../include/beam.hpp"
#include <glm/fwd.hpp>
#include <string>

entt::entity Factory::makeParticle(entt::registry &reg, std::string type, glm::vec2 position,
    glm::vec4 color) {
    // call on the registry for a new entity ID
    auto entity = reg.create();

    if (type == "WATER")
        reg.emplace<Liquid>(entity, 0.1f, 0.0f);
        
    reg.emplace<Physics>(entity, 10.0f);
    
    // insert data passed to method into renderable component of entity
    reg.emplace<Renderable>(entity, "particle", "button1", position.x, position.y, 5, 5,
        0.0f, color.x, color.y, color.z, color.w);

    return entity;
}

entt::entity Factory::makeShape(entt::registry &reg, glm::vec2 position,
    glm::vec4 color, glm::vec2 dimensions, std::string type) {
    auto entity = reg.create();
    std::string shapeTexture;
    //Add the triangle component if the shape is a triangle
    if(type=="TRIANGLE")    {
        //The triangle points are in this particular order in the vector. The order DOES matter
        //Vertices: Left, right, top
        //Midpoints: Left, right, bottom, middle (of triangle)
        std::vector<glm::vec2> trianglePoints={ glm::vec2(position.x,position.y), glm::vec2(position.x+20,position.y),
            glm::vec2(position.x+10,position.y+20), glm::vec2(position.x+5,position.y+10), glm::vec2(position.x+15,position.y+10),
            glm::vec2(position.x+10,position.y), glm::vec2(position.x+10,position.y+10)};
        reg.emplace<Triangle>(entity, trianglePoints);
        shapeTexture="triangle";
    }
    else    shapeTexture="button2";
    reg.emplace<Renderable>(entity,type, shapeTexture, position.x, position.y, 
        (int)dimensions.x, (int)dimensions.y, 0.0f, color.x, color.y, color.z, color.w);
    //For gravity and velocity
    reg.emplace<Physics>(entity, 30.0f);
    return entity;
}

entt::entity Factory::makeRay(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    Ray ray({position.x, position.y});
    ray.init(position.x, position.y);
    auto entity = reg.create();

    reg.emplace<Renderable>(entity, "ray", "laser", position.x, position.y, (int)ray.Size[0], (int)ray.Size[1],
        0.0f, color.x, color.y, color.z, color.w);
    ray.printRayStats();
    return entity;
}

entt::entity Factory::makeBeam(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    Beam beam({position.x, position.y});
    auto entity = reg.create();

    reg.emplace<Renderable>(entity, "beam", "laser", position.x, position.y, (int)beam.rays[0].Size[0], (int)beam.rays[0].Size[1],
        0.0f, color.x, color.y, color.z, color.w);
    return entity;
}

entt::entity Factory::makeForceVector(entt::registry &reg, glm::vec2 position, 
    float rotation,glm::vec4 color, glm::vec2 velocity) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, "force", "triangle", position.x, position.y, 20, 20,
        rotation, color.x, color.y, color.z, color.w);
    reg.emplace<Forcewave>(entity, velocity.x, velocity.y);
    //The triangle points are in this particular order in the vector. The order DOES matter
    //Vertices: Left, right, top
    //Midpoints: Left, right, bottom, middle (of triangle)
    std::vector<glm::vec2> trianglePoints={ glm::vec2(position.x,position.y), glm::vec2(position.x+20,position.y),
        glm::vec2(position.x+10,position.y+20), glm::vec2(position.x+5,position.y+10), glm::vec2(position.x+15,position.y+10),
        glm::vec2(position.x+10,position.y), glm::vec2(position.x+10,position.y+10)};
    reg.emplace<Triangle>(entity, trianglePoints);
    return entity;
}
void Factory::makeBorder(entt::registry &reg, int scrWidth, int scrHeight, glm::vec4 color){
    int areaWidth=scrWidth*0.9, areaHeight=scrHeight*0.4;
    int xPos=scrWidth*0.05, yPos=scrHeight*0.05;

    auto entity1 = reg.create(); //Top Line
    reg.emplace<Renderable>(entity1, "topBorder", "button1", (float)xPos+1, (float)yPos, 
        areaWidth-2, 3, 0.0f, color.x, color.y, color.z, color.w);
    reg.emplace<Border>(entity1, "topBorder");

    auto entity2 = reg.create(); //Bottom Line
    reg.emplace<Renderable>(entity2, "bottomBorder", "button1", (float)xPos+1, (float)yPos+areaHeight, 
        areaWidth-2, 3, 0.0f, color.x, color.y, color.z, color.w);
    reg.emplace<Border>(entity2, "bottomBorder");

    auto entity3 = reg.create(); //Left Line
    reg.emplace<Renderable>(entity3, "leftBorder", "button1", (float)xPos-1, (float)yPos, 
        3, areaHeight+3, 0.0f, color.x, color.y, color.z, color.w);
    reg.emplace<Border>(entity3, "leftBorder");

    auto entity4 = reg.create(); //Right Line
    reg.emplace<Renderable>(entity4, "rightBorder", "button1", (float)(xPos+areaWidth)-1, (float)yPos, 
        3, areaHeight+3, 0.0f, color.x, color.y, color.z, color.w);
    reg.emplace<Border>(entity4, "rightBorder");

    
    std::cout << (xPos + areaWidth - 1) << std::endl;
    std::cout << (yPos + areaHeight) << std::endl;

    return;
};

void Factory::draw(entt::registry &reg, entt::entity entity,
    SpriteRenderer &spriteRenderer) {
    
    Texture2D texture;

    // pass the specified texture string from the entity and provide it to the
    // resource manager to get our texture
    if (reg.get<Renderable>(entity).texture != "") {
        texture =
        ResourceManager::GetTexture(reg.get<Renderable>(entity).texture);
    } else {
        texture.ID = -1;
    }

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
