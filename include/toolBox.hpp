/*
Since each of these classes fulfill similar roles and control the logic of the toolbar
buttons, I've decided to put all of them in one file for simplicity sake, rather than creating
a seperate file for each similar class with similar methods. I can fix it later if it's a 
universally hated design choice. -Cyrus
*/

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "entt.hpp"
#include "../include/input.hpp"

/*Methods pertaining to the Move button functionality*/
class Move{
    public:
        static void moveObject(entt::registry *reg, Click newMouseClick);
        static entt::entity getObject(entt::registry *reg, Click newMouseClick);
        void moveLoop(entt::registry *reg, float dt);
};

/*Methods pertaining to the Resize button functionality*/
class Resize{
    public:
        void resizeObject(entt::registry *reg, Click newMouseClick);
        void outlineObject(entt::registry *reg, entt::entity obj);
};

/*Methods pertaining to the Delete button functionality*/
class Delete{
    public:
        void deleteObject();
};

#endif