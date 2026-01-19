#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <cstdlib>   

using namespace sf;

class Fire {

	Texture* texture;
	Sprite sprite;
	Vector2f maxVelocity; //speed of bullet

private:
	int damage;

public:
	Fire(Texture* texture, Vector2f position, Vector2f maxVelocity = Vector2f(0.f, -10.f), 
Vector2f scale = Vector2f(0.15f, 0.15f), int damage = 1);
	//virtual ~Fire();
	


	//Accessors
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	int getDamage() const;

	//Functions
	void movement();
	void update();
	void draw(RenderTarget& target);
	void setColor(const sf::Color& color) { sprite.setColor(color); }
	sf::Sprite& getSprite() { return sprite; }
};
