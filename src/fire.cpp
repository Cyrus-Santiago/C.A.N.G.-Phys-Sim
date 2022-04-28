#include "../include/fire.hpp"
#include "../include/factory.hpp"

bool burn(entt::registry &reg, entt::entity entt, float dt, Collision &colEngine) {
    // get the renderable component of our entity
    auto enttR = reg.get<Renderable>(entt);
    // choose a direction to move randomly even is right, odd is left
    int direction = ((uint)entt << 1) % 19;
    direction = (direction % 2 == 0) ? direction : direction * -1;

    reg.patch<Renderable>(entt, [dt, direction](auto &renderable) {
        renderable.xPos += dt * 3 * direction;
        renderable.yPos -= dt * 100;
        renderable.colorA -= dt;
    });
    if (reg.get<Renderable>(entt).colorA <= 0) {
        if (reg.valid(entt))
            reg.destroy(entt);
        return true;
    }
    
    enttR = reg.get<Renderable>(entt);
    auto otherEntt = colEngine.entityExists(reg, entt, enttR, IN_PLACE, true);

    if (reg.valid(otherEntt)) {
        colEngine.entityUnclaim(reg, otherEntt, reg.get<Renderable>(otherEntt));

        if (reg.any_of<Water>(otherEntt)) {
            reg.erase<Liquid>(otherEntt);
            reg.erase<Water>(otherEntt);
            reg.erase<Physics>(otherEntt);
            reg.emplace<Gas>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", enttR.xPos, enttR.yPos,
                5, 5, 0.0f, 0.9f, 0.9f, 0.9f, 0.6f);
        }
        if(reg.any_of<Flammable>(otherEntt)){
            if(!reg.any_of<Animated>(otherEntt)){
                reg.emplace<Animated>(otherEntt,4.0f,0.0f);
            }
            colEngine.entityClaim(reg, otherEntt, reg.get<Renderable>(otherEntt));
        }
        if (reg.valid(entt) && (!reg.all_of<Flammable>(entt)))
            reg.destroy(entt);
        return true;
    }
    return false;
}
