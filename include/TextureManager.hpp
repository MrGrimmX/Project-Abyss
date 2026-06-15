#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class TextureManager {
private:
    sf::Image textureSheet;
    int textureSize;

public:
    TextureManager(int size = 64);
    
    bool loadTextures(const std::string& filename);
    
    sf::Color getPixelColor(int textureID, int texX, int texY);
    
    int getTextureSize() const { return textureSize; }
};

#endif