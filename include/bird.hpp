// implemented using boids algorithm
// references: https://github.com/beneater/boids,
//             https://www.youtube.com/watch?v=4LWmRuB-uNU,
//             https://en.wikipedia.org/wiki/Flocking_(behavior)

#ifndef BIRD
#define BIRD

#include "../include/entt.hpp"
#include "../include/collision.hpp"

void birdLoop(entt::registry &reg, entt::entity birdA, float dt);

void pointTorwardsCenter(entt::registry &reg, entt::entity birdA, float dt);
void avoidOthers(entt::registry &reg, entt::entity birdA, float dt);
void matchVelocity(entt::registry &reg, entt::entity birdA, float dt);
void limitSpeed(entt::registry &reg, entt::entity birdA, float dt);
void keepWithinBounds(entt::registry &reg, entt::entity birdA, float dt);

int distance(entt::entity birdA, entt::entity birdB, entt::registry &reg);

#endif