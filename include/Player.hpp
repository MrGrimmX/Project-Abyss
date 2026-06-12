#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Player {
public:
    sf::Vector2f pos;
    sf::Vector2f getPosition() const;
    float angle;

    // Estadísticas del jugador
    int health;
    int ammo;
    int score;
    int lives;

    bool blueKey;
    bool goldKey;


    Player(sf::Vector2f startPos);
    void update(float deltaTime, MapManager& mapManager);
private:
    sf::RectangleShape body;
};

#endif