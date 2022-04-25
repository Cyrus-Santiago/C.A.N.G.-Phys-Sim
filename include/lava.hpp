#ifndef LAVA
#define LAVA

#include "../include/entt.hpp"
#include "../include/collision.hpp"

class Lava {
public:
    void lavaWaterCollision(entt::registry &reg, entt::entity entt, float dt,
        Collision &colEngine);
    void lavaStoneCollision(entt::registry &reg, entt::entity entt, float dt,
        Collision &colEngine);
};

#endif