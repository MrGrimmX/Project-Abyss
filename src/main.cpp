#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

enum class EnemyState { IDLE, CHASE, ATTACK };

class DoomEnemy {
public:
    sf::Vector2f pos;
    EnemyState state;
    float speed;
    float attackRange;
    float size;

    DoomEnemy(float x, float y) {
        pos = sf::Vector2f(x, y);
        state = EnemyState::IDLE;
        speed = 120.0f;       
        attackRange = 45.0f;  
        size = 30.0f;         
    }

    void update(sf::Vector2f playerPos, const std::vector<sf::FloatRect>& walls, float deltaTime) {
        sf::Vector2f toPlayer = playerPos - pos;
        float distance = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        switch (state) {
            case EnemyState::IDLE:
                if (distance < 300.0f) state = EnemyState::CHASE;
                break;

            case EnemyState::CHASE: {
                if (distance <= attackRange) {
                    state = EnemyState::ATTACK;
                } else {
                    sf::Vector2f direction = toPlayer / distance;
                    sf::Vector2f nextPos = pos + direction * speed * deltaTime;

                    sf::FloatRect futureBounds(nextPos.x - size / 2.0f, nextPos.y - size / 2.0f, size, size);
                    bool collision = false;
                    for (const auto& wall : walls) {
                        if (futureBounds.intersects(wall)) { collision = true; break; }
                    }
                    if (!collision) pos = nextPos;
                }
                break;
            }

            case EnemyState::ATTACK:
                if (distance > attackRange) state = EnemyState::CHASE;
                break;
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "DOOM Base - SFML");
    window.setFramerateLimit(60);

    sf::Vector2f playerPos(400.0f, 300.0f);
    float playerSpeed = 200.0f;
    float playerSize = 30.0f;

    DoomEnemy enemy(150.0f, 150.0f);

    std::vector<sf::FloatRect> walls;
    walls.push_back(sf::FloatRect(100.0f, 450.0f, 600.0f, 40.0f)); 
    walls.push_back(sf::FloatRect(550.0f, 150.0f, 40.0f, 200.0f)); 

    sf::Clock deltaClock;

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        sf::Vector2f movement(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= playerSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += playerSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= playerSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += playerSpeed * deltaTime;

        // Colisión X
        sf::Vector2f nextPlayerPos = playerPos;
        nextPlayerPos.x += movement.x;
        sf::FloatRect playerBoundsX(nextPlayerPos.x - playerSize/2.0f, playerPos.y - playerSize/2.0f, playerSize, playerSize);
        bool collideX = false;
        for (const auto& wall : walls) { if (playerBoundsX.intersects(wall)) { collideX = true; break; } }
        if (!collideX) playerPos.x = nextPlayerPos.x;

        // Colisión Y
        nextPlayerPos = playerPos;
        nextPlayerPos.y += movement.y;
        sf::FloatRect playerBoundsY(playerPos.x - playerSize/2.0f, nextPlayerPos.y - playerSize/2.0f, playerSize, playerSize);
        bool collideY = false;
        for (const auto& wall : walls) { if (playerBoundsY.intersects(wall)) { collideY = true; break; } }
        if (!collideY) playerPos.y = nextPlayerPos.y;

        enemy.update(playerPos, walls, deltaTime);

        window.clear(sf::Color(30, 30, 30));

        for (const auto& wall : walls) {
            sf::RectangleShape wallSprite(sf::Vector2f(wall.width, wall.height));
            wallSprite.setPosition(wall.left, wall.top);
            wallSprite.setFillColor(sf::Color(100, 100, 100));
            window.draw(wallSprite);
        }

        sf::RectangleShape playerSprite(sf::Vector2f(playerSize, playerSize));
        playerSprite.setOrigin(playerSize / 2.0f, playerSize / 2.0f);
        playerSprite.setPosition(playerPos);
        playerSprite.setFillColor(sf::Color::Green);
        window.draw(playerSprite);

        sf::RectangleShape enemySprite(sf::Vector2f(enemy.size, enemy.size));
        enemySprite.setOrigin(enemy.size / 2.0f, enemy.size / 2.0f);
        enemySprite.setPosition(enemy.pos);
        
        if (enemy.state == EnemyState::IDLE) enemySprite.setFillColor(sf::Color::White);
        else if (enemy.state == EnemyState::CHASE) enemySprite.setFillColor(sf::Color::Red);
        else if (enemy.state == EnemyState::ATTACK) enemySprite.setFillColor(sf::Color::Magenta);
        
        window.draw(enemySprite);
        window.display();
    }
    return 0;
}