#include <SFML/Graphics.hpp>
#include "fire.h"


Fire::Fire(Texture* texture, Vector2f position, Vector2f maxVelocity,Vector2f scale) {

	this->damage = 1;
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->maxVelocity = maxVelocity;

	this->sprite.setScale(Vector2f(scale));
	float angle = std::atan2(maxVelocity.y, maxVelocity.x) * 180.f / 3.14159265f;
	this->sprite.setRotation(angle);
	this->sprite.setPosition(Vector2f(position.x - this->sprite.getGlobalBounds().width / 2.f,
		position.y - this->sprite.getGlobalBounds().height / 2.f));
		
		}

void Fire::update() {

	this->movement();
}

void Fire::movement() {

	this->sprite.move(this->maxVelocity.x, this->maxVelocity.y);
}

void Fire::draw(RenderTarget& target) {
	//if (!fireTex.loadFromFile("Textures/fire_prev.png"))
	//	throw "Nie udalo sie wczytac obrazu!";

	//fire.setTexture(fireTex);
	//fire.setScale(Vector2f(0.15f, 0.15f));

	target.draw(this->sprite);
}

int Fire::getDamage() const
{
	return this->damage;
}
