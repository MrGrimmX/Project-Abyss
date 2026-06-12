#include "Enemy.hpp"

Enemy::Enemy(float x, float y)
{
    posX = x;
    posY = y;

    texture.loadFromFile("assets/enemies/Recruit.png");

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));

    state = WALK;

    health = 100;

    currentFrame = 0;
    animationTimer = 0.f;
    frameDuration = 0.1f;
}

bool Enemy::isAlive() const
{
    return state != DEAD;
}

void Enemy::takeDamage(int damage)
{
    if (state == DEAD)
        return;

    health -= damage;

    if (health <= 0)
    {
        state = DEAD;
        currentFrame = 20;
    }
    else
    {
        state = HURT;
        currentFrame = 16;
    }
}

void Enemy::update(float deltaTime)
{
    animationTimer += deltaTime;

    if (animationTimer >= frameDuration)
    {
        animationTimer = 0.f;
        updateAnimation();
    }
}

void Enemy::updateAnimation()
{
    switch (state)
    {
        case WALK:
        {
            currentFrame++;

            if (currentFrame > 7)
                currentFrame = 0;

            break;
        }

        case ATTACK:
        {
            currentFrame++;

            if (currentFrame > 15)
                currentFrame = 8;

            break;
        }

        case HURT:
        {
            currentFrame++;

            if (currentFrame > 19)
            {
                currentFrame = 0;
                state = WALK;
            }

            break;
        }

        case DEAD:
        {
            if (currentFrame < 23)
                currentFrame++;

            break;
        }

        default:
            break;
    }

    sprite.setTextureRect(
        sf::IntRect(
            currentFrame * 80,
            0,
            80,
            80
        )
    );
}