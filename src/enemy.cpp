#include "enemy.h"
#include <iostream> 
#include <cmath>
enum eTypes { MOVELEFT = 0,FOLLOW, FOLLOWFAST, FOLLOWSHOOT, FOLLOWFASTSHOOT};

Enemy::Enemy(Texture* texture, Vector2u windowBounds,
    Vector2f position, Vector2f direction,
    Vector2f scale, EnemyType type, Texture* projectileTexture,
    int hpMax, int damageMax, int damageMin)
    :texture(texture), projectileTexture(projectileTexture), type(type),
    direction(direction)
{
    sprite.setTexture(*texture);
    sprite.setScale(scale);
    sprite.setPosition(position);

    this->texture = texture;
    this->sprite.setTexture(*texture);
    this->sprite.setScale(scale);
    this->sprite.setPosition(position);

    this->projectileTexture = projectileTexture;

    this->type = type;
    this->direction = direction;

    this->scale = scale;
    this->sprite.setScale(this->scale);

    // Statystyki domyœlne w zale¿noœci od typu
    switch (type)
    {
    case EnemyType::Harpy:
        this->shootTimerMax = 60.f;
        this->projectileSpeed = 2.f;
        this->speed = 1.f;
        this->stopY = windowBounds.y * 0.2f;
        this->hpMax = (hpMax > 0) ? hpMax : 3;
        this->damageMax = (damageMax > 0) ? damageMax : 2;
        this->damageMin = (damageMin > 0) ? damageMin : 1;
        this->scale = Vector2f(0.03f, 0.06f);//pocisk
        break;

    case EnemyType::Raven:
        this->speed = 2.f;
        this->hpMax = (hpMax > 0) ? hpMax : 2;
        this->damageMax = (damageMax > 0) ? damageMax : 3;
        this->damageMin = (damageMin > 0) ? damageMin : 2;
        break;

    case EnemyType::Argus:
        this->speed = 1.5f;
        this->startX = position.x;
        this->amplitude = 10.f;
        this->frequency = 2.f;
        this->hpMax = (hpMax > 0) ? hpMax : 3;
        this->damageMax = (damageMax > 0) ? damageMax : 2;
        this->damageMin = (damageMin > 0) ? damageMin : 1;
        break;
    }

    this->hp = this->hpMax;
}
	
Enemy::~Enemy()
{

}

void Enemy::takeDamage(int damage)
{
	this->hp -= damage;

	if (this->hp <= 0)
		this->hp = 0;
}

void Enemy::Update(const sf::Vector2f& playerPos)
	// Wypisuj pozycjê co klatkê (tymczasowo)
	 //std::cout << "Enemy X: " << this->sprite.getPosition().x << " Y: " << this->sprite.getPosition().y << std::endl;
{
	
    if (type == EnemyType::Harpy)
    {
        // Zatrzymaj siê na 60% wysokoœci ekranu
        if (sprite.getPosition().y < stopY)
        {
            sprite.move(0.f, speed);
        }
        else
        {
            // Liczymy czas do strza³u
            shootTimer++;
            if (shootTimer >= shootTimerMax)
            {
                shootTimer = 0; // reset licznika

                // Tworzymy pocisk w kierunku gracza
                sf::Vector2f dir = playerPos - sprite.getPosition();
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0.f) dir /= len;
                Vector2f spawnPos = sprite.getPosition();
                spawnPos.x += sprite.getGlobalBounds().width / 2.f;
                spawnPos.y += sprite.getGlobalBounds().height / 2.f;
              
                Vector2f offset(sprite.getGlobalBounds().width * 1.1f, sprite.getGlobalBounds().height * 1.1f);
                fires.emplace_back(projectileTexture, sprite.getPosition() + offset, dir * projectileSpeed,scale);// dir predkosc
                
            }
        }
    }
	else if (type == EnemyType::Raven)
	{
		sf::Vector2f toPlayer = playerPos - sprite.getPosition();
		float len = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
		if (len != 0.f) toPlayer /= len;
		sprite.move(toPlayer * speed * 2.f);
	}
    else if (type == EnemyType::Argus)
    {
        elapsedTime += 1.f / 60.f; // lub deltaTime jeœli masz
        sf::Vector2f toPlayer = playerPos - sprite.getPosition();
        float len = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
        if (len != 0.f) toPlayer /= len;

        // Y idzie w kierunku gracza
        float moveY = toPlayer.y * speed;

        // X pod¹¿a za graczem + sinus
        float moveX = toPlayer.x * speed + amplitude * std::sin(frequency * elapsedTime);

        sprite.move(moveX, moveY);
    }
}
void Enemy::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}



