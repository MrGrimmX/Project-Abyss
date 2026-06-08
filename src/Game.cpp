#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Player.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"
#include "Menu.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 64;

class Game {
private:
    sf::RenderWindow window;
    MapManager mapManager;
    Player player;
    sf::Clock gameClock;
    TextureManager textureManager;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                float lookX = player.pos.x + std::cos(player.angle) * 45.0f;
                float lookY = player.pos.y + std::sin(player.angle) * 45.0f;
                int targetX = (int)(lookX) / TILE_SIZE;
                int targetY = (int)(lookY) / TILE_SIZE;

                if (mapManager.isDoor(targetX, targetY)) {
                    mapManager.openDoor(targetX, targetY);
                }
                else if (mapManager.isSecret(targetX, targetY)) {
                    mapManager.triggerSecret(targetX, targetY, player.angle);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                mapManager.showFullMap = !mapManager.showFullMap;
            }
        }
    }

    void update(float deltaTime) {
        player.update(deltaTime, mapManager);
        mapManager.updateSecret(deltaTime);
        mapManager.updateDoors(deltaTime, player.pos);
    }

    void renderRaycast() {
        float fov = 60.0f * (3.14159265f / 180.0f);
        float halfFov = fov / 2.0f;
        int numRays = SCREEN_WIDTH;
        float deltaAngle = fov / (float)numRays;

        Level* curLevel = mapManager.getCurrentLevel();

        for (int i = 0; i < numRays; i++) {
            float rayAngle = (player.angle - halfFov) + (float)i * deltaAngle;
            
            while (rayAngle < 0) rayAngle += 2.0f * 3.14159265f;
            while (rayAngle >= 2.0f * 3.14159265f) rayAngle -= 2.0f * 3.14159265f;

            float cosRay = std::cos(rayAngle);
            float sinRay = std::sin(rayAngle);

            float distance = 0.0f;
            float stepSize = 0.5f; 
            bool hitWall = false;
            int wallType = 1;      
            int side = 0;          // 0 = Impacto Este/Oeste, 1 = Impacto Norte/Sur
            
            float rayX = player.pos.x;
            float rayY = player.pos.y;

while (!hitWall && distance < 800.0f) {
                distance += stepSize;
                rayX = player.pos.x + cosRay * distance;
                rayY = player.pos.y + sinRay * distance;

                int testX = (int)(rayX) / TILE_SIZE;
                int testY = (int)(rayY) / TILE_SIZE;

                // 🌟 1. DETECCIÓN DINÁMICA DEL MURO SECRETO (SI SE ESTÁ MOVIENDO)
                if (mapManager.activeSecret.isActive) {
                    float wallMinX = (float)mapManager.activeSecret.originalX * TILE_SIZE + mapManager.activeSecret.moveX;
                    float wallMaxX = wallMinX + TILE_SIZE;
                    float wallMinY = (float)mapManager.activeSecret.originalY * TILE_SIZE + mapManager.activeSecret.moveY;
                    float wallMaxY = wallMinY + TILE_SIZE;

                    if (rayX >= wallMinX && rayX < wallMaxX && rayY >= wallMinY && rayY < wallMaxY) {
                        wallType = 3; 
                        hitWall = true;

                        float hitX = rayX - wallMinX;
                        float hitY = rayY - wallMinY;

                        if (std::min(hitX, (float)TILE_SIZE - hitX) < std::min(hitY, (float)TILE_SIZE - hitY)) {
                            side = 0;
                        } else {
                            side = 1;
                        }
                        break; // Impacto dinámico encontrado
                    }
                }

                // 2. DETECCIÓN DE MUROS ESTÁTICOS Y SECRETOS EN REPOSO
                if (testX >= 0 && testX < curLevel->cols && testY >= 0 && testY < curLevel->rows) {
                    int type = curLevel->grid[testY][testX].type;
                    
                    // 🌟 LA CLAVE: Detectar tipo 1, 2, 9 O TIPO 3 (SECRETO) SI NO SE ESTÁ MOVIENDO
                    bool isStaticObstacle = (type == 1 || type == 9 || (type == 2 && !curLevel->grid[testY][testX].isOpen));
                    bool isSecretAtRest = (type == 3 && !mapManager.activeSecret.isActive);

                    if (isStaticObstacle || isSecretAtRest) {
                        wallType = type;
                        hitWall = true;

                        float hitX = rayX - (testX * TILE_SIZE);
                        float hitY = rayY - (testY * TILE_SIZE);

                        if (std::min(hitX, (float)TILE_SIZE - hitX) < std::min(hitY, (float)TILE_SIZE - hitY)) {
                            side = 0;
                        } else {
                            side = 1;
                        }
                    }
                } else {
                    hitWall = true; 
                }
            }

            // Proyección geométrica vertical
            float correctedDistance = distance * std::cos(rayAngle - player.angle);
            if (correctedDistance < 1.0f) correctedDistance = 1.0f; 

            int wallHeight = (int)((TILE_SIZE * SCREEN_HEIGHT) / correctedDistance);
            int drawStart = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
            int drawEnd = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

            int screenDrawStart = std::max(0, drawStart);
            int screenDrawEnd = std::min(SCREEN_HEIGHT - 1, drawEnd);

            // --- MAPEO HORIZONTAL DE TEXTURA ADAPTABLE (32x32) ---
            int blockX = (int)rayX % TILE_SIZE;
            int blockY = (int)rayY % TILE_SIZE;
            int wallOffset = (side == 0) ? blockY : blockX;

            // Factor de escala: Tamaño real (32) / Tamaño del mundo (64)
            float scaleFactor = (float)textureManager.getTextureSize() / (float)TILE_SIZE;
            int texX = (int)((float)wallOffset * scaleFactor);

            if ((side == 0 && cosRay > 0) || (side == 1 && sinRay < 0)) {
                texX = textureManager.getTextureSize() - 1 - texX;
            }
            texX = std::max(0, std::min(texX, textureManager.getTextureSize() - 1));

            // --- RASTERIZADO VERTICAL CON VERTEX ARRAY ---
            sf::VertexArray wallSlice(sf::Points, screenDrawEnd - screenDrawStart);
            int vertexCounter = 0;

            for (int y = screenDrawStart; y < screenDrawEnd; y++) {
                float texYRatio = (float)(y - drawStart) / (float)wallHeight;
                int texY = (int)(texYRatio * (float)textureManager.getTextureSize());
                texY = std::max(0, std::min(texY, textureManager.getTextureSize() - 1));

                // Si es el muro secreto (tipo 3), forzamos el renderizado de la textura de ladrillo (ID 1)
                int textureToRender = (wallType == 3) ? 1 : wallType;
                sf::Color pixelColor = textureManager.getPixelColor(textureToRender, texX, texY);

                // Efecto neblina / Profundidad en base a la distancia corregida
                float shadow = 1.0f;
                if (correctedDistance > 150.0f) {
                    shadow = 1.0f - (correctedDistance / 750.0f);
                    if (shadow < 0.15f) shadow = 0.15f;
                }
                if (side == 1) {
                    shadow *= 0.7f; // Caras laterales más oscuras para volumen 3D
                }

                pixelColor.r = (sf::Uint8)((float)pixelColor.r * shadow);
                pixelColor.g = (sf::Uint8)((float)pixelColor.g * shadow);
                pixelColor.b = (sf::Uint8)((float)pixelColor.b * shadow);

                wallSlice[vertexCounter].position = sf::Vector2f((float)i, (float)y);
                wallSlice[vertexCounter].color = pixelColor;
                vertexCounter++;
            }

            window.draw(wallSlice);
        }
    }

