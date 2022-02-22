// This code was written by Nate

#include "../include/character.hpp"

#include <glm/fwd.hpp>
#include <string>
#include <vector>

// this class facilitates rendering text. Well actually sprite renderer
// does that still... should've called it TextManager or something
class TextRenderer {
public:
    // this array is for storing bitmap coordinates
    std::vector<Character> CharacterPositionData;
    // this array is for storing words we want to draw
    std::vector<Character> Characters;

    // constructor
    TextRenderer() { }

    // initial setup (mainly adding bitmap data to the array)
    void Init();
    
    // draws every character in the Characters array
    void Draw(SpriteRenderer &renderer, std::string sentence,
              glm::vec2 position, unsigned int fontSize,
              bool fade = false);

    void SetMessage(SpriteRenderer &renderer, std::string sentence);

    void Update(float dt);

    void FlashMessage(bool flag);

private:
    float dt;
    float timeElapsed;
    bool stopDrawing;
    bool statusOn;
};