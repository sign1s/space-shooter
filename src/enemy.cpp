#include "enemy.h"
#include <iostream> 
enum eTypes { MOVELEFT = 0,FOLLOW, FOLLOWFAST, FOLLOWSHOOT, FOLLOWFASTSHOOT};

Enemy::Enemy(Texture* texture, Vector2u windowBounds,
	Vector2f position, Vector2f direction,
	Vector2f scale, int type, int hpMax, int damageMax, int damageMin)

{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	this->sprite.setScale(scale);

	//this->sprite.setPosition(windowBounds.x + this->sprite.getGlobalBounds().width, rand() % windowBounds.y);
	this->sprite.setPosition(position);
	
	this->type = type;

	this->direction = direction;
	this->speed = 1.f;

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->damageMax = damageMax;
	this->damageMin = damageMin;
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

void::Enemy::Update()
{
	// Wypisuj pozycjê co klatkê (tymczasowo)
	 //std::cout << "Enemy X: " << this->sprite.getPosition().x << " Y: " << this->sprite.getPosition().y << std::endl;
	switch (this->type)
	{
	case 0:
			
			//this->sprite.move(0.f, 10.f);
			this->sprite.move(this->direction.x * this->speed, this->direction.y * this->speed);
			break;

		default:

			break;
	}
}

void Enemy::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}



