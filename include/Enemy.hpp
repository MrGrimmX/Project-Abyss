#pragma once

#include <SFML/Graphics.hpp>

enum EnemyState
{
    IDLE,
    WALK,
    ATTACK,
    HURT,
    DEAD
};

class Enemy
{
public:
    Enemy(float x, float y);

    void update(float deltaTime);
    void takeDamage(int damage);

    sf::Sprite sprite;

    float posX;
    float posY;

    bool isAlive() const;

private:
    sf::Texture texture;

    EnemyState state;

    int health;

    int currentFrame;
    float animationTimer;
    float frameDuration;

    void updateAnimation();
};