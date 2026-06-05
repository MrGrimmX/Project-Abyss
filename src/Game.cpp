#include <SFML/Graphics.hpp>
#include <iostream>   // <-- ¡ESTA FALTA para corregir std::cerr!
#include <cmath>
#include <vector>
#include "Player.hpp"
#include "Map.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;

class Game {
private:
    sf::RenderWindow window;
    MapManager mapManager;
    Player player;
    sf::Clock gameClock;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Abrir puertas con Espacio
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                int cellX = (int)(player.pos.x) / TILE_SIZE;
                int cellY = (int)(player.pos.y) / TILE_SIZE;
                
                // Revisar bloques adyacentes frente al jugador
                float lookX = player.pos.x + std::cos(player.angle) * 40.0f;
                float lookY = player.pos.y + std::sin(player.angle) * 40.0f;
                int targetX = (int)(lookX) / TILE_SIZE;
                int targetY = (int)(lookY) / TILE_SIZE;

                if (mapManager.isDoor(targetX, targetY)) {
                    mapManager.openDoor(targetX, targetY);
                }
            }

            // Alternar mapa con Tab
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                mapManager.showFullMap = !mapManager.showFullMap;
            }
        }
    }

    void update(float deltaTime) {
        player.update(deltaTime, mapManager);
        mapManager.updateElevator(deltaTime);
    }

    void drawColumnSegment(int screenX, float distance, float rayAngle, float currentFloor, float targetFloor, sf::Color color) {
        float correctedDistance = distance * std::cos(rayAngle - player.angle);
        if (correctedDistance < 1.0f) correctedDistance = 1.0f;

        int baseWallHeight = (int)(SCREEN_HEIGHT * TILE_SIZE / correctedDistance);
        int horizonOffset = (int)(player.cameraZ / correctedDistance);

        int finalTop = (SCREEN_HEIGHT / 2) - (baseWallHeight / 2) - horizonOffset + (int)((1.0f - targetFloor) * baseWallHeight);
        int finalBottom = (SCREEN_HEIGHT / 2) + (baseWallHeight / 2) - horizonOffset + (int)((1.0f - currentFloor) * baseWallHeight);

        if (finalTop < 0) finalTop = 0;
        if (finalTop >= SCREEN_HEIGHT) finalTop = SCREEN_HEIGHT - 1;
        if (finalBottom < 0) finalBottom = 0;
        if (finalBottom >= SCREEN_HEIGHT) finalBottom = SCREEN_HEIGHT - 1;

        if (finalTop >= finalBottom) return;

        float shadow = 1.0f - (distance / 700.0f);
        if (shadow < 0.0f) shadow = 0.0f;
        color.r = (sf::Uint8)(color.r * shadow);
        color.g = (sf::Uint8)(color.g * shadow);
        color.b = (sf::Uint8)(color.b * shadow);

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f((float)screenX, (float)finalTop), color),
            sf::Vertex(sf::Vector2f((float)screenX, (float)finalBottom), color)
        };
        window.draw(line, 2, sf::Lines);
    }

    void renderRaycast() {
        Level* curLevel = mapManager.getCurrentLevel();
        float fov = 60.0f * 3.14159265f / 180.0f;
        float startAngle = player.angle - fov / 2.0f;
        float angleStep = fov / (float)SCREEN_WIDTH;

        // Dibujar Techo y Suelo base
        sf::RectangleShape sky(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2));
        sky.setFillColor(sf::Color(25, 25, 25));
        sf::RectangleShape floorRect(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2));
        floorRect.setPosition(0, (float)SCREEN_HEIGHT / 2);
        floorRect.setFillColor(sf::Color(45, 45, 45));
        window.draw(sky);
        window.draw(floorRect);

        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float rayAngle = startAngle + (float)i * angleStep;
            float cosRay = std::cos(rayAngle);
            float sinRay = std::sin(rayAngle);

            float distance = 0.0f;
            float maxDistance = 800.0f;
            float stepSize = 2.0f;
            bool rayFinished = false;
            float currentHighestEdge = 0.0f;

            while (!rayFinished && distance < maxDistance) {
                distance += stepSize;
                float rayX = player.pos.x + cosRay * distance;
                float rayY = player.pos.y + sinRay * distance;

                int testX = (int)(rayX) / TILE_SIZE;
                int testY = (int)(rayY) / TILE_SIZE;

                if (testX >= 0 && testX < curLevel->cols && testY >= 0 && testY < curLevel->rows) {
                    Tile targetTile = curLevel->grid[testY][testX];
                    
                    if (targetTile.type == 1) { // Muro Gris
                        drawColumnSegment(i, distance, rayAngle, currentHighestEdge, 1.5f, sf::Color(90, 95, 100));
                        rayFinished = true;
                    }
                    else if (targetTile.type == 2) { // Puerta Violeta
                        drawColumnSegment(i, distance, rayAngle, currentHighestEdge, 1.0f, sf::Color(130, 40, 130));
                        rayFinished = true;
                    }
                    else if (targetTile.type == 9) { // Salida Verde
                        drawColumnSegment(i, distance, rayAngle, currentHighestEdge, 1.0f, sf::Color(35, 180, 70));
                        rayFinished = true;
                    }
                    else if (targetTile.floorHeight > currentHighestEdge) {
                        sf::Color stepColor = targetTile.isElevator ? sf::Color(190, 110, 30) : sf::Color(50, 100, 160);
                        drawColumnSegment(i, distance, rayAngle, currentHighestEdge, targetTile.floorHeight, stepColor);
                        currentHighestEdge = targetTile.floorHeight;
                    }
                } else {
                    rayFinished = true;
                }
            }
        }
    }

    void render() {
        window.clear();
        if (mapManager.showFullMap) {
            mapManager.drawFullMap(window);
            // Dibujar punto del jugador en el minimapa
            sf::CircleShape pDot(4);
            pDot.setFillColor(sf::Color::Red);
            pDot.setOrigin(4, 4);
            // Centrar con respecto al minimapa
            Level* cur = mapManager.getCurrentLevel();
            float startX = (window.getSize().x - (cur->cols * 32)) / 2.0f;
            float startY = (window.getSize().y - (cur->rows * 32)) / 2.0f;
            pDot.setPosition(startX + (player.pos.x / 64.0f) * 32.0f, startY + (player.pos.y / 64.0f) * 32.0f);
            window.draw(pDot);
        } else {
            renderRaycast();
        }
        window.display();
    }

public:
    Game() : 
        window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Doom Height Engine"),
        player(sf::Vector2f(96.0f, 96.0f)) 
    {
        window.setFramerateLimit(60);
        Level* startLvl = mapManager.getCurrentLevel();
        player.pos = startLvl->spawnPoint;
    }

    void run() {
        Level* test = mapManager.getCurrentLevel();
        if(test->cols <= 1) {
            std::cerr << "CRITICAL ERROR: Geometria invalida." << std::endl;
            return;
        }

        while (window.isOpen()) {
            float deltaTime = gameClock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            render();
        }
    }
};

// Punto de entrada único del programa
int main() {
    Game game;
    game.run();
    return 0;
}