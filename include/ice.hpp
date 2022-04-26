#ifndef ICE
#define ICE

#include "../include/entt.hpp"
#include "../include/collision.hpp"

void iceWaterCollision(entt::registry &reg, entt::entity entt, float dt,
    Collision &colEngine);

#endif