#ifndef LAVA
#define LAVA

#include "../include/entt.hpp"
#include "../include/collision.hpp"

void lavaWaterCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine);
void lavaStoneCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine);
void lavaIceCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine);

#endif