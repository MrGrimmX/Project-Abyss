#include "Player.hpp"
#include <cmath>

Player::Player(sf::Vector2f startPos) {
    pos = startPos;
    angle = 0.0f;
}

void Player::update(float deltaTime, MapManager& mapManager) {
    float moveSpeed = 180.0f * deltaTime; // Un poco más rápido para acción rápida
    float rotSpeed = 3.5f * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) angle -= rotSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) angle += rotSpeed;

    sf::Vector2f oldPos = pos;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        pos.x += std::cos(angle) * moveSpeed;
        pos.y += std::sin(angle) * moveSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        pos.x -= std::cos(angle) * moveSpeed;
        pos.y -= std::sin(angle) * moveSpeed;
    }

    int cellX = (int)(pos.x) / 64;
    int cellY = (int)(pos.y) / 64;
    Level* cur = mapManager.getCurrentLevel();

    // Protección de bordes y colisiones con muros o puertas
    if (cellX >= 0 && cellX < cur->cols && cellY >= 0 && cellY < cur->rows) {
        if (cur->grid[cellY][cellX].type == 1 || cur->grid[cellY][cellX].type == 2) {
            pos = oldPos; // Choca de frente y regresa
            cellX = (int)(pos.x) / 64;
            cellY = (int)(pos.y) / 64;
        }
    } else {
        pos = oldPos;
    }

    // Si pisa la salida, avanza de nivel
    if (mapManager.isExit(cellX, cellY)) {
        mapManager.nextLevel();
        pos = mapManager.getCurrentLevel()->spawnPoint;
    }
}