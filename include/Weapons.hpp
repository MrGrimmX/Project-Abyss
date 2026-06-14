#pragma once
#include <SFML/Graphics.hpp>

enum class WeaponType
{
    Pistol,
    AK,
    Shotgun,
    Cannon,
    SKS,
    Uzi
};

class Weapon
{
public:
    WeaponType type;

    int damage;
    int ammoCost;

    sf::Texture texture;
    sf::Sprite sprite;

    int frameWidth;
    int frameHeight;

    int totalFrames;
    int currentFrame;

    float animationSpeed;
    float animationTimer;

    bool shooting;

    void update(float deltaTime);
    void shoot();
    Weapon();
};