#include "Game.hpp"
#include <cmath>

Game::Game() : 
    window(sf::VideoMode(800, 600), "Raycaster Modular"), 
    player(mapManager.getCurrentLevel()->spawnPoint) 
{
    window.setFramerateLimit(60);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        renderRaycast();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
    }
}

void Game::update(float deltaTime) {
    player.update(deltaTime, mapManager);
}

void Game::renderRaycast() {
    window.clear();

    // Dibujar Techo y Suelo fijos
    sf::RectangleShape ceiling(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2));
    ceiling.setFillColor(sf::Color(20, 20, 20));
    window.draw(ceiling);

    sf::RectangleShape floor(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2));
    floor.setPosition(0, SCREEN_HEIGHT / 2);
    floor.setFillColor(sf::Color(40, 40, 40));
    window.draw(floor);

    // Algoritmo de Raycasting
    float halfFOV = FOV / 2.0f;
    const int TILE_SIZE = 64;

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = (player.angle - halfFOV) + ((float)x / (float)SCREEN_WIDTH) * FOV;
        float distanceToWall = 0.0f;
        bool hitWall = false;
        bool hitExit = false;

        float cosRay = std::cos(rayAngle);
        float sinRay = std::sin(rayAngle);

        while (!hitWall && distanceToWall < MAX_DEPTH) {
            distanceToWall += 1.5f;
            int testX = (int)(player.pos.x + cosRay * distanceToWall) / TILE_SIZE;
            int testY = (int)(player.pos.y + sinRay * distanceToWall) / TILE_SIZE;

            if (mapManager.isWall(testX, testY)) {
                hitWall = true;
            } else if (mapManager.isExit(testX, testY)) {
                hitWall = true;
                hitExit = true;
            }
        }

        float correctedDistance = distanceToWall * std::cos(rayAngle - player.angle);
        int wallHeight = (int)(SCREEN_HEIGHT * TILE_SIZE / correctedDistance);
        if (wallHeight > SCREEN_HEIGHT) wallHeight = SCREEN_HEIGHT;
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);

        sf::RectangleShape wallColumn(sf::Vector2f(1.0f, wallHeight));
        wallColumn.setPosition(x, wallTop);

        int shadow = 220 - (int)(correctedDistance * 0.25f);
        if (shadow < 20) shadow = 20;

        if (hitExit) {
            wallColumn.setFillColor(sf::Color(0, shadow, 0)); // Salida Verde
        } else {
            // El color del muro cambia dependiendo del nivel activo
            if (mapManager.getCurrentIndex() == 0)
                wallColumn.setFillColor(sf::Color(shadow, shadow / 2, 0)); 
            else
                wallColumn.setFillColor(sf::Color(0, shadow, shadow));
        }

        window.draw(wallColumn);
    }

    window.display();
}