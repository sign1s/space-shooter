#pragma once
#include <SFML/Graphics.hpp>

enum class PowerUpType
{
    Heart,
    Lightning,
    Sword,
    Pierce
};

class Player;

class PowerUp
{
public:
    sf::Sprite sprite;
    PowerUpType type;
    float speed = 150.f;

    PowerUp(PowerUpType t, const sf::Vector2f& pos, sf::Texture& tex);

    void update(float dt);

    void apply(Player& player);
};