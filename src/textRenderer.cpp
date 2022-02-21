#include "../include/textRenderer.hpp"
#include <glm/fwd.hpp>
#include <string>
#include <iostream>

void TextRenderer::Init() {
    this->CharacterPositionData.push_back(Character('A', glm::vec2(0.07f, 0.01f))); // A
    this->CharacterPositionData.push_back(Character('B', glm::vec2(1.17f, 0.01f))); // B
    this->CharacterPositionData.push_back(Character('C', glm::vec2(2.23f,0.01f))); // C
    this->CharacterPositionData.push_back(Character('D', glm::vec2(3.37f, 0.01f))); // D
    this->CharacterPositionData.push_back(Character('E', glm::vec2(4.47f, 0.01f))); // E
    this->CharacterPositionData.push_back(Character('F', glm::vec2(5.57f, 0.01f))); // F
    this->CharacterPositionData.push_back(Character('G', glm::vec2(6.63f, 0.01f))); // G
    this->CharacterPositionData.push_back(Character('H', glm::vec2(7.77f, 0.01f))); // H
    this->CharacterPositionData.push_back(Character('I', glm::vec2(8.87f, 0.01f))); // I
    this->CharacterPositionData.push_back(Character('J', glm::vec2(0.07f, 1.05f))); // J
    this->CharacterPositionData.push_back(Character('K', glm::vec2(1.17f, 1.05f))); // K
    this->CharacterPositionData.push_back(Character('L', glm::vec2(2.27f, 1.05f))); // L
    this->CharacterPositionData.push_back(Character('M', glm::vec2(3.37f, 1.05f))); // M
    this->CharacterPositionData.push_back(Character('N', glm::vec2(4.50f, 1.05f))); // N
    this->CharacterPositionData.push_back(Character('O', glm::vec2(5.57f, 1.05f))); // O
    this->CharacterPositionData.push_back(Character('P', glm::vec2(6.67f, 1.05f))); // P
    this->CharacterPositionData.push_back(Character('Q', glm::vec2(7.77f, 1.05f))); // Q
    this->CharacterPositionData.push_back(Character('R', glm::vec2(8.87f, 1.05f))); // R
    this->CharacterPositionData.push_back(Character('S', glm::vec2(0.07f, 2.09f))); // S
    this->CharacterPositionData.push_back(Character('T', glm::vec2(1.17f, 2.09f))); // T
    this->CharacterPositionData.push_back(Character('U', glm::vec2(2.305f, 2.09f)));// U
    this->CharacterPositionData.push_back(Character('V', glm::vec2(3.37f, 2.09f))); // V
    this->CharacterPositionData.push_back(Character('W', glm::vec2(4.47f, 2.09f))); // W
    this->CharacterPositionData.push_back(Character('X', glm::vec2(5.57f, 2.09f))); // X
    this->CharacterPositionData.push_back(Character('Y', glm::vec2(6.67f, 2.09f))); // Y
    this->CharacterPositionData.push_back(Character('Z', glm::vec2(7.77f, 2.09f))); // Z
    this->CharacterPositionData.push_back(Character(' ', glm::vec2(8.87f, 2.09f)));// ' '
    this->CharacterPositionData.push_back(Character('.', glm::vec2(1.2f, 7.75f))); //  .
}

void TextRenderer::Draw(SpriteRenderer &renderer, std::string sentence, glm::vec2 position, unsigned int fontSize) {
    for (char &letter : sentence) {
        for (int i = 0; i < CharacterPositionData.size(); i++) {
            if (letter == CharacterPositionData[i].character) {
                Characters.push_back(CharacterPositionData[i]);
                break;
            }
        }
    }
    int i = 0;
    for (Character &character : this->Characters) {
        character.Draw(renderer, i, position, fontSize);
        i++;
    }
    Characters.clear();
}