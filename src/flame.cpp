#include "../include/flame.hpp"
#include "../include/factory.hpp"

void Flame::burn(entt::registry &reg, entt::entity entt, float dt, Collision colEngine) {
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
        reg.destroy(entt);
        return;
    }
    
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