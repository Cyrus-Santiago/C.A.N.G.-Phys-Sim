#ifndef LAVA
#define LAVA

#include "../include/entt.hpp"
#include "../include/collision.hpp"

class Lava {
public:
    bool lavaWaterCollision(entt::registry &reg, entt::entity entt, float dt, Collision colEngine);
};

#endif