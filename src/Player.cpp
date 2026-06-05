#include "Player.hpp"
#include <cmath>

Player::Player(sf::Vector2f spawnPos) {
    pos = spawnPos;
    angle = 0.0f;
    cameraZ = 0.0f;
}

void Player::update(float deltaTime, MapManager& mapManager) {
    float moveSpeed = 150.0f * deltaTime;
    float rotSpeed = 3.0f * deltaTime;

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

    int oldCellX = (int)(oldPos.x) / 64;
    int oldCellY = (int)(oldPos.y) / 64;

    // 🌟 PROTECCIÓN CRÍTICA: Validar que los índices estén estrictamente dentro del mapa antes de leer la matriz
    if (cellX >= 0 && cellX < cur->cols && cellY >= 0 && cellY < cur->rows &&
        oldCellX >= 0 && oldCellX < cur->cols && oldCellY >= 0 && oldCellY < cur->rows) 
    {
        float targetFloorHeight = cur->grid[cellY][cellX].floorHeight;
        float currentFloorHeight = cur->grid[oldCellY][oldCellX].floorHeight;

        // Si choca con muros, puertas cerradas o escalones muy altos
        if (cur->grid[cellY][cellX].type == 1 || 
            cur->grid[cellY][cellX].type == 2 || 
            (targetFloorHeight - currentFloorHeight) > 0.35f) 
        {
            pos = oldPos; // Revertir movimiento
            cellX = oldCellX;
            cellY = oldCellY;
            targetFloorHeight = currentFloorHeight;
        }

        // Suavizado de la cámara
        float targetCameraZ = targetFloorHeight * 220.0f;
        cameraZ += (targetCameraZ - cameraZ) * 12.0f * deltaTime;
    } else {
        // Si por alguna razón el jugador se sale del mapa, lo regresamos a una zona segura
        pos = oldPos;
        cellX = oldCellX;
        cellY = oldCellY;
    }

    // Comprobación de salida segura
    if (mapManager.isExit(cellX, cellY)) {
        mapManager.nextLevel();
        pos = mapManager.getCurrentLevel()->spawnPoint;
    }
}

void Player::drawOnFullMap(sf::RenderWindow& window, MapManager& mapManager) {
    Level* cur = mapManager.getCurrentLevel();
    int tileSize = 32;
    float startX = (window.getSize().x - (cur->cols * tileSize)) / 2.0f;
    float startY = (window.getSize().y - (cur->rows * tileSize)) / 2.0f;

    sf::CircleShape pDot(5.0f);
    pDot.setFillColor(sf::Color::Red);
    
    float mapX = startX + (pos.x / 64.0f) * tileSize;
    float mapY = startY + (pos.y / 64.0f) * tileSize;
    
    pDot.setPosition(mapX - 2.5f, mapY - 2.5f);
    window.draw(pDot);
}