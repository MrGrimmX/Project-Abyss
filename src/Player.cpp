#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

const int TILE_SIZE = 64;

Player::Player(sf::Vector2f startPos) {
    pos = startPos;
    angle = 0.0f;
    speed = 180.0f;
    rotationSpeed = 3.2f;
}

void Player::update(float deltaTime, MapManager& mapManager) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) angle -= rotationSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) angle += rotationSpeed * deltaTime;

    sf::Vector2f moveDir(std::cos(angle), std::sin(angle));
    sf::Vector2f nextPos = pos;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) nextPos += moveDir * speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) nextPos -= moveDir * speed * deltaTime;

    int cellX = nextPos.x / TILE_SIZE;
    int cellY = nextPos.y / TILE_SIZE;

    if (!mapManager.isWall(cellX, cellY)) {
        pos = nextPos;
    }

    if (mapManager.isExit(cellX, cellY)) {
        mapManager.nextLevel();
        pos = mapManager.getCurrentLevel()->spawnPoint;
        angle = 0.0f;
    }
}