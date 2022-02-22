// this code was written by Nate

#include "../include/textRenderer.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <iostream>

void TextRenderer::Init() {
    
    // an array used to store specific coordinates for letters in the bitmap
    std::vector<Character> charPositions = {
        Character('A', glm::vec2(0.07f, 0.01f)), Character('B', glm::vec2(1.17f, 0.01f)),
        Character('C', glm::vec2(2.23f, 0.01f)), Character('D', glm::vec2(3.37f, 0.01f)),
        Character('E', glm::vec2(4.47f, 0.01f)), Character('F', glm::vec2(5.57f, 0.01f)),
        Character('G', glm::vec2(6.63f, 0.01f)), Character('H', glm::vec2(7.77f, 0.01f)),
        Character('I', glm::vec2(8.87f, 0.01f)), Character('J', glm::vec2(0.07f, 1.05f)),
        Character('K', glm::vec2(1.17f, 1.05f)), Character('L', glm::vec2(2.27f, 1.05f)),
        Character('M', glm::vec2(3.37f, 1.05f)), Character('N', glm::vec2(4.50f, 1.05f)),
        Character('O', glm::vec2(5.57f, 1.05f)), Character('P', glm::vec2(6.67f, 1.05f)),
        Character('Q', glm::vec2(7.77f, 1.05f)), Character('R', glm::vec2(8.87f, 1.05f)),
        Character('S', glm::vec2(0.07f, 2.09f)), Character('T', glm::vec2(1.17f, 2.09f)),
        Character('U', glm::vec2(2.305f,2.09f)), Character('V', glm::vec2(3.37f, 2.09f)),
        Character('W', glm::vec2(4.47f, 2.09f)), Character('X', glm::vec2(5.57f, 2.09f)),
        Character('Y', glm::vec2(6.67f, 2.09f)), Character('Z', glm::vec2(7.77f, 2.09f)),
        Character(' ', glm::vec2(8.87f, 2.09f)), Character('.', glm::vec2(1.2f,  7.75f))
    };

    // here we load up the character position array so we can reference it later
    for (Character &letter : charPositions) {
        this->CharacterPositionData.push_back(Character(letter));
    }
}

void TextRenderer::Draw(SpriteRenderer &renderer, std::string sentence,
                        glm::vec2 position, unsigned int fontSize, bool fade,
                        float dt) {
    // loop through every char in sentence
    for (char &letter : sentence) {
        // loop through each Character object in position data array
        for (int i = 0; i < CharacterPositionData.size(); i++) {
            // if a letter matches data from the array then we add
            // it to the Characters array so we can draw it
            if (letter == CharacterPositionData[i].character) {
                Characters.push_back(CharacterPositionData[i]);
                break;
            }
        }
    }
    int i = 0;
    // here we loop through the characters array and draw each
    // character
    for (Character &character : this->Characters) {
        character.Draw(renderer, i, position, fontSize, fade, dt);
        i++;
    }
    // no longer need the data in this array
    Characters.clear();
}

void TextRenderer::statusMessage(SpriteRenderer &renderer,
                                 std::string sentence, float dt) {
    this->Draw(renderer, sentence, glm::vec2(20, 20), 20, true, dt);
}