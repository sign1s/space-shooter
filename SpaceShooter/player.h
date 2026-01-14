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

	int controls[5];//left,right,up,down,shoot

	int level;
	int exp;
	int expNext;

	int hp;
	float hpMax;

	int damage;
	int damageMax;

	//int gold;

	int score;

	int levelBonus;//dodatkowe punkty do score z osiagnieciem nowego levelu

public:

	Player(Texture* texture, Texture* fireTexture, //Texture* fireTexture
		Vector2u windowBounds,
		int RIGHT = Keyboard::Right,
		int LEFT = Keyboard::Left,
		int UP = Keyboard::Up,
		int DOWN = Keyboard::Down,
		int SHOOT = Keyboard::Space);//i shoot
	virtual ~Player();


	//Accessors
	inline std::vector<Fire>& getFires() { return this->fires; }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const String getHPasString()const { return std::to_string(this->hp) + "/" + std::to_string(static_cast<int>(this->hpMax)); }
	inline const String getLVLasString()const { return std::to_string(this->level); }
	inline const String getEXPasString()const { return std::to_string(this->exp) + "/" + std::to_string(static_cast<int>(this->expNext)); }
	//inline const String getGOLDasString()const { return std::to_string(this->gold); }
	inline const String getSCOREasString()const { return std::to_string(this->score); }
	sf::FloatRect getGlobalBounds() const;//cos do hitboxow
	//Functions
	void Movement();
	void updateWindowBoundsCollision(Vector2u windowBounds);
	void Update(Vector2u windowBounds); //Vector2u windowBounds
	void Draw(RenderTarget& target);
	void takeDamage(int damage);
	int getHP() const;
	float getHPmax() const;
	void addScore(int value);
	void addEXP(int value);
	int getEXP() const;
	float getEXPnext() const;
	int getlevelBonus() const;
	//int addGold(); //niewiadomo jeszcze czy bedzie zloto
	void levelUP();


};
#pragma once
