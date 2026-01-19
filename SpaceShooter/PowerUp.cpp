#include "PowerUp.h"
#include "Player.h"

PowerUp::PowerUp(PowerUpType t, const sf::Vector2f& pos, sf::Texture& tex)
    : type(t)
{
    sprite.setTexture(tex);

    sprite.setOrigin(
        sprite.getTexture()->getSize().x / 2.f,
        sprite.getTexture()->getSize().y / 2.f
    );

    switch (type)
    {
    case PowerUpType::Heart:
        sprite.setScale(0.07f, 0.07f);
        break;
    case PowerUpType::Sword:
        sprite.setScale(0.1f, 0.1f);
        break;
    case PowerUpType::Lightning:
        sprite.setScale(0.1f, 0.1f);
        break;
    case PowerUpType::Pierce:
        sprite.setScale(0.1f, 0.1f);
    }

    sprite.setPosition(pos);
}

void PowerUp::update(float dt)
{
    sprite.move(0.f, speed * dt);
}

void PowerUp::apply(Player& player)
{
    switch (type)
    {
    case PowerUpType::Heart:
        player.addHP(1);
        break;

    case PowerUpType::Lightning:
        player.addLightning();
        break;

    case PowerUpType::Sword:
        player.enableTripleShot();
        break;
    case PowerUpType::Pierce:
        player.enablePiercing();
        break;
    }
}
