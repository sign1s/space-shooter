#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "fire.h"
#include "DragonProfile.h"


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

	int gold;

	int score;

	int levelBonus;//dodatkowe punkty do score z osiagnieciem nowego levelu

public:

	Player(const DragonProfile& profile, Texture* texture, Texture* fireTexture,
		Vector2u windowBounds,
		int RIGHT = Keyboard::Right,
		int LEFT = Keyboard::Left,
		int UP = Keyboard::Up,
		int DOWN = Keyboard::Down,
		int SHOOT = Keyboard::Space);
	virtual ~Player();


	//Accessors
	inline std::vector<Fire>& getFires() { return this->fires; }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const String getHPasString()const { return std::to_string(this->hp) + "/" + std::to_string(static_cast<int>(this->hpMax)); }
	inline const String getLVLasString()const { return std::to_string(this->level); }
	inline const String getEXPasString()const { return std::to_string(this->exp) + " / " + std::to_string(static_cast<int>(this->expNext)); }
	inline const String getGOLDasString()const { return std::to_string(this->gold); }
	inline const String getSCOREasString()const { return std::to_string(this->score); }
	sf::FloatRect getGlobalBounds() const;//cos do hitboxow


	//Functions
	void Movement();
	void updateWindowBoundsCollision(Vector2u windowBounds);
	void Update(Vector2u windowBounds);
	void Draw(RenderTarget& target);
	void takeDamage(int damage);
	void addScore(int value);
	void addEXP(int value);
	void addGold(int value);
	void UpdateStats();
	void LevelUp();

	//Gettery
	int getLevel()const { return level; }

	int getHP() const { return hp; }
	float getHPmax() const { return hpMax; }

	int getEXP() const { return exp; }
	float getEXPnext() const { return expNext; }

	int getlevelBonus() const { return levelBonus; }

	int getGold() const { return gold; }

	int getDamage() const { return damage; }

	int getScore() const { return score; }


	//Settery
	void setLevel(int value);
	void setHP(int value);
	void setEXP(int value);
	void setGOLD(int value);
	void setScore(int value);

};

