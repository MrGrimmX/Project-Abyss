#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Tile {
    int type;
    bool isOpen = false;
    float doorTimer = 0.0f;
};

struct PushWall {
    bool isActive = false;
    int originalX, originalY;
    int targetX, targetY;
    float moveX = 0.0f;
    float moveY = 0.0f;
    float speed = 64.0f;
};

struct Level {
    std::string name;
    int rows;
    int cols;
    sf::Vector2f spawnPoint;
    std::vector<std::vector<Tile>> grid;
};

class MapManager {
private:
    std::vector<Level> levels;
    int currentLevelIndex;

public:
    bool showFullMap;
    PushWall activeSecret;

    MapManager();
    Level* getCurrentLevel();
    int getCurrentIndex() const;
    void nextLevel();
    bool isWall(int cellX, int cellY);
    bool isDoor(int cellX, int cellY);
    bool isExit(int cellX, int cellY);
    bool isSecret(int cellX, int cellY);
    void openDoor(int cellX, int cellY);
    void updateDoors(float deltaTime, sf::Vector2f playerPos);
    void triggerSecret(int cellX, int cellY, float playerAngle);
    void updateSecret(float deltaTime);
    void drawFullMap(sf::RenderWindow& window, const sf::Vector2f& playerPos);
};

#endif