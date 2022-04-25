#include "entt.hpp"
class Animation{
public:
    void animationUpdate(entt::registry &reg, float dt);
    void updateTimeActive(entt::registry &reg, entt::entity entt, float dt);
    void resizeAnimation(entt::registry &reg, entt:: entity entt,float dt);
};