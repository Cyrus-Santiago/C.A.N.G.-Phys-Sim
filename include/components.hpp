#include "../include/glm/glm.hpp"
#include "../include/texture.hpp"

typedef struct{
    float mass = 1.00; //Default to 1 kg (Are we using kg or lbs?)
}physics;

typedef struct{
    float xPos = 50;
    float yPos = 50;
    float xSize = 10;
    float ySize = 10;
}dimensions;
//Can change based on what we want to store
    
typedef struct{
    Texture2D &texture;
    glm::vec2 position;
    glm::vec2 size = glm::vec2(10.f,10.0f);
    float rotate = 0.0f;
    glm::vec4 color = glm::vec4(1.0f);
}renderable;

struct TagComponent{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag){}
};