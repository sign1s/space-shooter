#pragma once

#include"fire.h"
class Enemy
{
public:
	enum class EnemyType
	{
		Harpy,
		Raven,
		Argus
	};
private:
	Texture* texture;
	Texture* projectileTexture;
	Sprite sprite;

	Vector2f direction;
	float speed;

	EnemyType type;
	int hp;
	int hpMax;
	int damageMax;
	int damageMin;

	float elapsedTime = 0.f;   // licznik czasu od spawn
	float amplitude = 50.f;    // dla ruchu sinusoidalnego Argusa
	float frequency = 2.f;     // dla ruchu sinusoidalnego Argusa
	float startX = 0.f;        // zapamiêtanie startowej pozycji X
	Vector2f scale;//pocisku mam nadzieje

	Vector2f projectileScale = Vector2f(0.05f, 0.2f); // domyœlna skala dla pióra Harpy
	float projectileSpeed = 5.f; // domyœlna prêdkoœæ pocisku Harpy

	float shootTimer = 0.f;         // licznik czasu miêdzy strza³ami
	float shootTimerMax = 60.f;     // strza³ co 60 klatek (1 sekunda przy 60 FPS)
	float stopY = 0.f;              // wysokoœæ, na której Harpy przestaje lecieæ w dó³

	bool reachedStopY = false;

	std::vector<Fire> fires;  // pociski wroga
public:

	Enemy(Texture* texture, Vector2u windowBounds,
		Vector2f position, Vector2f direction,
		Vector2f scale, EnemyType type, Texture* projectileTexture = nullptr,
		int hpMax = 0, int damageMax = 0, int damageMin = 0);


	virtual ~Enemy();


	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const bool isDead()const { return this->hp <= 0; }
	inline const FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f getPosition()const { return this->sprite.getPosition(); }
	EnemyType getEnemyType() const { return this->type; };

	std::vector<Fire>& getFires() { return fires; }
	//funkcje
	void takeDamage(int damage);
	void Update(const sf::Vector2f& playerPos);
	void Draw(RenderTarget& target);
};