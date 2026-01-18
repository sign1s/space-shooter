#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "fire.h"
#include "DragonProfile.h"


using namespace sf;

class Player
{
private:
	sf::Clock deltaClock;
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

	int controls[6];//left,right,up,down,shoot,dash
	int originalControls[6];

	// Dash bar
	sf::RectangleShape dashBar;
	sf::RectangleShape dashBarInside;
	float dashCooldownMax; // przechowuje wartoœæ cooldownu, ¿eby normalizowaæ pasek


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

	// --- Dash ---
	bool isDashing = false;
	float dashSpeed = 2500.f;          // prêdkoœæ podczas dash
	float dashTime = 0.1f;            // czas trwania dash w sekundach
	float dashTimer = 0.f;
	Vector2f dashDirection;
	float dashCooldown = 20.f;       // czas miêdzy dashami
	float dashCooldownTimer = 0.f;

	// --- I-frames ---
	bool isInvincible = false;
	float invincibilityTime = 1.f;    // czas ochrony po obra¿eniach
	float invincibilityTimer = 0.f;

	bool controlsShuffled = false;
	float shuffleTimer = 0.f;
	float shuffleDuration = 2.f;

	float wraithEffectTimer = 0.f;      // czas trwania efektu
	float wraithEffectDuration = 3.f;   // 3 sekundy
	bool isWraithEffectActive = false; // blokada resetu koloru podczas efektu

public:

	Player(const DragonProfile& profile, Texture* texture, Texture* fireTexture,
		Vector2u windowBounds,
		int RIGHT = Keyboard::Right,
		int LEFT = Keyboard::Left,
		int UP = Keyboard::Up,
		int DOWN = Keyboard::Down,
		int SHOOT = Keyboard::Space,
		int DASH = Keyboard::LShift);
	virtual ~Player();

	// Zmiana przypisanego klawisza dla danej akcji
	void setControl(int actionIndex, Keyboard::Key newKey)
	{
		if (actionIndex >= 0 && actionIndex < 6) // mamy 6 klawiszy: RIGHT, LEFT, UP, DOWN, SHOOT, DASH
		{
			controls[actionIndex] = newKey;
			originalControls[actionIndex] = newKey; // zapisujemy orygina³ do przywracania po shuffle
		}
	}

	// Pobranie aktualnego przypisanego klawisza dla danej akcji
	Keyboard::Key getControl(int actionIndex) const
	{
		if (actionIndex >= 0 && actionIndex < 6)
			return static_cast<Keyboard::Key>(controls[actionIndex]);
		return Keyboard::Unknown;
	}

	//Accessors
	inline std::vector<Fire>& getFires() { return this->fires; }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const String getHPasString()const { return std::to_string(this->hp) + "/" + std::to_string(static_cast<int>(this->hpMax)); }
	inline const String getLVLasString()const { return std::to_string(this->level); }
	inline const String getEXPasString()const { return std::to_string(this->exp) + " / " + std::to_string(static_cast<int>(this->expNext)); }
	inline const String getGOLDasString()const { return std::to_string(this->gold); }
	inline const String getSCOREasString()const { return std::to_string(this->score); }
	sf::FloatRect getGlobalBounds() const;//cos do hitboxow
	void StartDash(const sf::Vector2f& dir);


	//Functions
	void Movement();
	void updateWindowBoundsCollision(Vector2u windowBounds);
	void Update(Vector2u windowBounds);
	void Draw(RenderTarget& target);
	void takeDamage(int damage);
	void addScore(int value);
	void addEXP(int value);
	void addGold(int value);
	void shuffleControls();
	void restoreControls();
	void startWraithEffect()
	{
		isWraithEffectActive = true;
		wraithEffectTimer = 0.f;
	}

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
	Sprite& getSprite() { return this->sprite; }
	void setWraithEffectActive(bool active) { isWraithEffectActive = active; }
	bool getWraithEffectActive() const { return isWraithEffectActive; }


	//update z levelami
	void setLevel(int value);
	void setScore(int value);
	void UpdateStats();

};