void render() {
        // 1. Techo: Pintamos toda la pantalla con un gris oscuro uniforme
        window.clear(sf::Color(45, 45, 50)); 

        // 2. Suelo: Dibujamos un rectángulo que cubre la mitad inferior con un gris más claro
        sf::RectangleShape floorShape(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2.0f));
        floorShape.setPosition(0.0f, (float)SCREEN_HEIGHT / 2.0f);
        floorShape.setFillColor(sf::Color(145, 145, 145)); // Gris claro tipo concreto/metal
        window.draw(floorShape);
        
        // 3. Dibujar los muros con texturas encima de la escena
        renderRaycast();

        if (mapManager.showFullMap) {
            mapManager.drawFullMap(window);
        }

        window.display();
    }

public:
    Game() : 
        window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Wolfenstein 3D Engine - CETI"),
        player(sf::Vector2f(96.0f, 96.0f)),
        textureManager(32) 
    {
        window.setFramerateLimit(60);
        player.pos = mapManager.getCurrentLevel()->spawnPoint;
        
        if (!textureManager.loadTextures("../assets/texturas/CASTLEBRICKS.png")) {
            std::cerr << "🛑 Alerta: No se pudo cargar ../assets/texturas/CASTLEBRICKS.png" << std::endl;
        } else {
            std::cout << "✅ Éxito: Base recuperada y texturas de 32x32 cargadas." << std::endl;
        }
    }

    void run() {
        while (window.isOpen()) {
            float deltaTime = gameClock.restart().asSeconds();
            processEvents();
            update(deltaTime);
            render();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}