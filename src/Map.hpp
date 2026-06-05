#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>

struct Level {
    std::string name;
    int rows;
    int cols;
    std::vector<std::vector<int>> grid;
    sf::Vector2f spawnPoint;
};

class MapManager {
private:
    std::vector<Level> levels;
    int currentLevelIndex;

public:
    MapManager();
    void nextLevel();
    Level* getCurrentLevel();
    int getCurrentIndex() const;
    bool isWall(int cellX, int cellY);
    bool isExit(int cellX, int cellY);
};