#include "Map.hpp"
#include "EnemyManager.hpp"
#include "Level.hpp"
#include <iostream>
#include <cmath>
#define TILE_SIZE 64

MapManager::MapManager() {
    currentLevelIndex = 0;
    showFullMap = false;

    levels = getBuiltInLevels();
}

Level* MapManager::getCurrentLevel() { 
    if (levels.empty()) {
        static Level dummy = {"Dummy", 1, 1, {96.f, 96.f}, {{{0}}}};
        return &dummy;
    }
    return &levels[currentLevelIndex]; 
}

int MapManager::getCurrentIndex() const { return currentLevelIndex; }

void MapManager::nextLevel() { 
    if (!levels.empty()) {
        currentLevelIndex = (currentLevelIndex + 1) % levels.size(); 
    }
}

bool MapManager::isWall(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return true;
    
    if (activeSecret.isActive) {
        float currentXpx = activeSecret.originalX * 64 + activeSecret.moveX;
        float currentYpx = activeSecret.originalY * 64 + activeSecret.moveY;

        int occupiedCellX1 = (int)(currentXpx) / 64;
        int occupiedCellY1 = (int)(currentYpx) / 64;
        int occupiedCellX2 = (int)(currentXpx + 63.0f) / 64;
        int occupiedCellY2 = (int)(currentYpx + 63.0f) / 64;

        if ((cellX == occupiedCellX1 && cellY == occupiedCellY1) ||
            (cellX == occupiedCellX2 && cellY == occupiedCellY2)) {
            return true;
        }

        if (cellX == activeSecret.targetX && cellY == activeSecret.targetY) {
            return true;
        }
    }

    Tile cell = cur->grid[cellY][cellX];
    if (cell.type == 1) return true;
    if (cell.type == 3) return true;
    if (cell.type == 2 && !cell.isOpen) return true;

    return false;
}

bool MapManager::isDoor(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX].type == 2 && !cur->grid[cellY][cellX].isOpen;
}

bool MapManager::isExit(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX].type == 9;
}

void MapManager::openDoor(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX >= 0 && cellX < cur->cols && cellY >= 0 && cellY < cur->rows) {
        if (cur->grid[cellY][cellX].type == 2) {
            cur->grid[cellY][cellX].isOpen = true;
            cur->grid[cellY][cellX].doorTimer = 0.0f;
        }
    }
}

void MapManager::drawFullMap(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
    if (levels.empty()) return;
    Level* cur = getCurrentLevel();
    int tileSize = 32;
    float startX = (window.getSize().x - (cur->cols * tileSize)) / 2.0f;
    float startY = (window.getSize().y - (cur->rows * tileSize)) / 2.0f;
    sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));
    
    for (int r = 0; r < cur->rows; r++) {
        for (int c = 0; c < cur->cols; c++) {
            Tile cell = cur->grid[r][c];
            
            if (cell.type == 2) {
                tile.setFillColor(sf::Color(70, 130, 200));
            }     
            else if (cell.type == 1) {
                tile.setFillColor(sf::Color(180, 180, 180));
            } 
            else if (cell.type == 9) {
                tile.setFillColor(sf::Color(35, 180, 70));
            }   
            else {
                tile.setFillColor(sf::Color(50, 50, 50));
            }
            
            tile.setPosition(startX + c * tileSize, startY + r * tileSize);
            window.draw(tile);
        }
    }
    
    int playerTileX = (int)(playerPos.x / 64);
    int playerTileY = (int)(playerPos.y / 64);
    
    float playerScreenX = startX + playerTileX * tileSize;
    float playerScreenY = startY + playerTileY * tileSize;
    
    sf::RectangleShape playerTile(sf::Vector2f(tileSize - 2, tileSize - 2));
    playerTile.setFillColor(sf::Color(255, 50, 50));
    playerTile.setPosition(playerScreenX, playerScreenY);
    window.draw(playerTile);
    
    sf::RectangleShape playerBorder(sf::Vector2f(tileSize - 2, tileSize - 2));
    playerBorder.setFillColor(sf::Color::Transparent);
    playerBorder.setOutlineColor(sf::Color::White);
    playerBorder.setOutlineThickness(2);
    playerBorder.setPosition(playerScreenX, playerScreenY);
    window.draw(playerBorder);

}

bool MapManager::isSecret(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX].type == 3;
}

void MapManager::triggerSecret(int cellX, int cellY, float playerAngle) {
    if (activeSecret.isActive) return;

    Level* cur = getCurrentLevel();

    int dirX = 0;
    int dirY = 0;

    float normAngle = playerAngle;
    while (normAngle < 0) normAngle += 2 * 3.14159265f;
    while (normAngle >= 2 * 3.14159265f) normAngle -= 2 * 3.14159265f;

    if (normAngle >= 7.0f * 3.14159265f / 4.0f || normAngle < 3.14159265f / 4.0f) dirX = 1;
    else if (normAngle >= 1.0f * 3.14159265f / 4.0f && normAngle < 3.0f * 3.14159265f / 4.0f) dirY = 1;
    else if (normAngle >= 3.0f * 3.14159265f / 4.0f && normAngle < 5.0f * 3.14159265f / 4.0f) dirX = -1;
    else dirY = -1;

    int tX = cellX + dirX;
    int tY = cellY + dirY;

    if (tX >= 0 && tX < cur->cols && tY >= 0 && tY < cur->rows && cur->grid[tY][tX].type == 0) {
        activeSecret.isActive = true;
        activeSecret.originalX = cellX;
        activeSecret.originalY = cellY;
        activeSecret.targetX = tX;
        activeSecret.targetY = tY;
        activeSecret.moveX = 0.0f;
        activeSecret.moveY = 0.0f;

        cur->grid[cellY][cellX].type = 0;
    }
}

void MapManager::updateSecret(float deltaTime) {
    if (!activeSecret.isActive) return;

    int dirX = activeSecret.targetX - activeSecret.originalX;
    int dirY = activeSecret.targetY - activeSecret.originalY;

    activeSecret.moveX += dirX * activeSecret.speed * deltaTime;
    activeSecret.moveY += dirY * activeSecret.speed * deltaTime;

    float distanceMoved = std::sqrt(activeSecret.moveX * activeSecret.moveX + activeSecret.moveY * activeSecret.moveY);
    if (distanceMoved >= 64.0f) {
        Level* cur = getCurrentLevel();
        cur->grid[activeSecret.targetY][activeSecret.targetX].type = 1;

        activeSecret.isActive = false;
    }
}

void MapManager::updateDoors(float deltaTime, sf::Vector2f playerPos) {
    Level* cur = getCurrentLevel();
    
    int playerCellX = (int)(playerPos.x) / 64;
    int playerCellY = (int)(playerPos.y) / 64;

    for (int r = 0; r < cur->rows; r++) {
        for (int c = 0; c < cur->cols; c++) {
            if (cur->grid[r][c].type == 2 && cur->grid[r][c].isOpen) {
                cur->grid[r][c].doorTimer += deltaTime;

                if (cur->grid[r][c].doorTimer >= 5.0f) {
                    
                    if (c == playerCellX && r == playerCellY) {
                        cur->grid[r][c].doorTimer = 3.0f;
                        continue;
                    }

                    cur->grid[r][c].isOpen = false;
                    cur->grid[r][c].doorTimer = 0.0f;
                }
            }
        }
    }
    
}