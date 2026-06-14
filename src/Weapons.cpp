#include <SFML/Graphics.hpp>
#include "Weapons.hpp"

Weapon::Weapon()
{
    damage = 0;
    ammoCost = 1;
    
    currentFrame = 0;

    totalFrames = 1;

    animationSpeed = 0.05f;
    animationTimer = 0.f;

    shooting = false;
}

void Weapon::shoot()
{
    if(!shooting)
    {
        shooting = true;
        currentFrame = 0;
    }
}

void Weapon::update(float deltaTime)
{
    if(!shooting)
        return;

    animationTimer += deltaTime;

    if(animationTimer >= animationSpeed)
    {
        animationTimer = 0.f;

        currentFrame++;

        if(currentFrame >= totalFrames)
        {
            currentFrame = 0;
            shooting = false;
        }

        sprite.setTextureRect(
            sf::IntRect(
                currentFrame * frameWidth,
                0,
                frameWidth,
                frameHeight
            )
        );
    }
}