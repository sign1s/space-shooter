#pragma once

#include <iostream>
#include <vector> //vector
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "fire.h" //to do  playera trzeba include

using namespace sf;

class Player
{
private:

	Vector2f playerCenter; //pocisk

	//pocisk
	int shootTimer;
	int shootTimerMax;

	Texture* texture;
	Sprite sprite;

	//pocisk
	Texture* fireTexture;
	std::vector<Fire> fires;

	//sounds
	sf::SoundBuffer dragonHitBuffer;
	sf::Sound dragonHitSound;

	int controls[3];//left,right,shoot

	int level;
	int exp;
	int expNext;

	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

public:

	Player(Texture* texture, Texture* fireTexture, //Texture* fireTexture
		int RIGHT = Keyboard::Right,
		int LEFT = Keyboard::Left,
		int SHOOT = Keyboard::Space);//i shoot
	virtual ~Player();


	//Accessors
	inline std::vector<Fire>& getFires() { return this->fires; }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const String getHPasString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	sf::FloatRect getGlobalBounds() const;//cos do hitboxow
	//Functions
	void Movement();
	void updateWindowBoundsCollision(Vector2u windowBounds);
	void Update(Vector2u windowBounds); //Vector2u windowBounds
	void Draw(RenderTarget& target);
	void takeDamage(int damage);
	int getHP() const;



};
#pragma once
