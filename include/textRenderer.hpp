
#include "../include/character.hpp"

#include <glm/fwd.hpp>
#include <string>
#include <vector>

class TextRenderer {
public:
    std::vector<Character> CharacterPositionData;
    std::vector<Character> Characters;

    TextRenderer() { }

    void Init();
    
    void Draw(SpriteRenderer &renderer, std::string sentence, glm::vec2 position);
};