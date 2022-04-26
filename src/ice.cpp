#include "../include/ice.hpp"
#include <GLFW/glfw3.h>

void iceWaterCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine) {

    auto enttR = reg.get<Renderable>(entt);

    auto otherEntt = colEngine.entityExists(reg, entt, enttR, IN_PLACE);
    if (!reg.valid(otherEntt))
        otherEntt = colEngine.entityExists(reg, entt, enttR, UP);
    if (!reg.valid(otherEntt))
        otherEntt = colEngine.entityExists(reg, entt, enttR, LEFT);
    if (!reg.valid(otherEntt))
        otherEntt = colEngine.entityExists(reg, entt, enttR, RIGHT);
    if (!reg.valid(otherEntt))
        otherEntt = colEngine.entityExists(reg, entt, enttR, DOWN);

    if (reg.valid(otherEntt)) {
        if (reg.any_of<Water>(otherEntt) && (((int)glfwGetTime() - (uint) otherEntt) % 4 == 0)) {
            auto otherEnttR = reg.get<Renderable>(otherEntt);
            reg.erase<Water>(otherEntt);
            reg.erase<Liquid>(otherEntt);
            reg.emplace<Ice>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", otherEnttR.xPos,
                otherEnttR.yPos, 5, 5, 0.0f, 0.8f, 0.8f, 1.0f, 0.6f);
        }
    }

}