#include "Map.hpp"

MapManager::MapManager() {
    currentLevelIndex = 0;

    // Nivel 1
    Level lvl1;
    lvl1.name = "Hangar de Inicio";
    lvl1.rows = 5; lvl1.cols = 5;
    lvl1.spawnPoint = sf::Vector2f(96.0f, 96.0f);
    lvl1.grid = {
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,1,0,1},
        {1,0,0,9,1}, // 9 es la salida
        {1,1,1,1,1}
    };
    levels.push_back(lvl1);

    // Nivel 2
    Level lvl2;
    lvl2.name = "El Laboratorio";
    lvl2.rows = 6; lvl2.cols = 6;
    lvl2.spawnPoint = sf::Vector2f(96.0f, 96.0f);
    lvl2.grid = {
        {1,1,1,1,1,1},
        {1,0,0,0,0,1},
        {1,0,1,1,0,1},
        {1,0,0,1,0,1},
        {1,0,0,0,9,1},
        {1,1,1,1,1,1}
    };
    levels.push_back(lvl2);
}

Level* MapManager::getCurrentLevel() {
    return &levels[currentLevelIndex];
}

int MapManager::getCurrentIndex() const {
    return currentLevelIndex;
}

void MapManager::nextLevel() {
    currentLevelIndex = (currentLevelIndex + 1) % levels.size();
}

bool MapManager::isWall(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return true;
    return cur->grid[cellY][cellX] == 1;
}

bool MapManager::isExit(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX] == 9;
}