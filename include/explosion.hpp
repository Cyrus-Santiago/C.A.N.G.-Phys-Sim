#include "entt.hpp"
#include <glm/fwd.hpp>
#include <vector>
class Explosion{
    public:
//----------Fields------------------
        static float velocityArrayX[];
        static float velocityArrayY[];
        static float rotation[];
//----------Methods-----------------
        static void updateForcePositions(entt::registry *reg, float dt);
        static void removeForceVector(entt::registry *reg, entt:: entity entity);
};
