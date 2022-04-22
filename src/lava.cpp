#include "../include/lava.hpp"

bool Lava::lavaWaterCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision colEngine) {
    
    if (!reg.any_of<Magma>(entt)) return false;

    // get the renderable component of our entity
    auto enttR = reg.get<Renderable>(entt);
    
    
    
    enttR = reg.get<Renderable>(entt);

    auto otherEntt = colEngine.entityAtLoc((int)enttR.xPos, (int)enttR.yPos);
    if (reg.valid(otherEntt)) {
        enttR = reg.get<Renderable>(otherEntt);
        colEngine.destroyEnttAtLoc(reg, (int)enttR.xPos, (int)enttR.yPos);
        if (reg.any_of<Liquid>(otherEntt)) {
            reg.erase<Liquid>(otherEntt);
            reg.erase<Physics>(otherEntt);
            reg.emplace<Gas>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", enttR.xPos, enttR.yPos,
                5, 5, 0.0f, 0.9f, 0.9f, 0.9f, 0.6f);
            colEngine.registerEntity(reg, otherEntt);
        }
        reg.destroy(entt);
    }

}