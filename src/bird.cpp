#include "../include/bird.hpp"
#include "../include/factory.hpp"
#include <cmath>

// configuration settings
#define VISUAL_RANGE 75 // how far a bird can see
#define MIN_DISTANCE 10 // if another bird is within this distance, move away
#define AVOID_FACTOR 0.05 // scales velocity birds avoid eachother at
#define CENTERING_FACTOR 0.005 // scales how fast birds move to flock center
#define MARGIN 43 // play area margin in pixels
#define TURN_FACTOR 15 // how fast a bird should turn to avoid edge
#define WIDTH 763 // width of play area
#define HEIGHT 344 // height of play area
#define SPEED_LIMIT 200 // max speed a bird can go
#define MATCHING_FACTOR 0.1 // how fast birds match velocity of fellow birds

/* Arguments: entity registry, bird entity from main col loop, delta frame
 * Returns:   N/A
 * Purpose:   calls all functions that adjust velocity of birds, mimics movement
 *            of a flock. */
void birdLoop(entt::registry &reg, entt::entity birdA, float dt) {
    pointTorwardsCenter(reg, birdA, dt);
    avoidOthers(reg, birdA, dt);
    matchVelocity(reg, birdA, dt);
    limitSpeed(reg, birdA, dt);
    keepWithinBounds(reg, birdA, dt);

    // apply velocity to position of birds
    auto birdAP = reg.get<Physics>(birdA);
    reg.patch<Renderable>(birdA, [dt, birdAP](auto &renderable) {
        renderable.xPos += birdAP.xVel * dt;
        renderable.yPos += birdAP.yVel * dt;
    });
}

/* Arguments: entity registry, entity of birdA, delta frame
 * Returns:   N/A
 * Purpose:   Points birds to the center of the flock so they stay together */
void pointTorwardsCenter(entt::registry &reg, entt::entity birdA, float dt) {

    // initialize variables
    int centerX = 0, centerY = 0, neighborCount = 0;

    // get renderable component of birdA
    auto birdAR = reg.get<Renderable>(birdA);

    // get physical component of birdA
    auto birdAP = reg.get<Physics>(birdA);

    // get a view of all birds in registry
    auto birdView = reg.view<Bird>();
    
    // loop through all birds in registry
    for (auto birdB : birdView) {

        // get renderable component of birdB
        auto birdBR = reg.get<Renderable>(birdB);

        // get physical component of birdB
        auto birdBP = reg.get<Physics>(birdB);

        // if bird A and bird B are close enough to see each other
        if (distance(birdA, birdB, reg) < VISUAL_RANGE) {

            // adjust center based on birdB
            centerX += reg.get<Renderable>(birdB).xPos;
            centerY += reg.get<Renderable>(birdB).yPos;

            // increase bird count
            neighborCount += 1;
        }

        // if birdA has a neighbor
        if (neighborCount > 0) {

            // we divide center by number of birds
            centerX = centerX / neighborCount;
            centerY = centerY / neighborCount;

            // we adjust velocity so each bird moves towards center
            reg.patch<Physics>(birdA, [dt, birdAR, centerX, centerY](auto &physics) {
                physics.xVel += (centerX - birdAR.xPos) * dt * CENTERING_FACTOR;
                physics.yVel += (centerY - birdAR.yPos) * dt * CENTERING_FACTOR;
            });
        }
    }
}

/* Arguments: entity registry, birdA entity, delta frame
 * Returns:   N/A
 * Purpose:   Keeps birds from clumping together, and makes them spread out a bit */
void avoidOthers(entt::registry &reg, entt::entity birdA, float dt) {

    // initialize values for how much the bird should move and get it's renderable
    // component
    int moveX = 0, moveY = 0;
    auto birdAR = reg.get<Renderable>(birdA);

    // loop through each entity with Bird component
    auto birdView = reg.view<Bird>();
    for (auto birdB : birdView) {

        // get renderable component of birdB
        auto birdBR = reg.get<Renderable>(birdB);

        // a bird shouldn't avoid itself
        if (birdA == birdB) continue;

        // if distance is less than min set distance, we separate birds
        if (distance(birdA, birdB, reg) < MIN_DISTANCE) {
            moveX += birdAR.xPos - birdBR.xPos;
            moveY += birdAR.yPos - birdBR.yPos;
        }
    }

    // apply changes ot velocity
    reg.patch<Physics>(birdA, [dt, moveX, moveY](auto &physics) {
        physics.xVel += moveX * dt * AVOID_FACTOR;
        physics.yVel += moveY * dt * AVOID_FACTOR;
    });
}

