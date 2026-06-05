#pragma once
#include <SFML/System/Vector2.hpp>
#include "Map.hpp"

class Player {
public:
    sf::Vector2f pos;
    float angle;
    float speed;
    float rotationSpeed;

    Player(sf::Vector2f startPos);
    void update(float deltaTime, MapManager& mapManager);
};