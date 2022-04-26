#include "../include/factory.hpp"
#include "../include/ray.hpp"
#include "../include/beam.hpp"
#include <glm/fwd.hpp>
#include <string>

entt::entity Factory::makeParticle(entt::registry &reg, std::string type, glm::vec2 position,
    glm::vec4 color) {
    // call on the registry for a new entity ID
    auto entity = reg.create();
    int modifierX = 0, modifierY = 0;

    if (type == "WATER") {
        reg.emplace<Liquid>(entity);
        reg.emplace<Water>(entity);
        reg.emplace<Physics>(entity, 10.0f);
    } else if (type == "ICE") {
        reg.emplace<Ice>(entity);
        reg.emplace<Physics>(entity, 10.0f);
    } else if (type == "FIRE") {
        reg.emplace<Fire>(entity);
    } else if (type == "STEAM") {
        reg.emplace<Gas>(entity);
        modifierX += (uint)entity % 8 - 4;
        modifierY -= (uint)entity % 8 - 4;
    } else if (type == "LAVA") {
        reg.emplace<Liquid>(entity, 2.0f);
        reg.emplace<Lava>(entity);
        reg.emplace<Physics>(entity, 10.0f);
    } else if (type == "STONE") {
        reg.emplace<Stone>(entity);
        reg.emplace<Physics>(entity, 10.0f);
    } else {
        reg.emplace<Physics>(entity, 10.0f);
    }
    
    // insert data passed to method into renderable component of entity
    reg.emplace<Renderable>(entity, "particle", "solid", position.x + modifierX,
    position.y + modifierY, 5, 5, 0.0f, color.x, color.y, color.z, color.w);

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
        std::vector<glm::vec2> trianglePoints={ glm::vec2(position.x-20,position.y+20), glm::vec2(position.x+20,position.y+20), 
            glm::vec2(position.x,position.y-20), glm::vec2(position.x-10,position.y), glm::vec2(position.x+10,position.y), 
            glm::vec2(position.x,position.y+20), glm::vec2(position.x,position.y), 
        };
        reg.emplace<Triangle>(entity, trianglePoints);
        shapeTexture="triangle";
    }
    else    shapeTexture="button2";
    reg.emplace<Renderable>(entity,type, shapeTexture, position.x-20, position.y-20, 
        (int)dimensions.x, (int)dimensions.y, 0.0f, color.x, color.y, color.z, color.w);
    //For gravity and velocity
    reg.emplace<Physics>(entity, 30.0f);
    reg.emplace<Shape>(entity);
    return entity;
}

/* Arguments: entity registry, position vector, color vector
 * Returns:   entity (Ray)
 * Purpose:   Responsible for drawing a Ray to the screen given mouse
              click data. All Rays originate from the top right corner
              of the play area. */
entt::entity Factory::makeRay(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    Ray ray;
    ray.init(position.x, position.y);
    auto entity = reg.create();
    float angle = (float)(ray.Angle * (180/M_PI));   
    float posX = ray.Position[0] + ray.Offset[0];
    float posY = ray.Position[1] + ray.Offset[1];
    reg.emplace<Renderable>(entity, "ray", "solid", posX, posY, (int)ray.Dimensions[0], (int)ray.Dimensions[1],
        angle, color.x, color.y, color.z, color.w);
    reg.emplace<Light>(entity);
    return entity;
}

/* Arguments: entity registry, position vector, color vector
 * Returns:   entity (Beam)
 * Purpose:   Responsible for drawing a Beam to the screen given mouse
              click data. All Beams originate from the top right corner
              of the play area. Beams are 3 Rays in width. */
entt::entity Factory::makeBeam(entt::registry &reg, glm::vec2 position,
    glm::vec4 color) {
    Beam myBeam;
    myBeam.incBeamWidth(); //2 wide
    myBeam.incBeamWidth(); //3 wide
    myBeam.init(position.x, position.y);
    auto entity = reg.create();
    float angle = (float)(myBeam.beam.Angle * (180/M_PI));   
    float posX = myBeam.beam.Position[0] + myBeam.beam.Offset[0];
    float posY = myBeam.beam.Position[1] + myBeam.beam.Offset[1];
    float dimX = myBeam.beam.Dimensions[0];
    float dimY = myBeam.beam.Dimensions[1];
    reg.emplace<Renderable>(entity, "beam", "solid", posX, posY, (int)dimX, (int)dimY,
        angle, color.x, color.y, color.z, color.w);
    reg.emplace<Light>(entity);
    return entity;
}

entt::entity Factory::makeForceVector(entt::registry &reg, glm::vec2 position, 
    float rotation,glm::vec4 color, glm::vec2 velocity) {
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, "force", "triangle", position.x, position.y, 20, 20,
        rotation, color.x, color.y, color.z, color.w);
    reg.emplace<Forcewave>(entity, velocity.x, velocity.y);
    reg.emplace<Animated>(entity, 3.5f,0.0f);
    //The triangle points are in this particular order in the vector. The order DOES matter
    //Vertices: Left, right, top
    //Midpoints: Left, right, bottom, middle (of triangle)
    std::vector<glm::vec2> trianglePoints={ glm::vec2(position.x-20,position.y+20), glm::vec2(position.x+20,position.y+20), 
        glm::vec2(position.x,position.y-20), glm::vec2(position.x-10,position.y), glm::vec2(position.x+10,position.y), 
        glm::vec2(position.x,position.y+20), glm::vec2(position.x,position.y), 
    };
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

    return;
};

entt::entity Factory::makeAnimation(entt::registry &reg, glm::vec2 position, 
    glm::vec4 color, glm::vec2 size, std::string texture, std::string type, float maxTime, float dR){
    auto entity = reg.create();
    reg.emplace<Renderable>(entity, type, texture, position.x,
        position.y, (int)size.x, (int)size.y, 0.0f, color.x, color.y, color.z, color.w);
    reg.emplace<Animated>(entity,maxTime,dR);
    return entity;
}

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