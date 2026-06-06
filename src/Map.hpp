#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Tile {
    int type; // 0=vacío, 1=muro, 2=puerta, 3=MURO SECRETO, 9=salida
};

// Estructura para controlar la animación del muro secreto
struct PushWall {
    bool isActive = false;
    int originalX, originalY; // Coordenadas de la celda de origen
    int targetX, targetY;     // Coordenadas de la celda de destino
    float moveX = 0.0f;       // Desplazamiento actual en el eje X (en píxeles)
    float moveY = 0.0f;       // Desplazamiento actual en el eje Y (en píxeles)
    float speed = 64.0f;      // Velocidad de movimiento (1 celda por segundo)
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
    PushWall activeSecret; // <-- El muro secreto activo en el frame actual

    MapManager();
    Level* getCurrentLevel();
    int getCurrentIndex() const;
    void nextLevel();
    
    bool isWall(int cellX, int cellY);
    bool isDoor(int cellX, int cellY);
    bool isExit(int cellX, int cellY);
    bool isSecret(int cellX, int cellY); // <-- Nueva función
    
    void openDoor(int cellX, int cellY);
    void triggerSecret(int cellX, int cellY, float playerAngle); // <-- Nueva función
    void updateSecret(float deltaTime);                          // <-- Nueva función
    
    void drawFullMap(sf::RenderWindow& window);
};

#endif