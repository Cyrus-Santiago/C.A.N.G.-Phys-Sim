#include "../include/lava.hpp"
#include <GLFW/glfw3.h>

void lavaWaterCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine) {
    
    // get the renderable component of our entity
    auto enttR = reg.get<Renderable>(entt);

    auto otherEntt = colEngine.entityExists(reg, entt, enttR, IN_PLACE);
    if (!reg.valid(otherEntt))
        otherEntt = colEngine.entityExists(reg, entt, enttR, UP);
    if (reg.valid(otherEntt)) {
        if (reg.any_of<Water>(otherEntt)) {
            auto otherEnttR = reg.get<Renderable>(otherEntt);
            colEngine.entityUnclaim(reg, otherEntt, otherEnttR);
            reg.erase<Liquid>(otherEntt);
            reg.erase<Physics>(otherEntt);
            reg.erase<Water>(otherEntt);
            reg.emplace<Gas>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", enttR.xPos,
                enttR.yPos, 5, 5, 0.0f, 0.9f, 0.9f, 0.9f, 0.6f);
            
            reg.erase<Lava>(entt);
            reg.erase<Liquid>(entt);
            reg.emplace<Stone>(entt);
            reg.replace<Renderable>(entt, "particle", "solid", enttR.xPos,
                enttR.yPos, 5, 5, 0.0f, 0.4f, 0.4f, 0.4f, 1.0f);

        }
    }
}

void lavaStoneCollision(entt::registry &reg, entt::entity entt, float dt,
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
        if (reg.any_of<Stone>(otherEntt) && (((int)glfwGetTime() - (uint) otherEntt) % 4 == 0)) {
            auto otherEnttR = reg.get<Renderable>(otherEntt);
            reg.erase<Stone>(otherEntt);
            reg.emplace<Liquid>(otherEntt);
            reg.emplace<Lava>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", otherEnttR.xPos,
                otherEnttR.yPos, 5, 5, 0.0f, 1.0f, 0.6f, 0.0f, 1.0f);
        }
    }
}

void lavaIceCollision(entt::registry &reg, entt::entity entt, float dt,
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
        if (reg.any_of<Ice>(otherEntt)) {
            auto otherEnttR = reg.get<Renderable>(otherEntt);
            reg.erase<Ice>(otherEntt);
            reg.emplace<Liquid>(otherEntt);
            reg.emplace<Water>(otherEntt);
            reg.replace<Renderable>(otherEntt, "particle", "solid", otherEnttR.xPos,
                otherEnttR.yPos, 5, 5, 0.0f, 0.2f, 0.2f, 1.0f, 1.0f);
        }
    }
}