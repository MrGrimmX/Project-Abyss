#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"
#include "Menu.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"

class Game {
private:
    sf::RenderWindow window;
    MapManager mapManager;
    Player player;
    sf::Clock gameClock;
    TextureManager textureManager;
    Menu menu;
    GameState gameState;
    sf::Font hudFont;
    std::vector<Enemy> enemies;
    sf::Vector2f findSpawnPosition();
    void spawnWave(int wave);
    bool waveSpawned = false;
    int currentWave;
    void processEvents();
    void update(float deltaTime);
    void render();
    void renderRaycast();
    void renderHUD();
    void renderEnemies();
    void loadFixedEnemies();
    
public:
    Game();
    ~Game();
    void run();
};

#endif