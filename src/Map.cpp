#include "Map.hpp"
#include "Level.hpp" // Asegúrate de que use el nombre correcto de tu cabecera de niveles
#include <iostream>

MapManager::MapManager() {
    currentLevelIndex = 0;
    showFullMap = false;
    elevatorHeight = 0.0f;
    elevatorDirection = 1;
    elevatorSpeed = 0.6f;

    // Cargar los niveles desde el archivo separado
    levels = getBuiltInLevels();

    // IMPRESIÓN DE SEGURIDAD: Esto te dirá en la terminal de MSYS2 si los mapas se cargaron con éxito
    std::cout << "--- CARGANDO MOTOR ---" << std::endl;
    std::cout << "Niveles cargados exitosamente: " << levels.size() << std::endl;
    if (!levels.empty()) {
        std::cout << "Nivel inicial activo: " << levels[0].name << std::endl;
    } else {
        std::cout << "¡ALERTA! El vector de niveles está VACÍO." << std::endl;
    }
}

Level* MapManager::getCurrentLevel() { 
    // Seguro anti-crash: si por alguna razón la lista está vacía, devuelve una referencia controlada
    if (levels.empty()) {
        static Level dummy = {"Dummy", 1, 1, {96.f, 96.f}, {{{0, 0.f, 1.f, false}}}};
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
    return cur->grid[cellY][cellX].type == 1;
}

bool MapManager::isExit(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX].type == 9;
}

bool MapManager::isDoor(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX < 0 || cellX >= cur->cols || cellY < 0 || cellY >= cur->rows) return false;
    return cur->grid[cellY][cellX].type == 2;
}

void MapManager::openDoor(int cellX, int cellY) {
    Level* cur = getCurrentLevel();
    if (cellX >= 0 && cellX < cur->cols && cellY >= 0 && cellY < cur->rows) {
        if (cur->grid[cellY][cellX].type == 2) cur->grid[cellY][cellX].type = 0;
    }
}

void MapManager::updateElevator(float deltaTime) {
    if (currentLevelIndex != 1 || levels.empty()) return;
    Level* cur = getCurrentLevel();

    elevatorHeight += elevatorSpeed * elevatorDirection * deltaTime;

    if (elevatorHeight >= 1.0f) {
        elevatorHeight = 1.0f;
        elevatorDirection = -1;
    } else if (elevatorHeight <= 0.0f) {
        elevatorHeight = 0.0f;
        elevatorDirection = 1;
    }

    for (int r = 0; r < cur->rows; r++) {
        for (int c = 0; c < cur->cols; c++) {
            if (cur->grid[r][c].isElevator) {
                cur->grid[r][c].floorHeight = elevatorHeight;
            }
        }
    }
}

void MapManager::drawFullMap(sf::RenderWindow& window) {
    if (levels.empty()) return;
    Level* cur = getCurrentLevel();
    int tileSize = 32;
    float startX = (window.getSize().x - (cur->cols * tileSize)) / 2.0f;
    float startY = (window.getSize().y - (cur->rows * tileSize)) / 2.0f;
    sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));

    for (int r = 0; r < cur->rows; r++) {
        for (int c = 0; c < cur->cols; c++) {
            Tile cell = cur->grid[r][c];
            if (cell.type == 1) tile.setFillColor(sf::Color(100, 100, 100));
            else if (cell.type == 2) tile.setFillColor(sf::Color(150, 50, 150));
            else if (cell.type == 9) tile.setFillColor(sf::Color(35, 180, 70));
            else if (cell.isElevator) tile.setFillColor(sf::Color(230, 130, 40));
            else if (cell.floorHeight > 0.0f) tile.setFillColor(sf::Color(40, 110, 190));
            else tile.setFillColor(sf::Color(35, 35, 35));

            tile.setPosition(startX + c * tileSize, startY + r * tileSize);
            window.draw(tile);
        }
    }
}