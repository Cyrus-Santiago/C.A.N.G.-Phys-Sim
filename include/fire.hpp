#ifndef FIRE
#define FIRE

#include "../include/entt.hpp"
#include "../include/collision.hpp"

bool burn(entt::registry &reg, entt::entity entt, float dt, Collision &colEngine);

#endif