/* Arguments: entity registry, birdA entity, delta frame
 * Returns:   N/A
 * Purpose:   keeps the birds within the play area, accelerating them away from
 *            edge when they pash a threshold. */
void keepWithinBounds(entt::registry &reg, entt::entity birdA, float dt) {

    // get renderable and physical components of birdA
    auto birdAR = reg.get<Renderable>(birdA);
    auto birdAP = reg.get<Physics>(birdA);

    // if bird passes left border, add to velocity
    if (birdAR.xPos < MARGIN + 25) {
        reg.patch<Physics>(birdA, [](auto &physics) {
            physics.xVel += TURN_FACTOR;
        });
    }

    // if bird passes right border, subtract from velocity
    if (birdAR.xPos > WIDTH + MARGIN - 25) {
        reg.patch<Physics>(birdA, [](auto &physics) {
            physics.xVel -= TURN_FACTOR;
        });
    }

    // if bird passes top border, add to velocity
    if (birdAR.yPos < MARGIN + 25) {
        reg.patch<Physics>(birdA, [](auto &physics) {
            physics.yVel += TURN_FACTOR;
        });
    }

    // if bird passes bottom border, subtract from velocity
    if (birdAR.yPos > HEIGHT + MARGIN - 25) {
        reg.patch<Physics>(birdA, [](auto &physics) {
            physics.yVel -= TURN_FACTOR;
        });
    }
}

/* Arguments: entity registry, birdA entity, delta frame
 * Returns:   N/A
 * Purpose:   keeps birds moving at a similar speed. */
void matchVelocity(entt::registry &reg, entt::entity birdA, float dt) {

    // intialized variables
    int avgXVel = 0, avgYVel = 0, neighborCount = 0;

    // get renderabel and physical components of birdA
    auto birdAR = reg.get<Renderable>(birdA);
    auto birdAP = reg.get<Physics>(birdA);

    // loop through all entities with Bird component
    auto birdView = reg.view<Bird>();
    for (auto birdB : birdView) {
        
        // get renderable and physical components of birdB
        auto birdBR = reg.get<Renderable>(birdB);
        auto birdBP = reg.get<Physics>(birdB);
        
        // if birds can see each other
        if (distance(birdA, birdB, reg) < VISUAL_RANGE) {
            // add their velocity to avg and increase bird count
            avgXVel += birdBP.xVel;
            avgYVel += birdBP.yVel;
            neighborCount += 1;
        }
    }

    if (neighborCount > 0) {
        // calc avg of velocities
        avgXVel = avgXVel / neighborCount;
        avgYVel = avgYVel / neighborCount;

        // adjust velocities according to avg
        reg.patch<Physics>(birdA, [avgXVel, avgYVel, dt](auto &physics) {
            physics.xVel += (avgXVel - physics.xVel) * dt * MATCHING_FACTOR;
            physics.yVel += (avgYVel - physics.yVel) * dt * MATCHING_FACTOR;
        });
    }
}

/* Arguments: entity registry, birdA entity, delta frame
 * Returns:   N/A
 * Purpose:   limits the max speed a bird can move */
void limitSpeed(entt::registry &reg, entt::entity birdA, float dt) {

    // get renderable and physical component of birdA
    auto birdAR = reg.get<Renderable>(birdA);
    auto birdAP = reg.get<Physics>(birdA);
    
    // set speed to the magnitude of the speed vector
    int speed = sqrt(birdAP.xVel * birdAP.xVel + birdAP.yVel * birdAP.yVel);

    // if it's over speed limit, adjust
    if (speed > SPEED_LIMIT) {
        reg.patch<Physics>(birdA, [speed, dt](auto &physics) {
            physics.xVel = (physics.xVel / speed) * SPEED_LIMIT * dt;
            physics.yVel = (physics.yVel / speed) * SPEED_LIMIT * dt;
        });
    }
}

/* Arguments: birdA entity, birdB entity, entity registry
 * Returns:   an integer than represents distance between the birds
 * Purpose:   find the distance between two birds */
int distance(entt::entity birdA, entt::entity birdB, entt::registry &reg) {

    // get renderable components of birdA and birdB
    auto birdAR = reg.get<Renderable>(birdA);
    auto birdBR = reg.get<Renderable>(birdB);

    // return magnitude of the line between the two birds
    return sqrt((birdAR.xPos - birdBR.xPos) * (birdAR.xPos - birdBR.xPos) +
                (birdAR.yPos - birdBR.yPos) * (birdAR.yPos - birdBR.yPos));
}