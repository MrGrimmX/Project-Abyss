#include "TextureManager.hpp"
#include <iostream>

TextureManager::TextureManager(int size) {
    textureSize = size;
}

bool TextureManager::loadTextures(const std::string& filename) {
    if (!textureSheet.loadFromFile(filename)) {
        std::cerr << "ERROR: No se pudo cargar el archivo de texturas: " << filename << std::endl;
        return false;
    }
    return true;
}

sf::Color TextureManager::getPixelColor(int textureID, int texX, int texY) {
    int sheetX = ((textureID - 1) * textureSize) + texX;
    int sheetY = texY;

    if (sheetX < 0 || sheetX >= (int)textureSheet.getSize().x || sheetY < 0 || sheetY >= (int)textureSheet.getSize().y) {
        return sf::Color::Magenta;
    }

    return textureSheet.getPixel(sheetX, sheetY);
}