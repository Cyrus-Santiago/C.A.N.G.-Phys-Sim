#ifndef FLAME
#define FLAME

#include "../include/entt.hpp"
#include "../include/collision.hpp"

class Flame {
public:
    void burn(entt::registry &reg, entt::entity entt, float dt, Collision colEngine);
};

#endif