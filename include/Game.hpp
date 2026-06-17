#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Player.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"
#include "Menu.hpp"
#include "Enemy.hpp"
#include "Constante.hpp"

class Game {
private:
    sf::RenderWindow window;
    Player player;
    MapManager mapManager;
    sf::Clock gameClock;
    TextureManager textureManager;
    Menu menu;
    GameState gameState;
    sf::Font hudFont;
    sf::Music backgroundMusic;
    std::vector<Enemy> enemies;
    sf::Vector2f findSpawnPosition();
    void processEvents();
    void update(float deltaTime);
    void render();
    void renderRaycast();
    void renderHUD();
    void renderEnemies();
    void renderSprites();
    float zBuffer[SCREEN_WIDTH];
    
public:
    Game();
    ~Game();
    void run();
};

#endif