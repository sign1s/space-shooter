#pragma once

#include"fire.h"
// Sta³e hitboxów (proporcje wzglêdem sprite)
//h
static constexpr float HARPY_HITBOX_SCALE_H = 0.05f;
static constexpr float RAVEN_HITBOX_SCALE_H = 0.05f;
static constexpr float TANK_HITBOX_SCALE_H = 0.25f;
static constexpr float WRAITH_HITBOX_SCALE_H = 0.05f;
static constexpr float Argus_HITBOX_SCALE_H = 0.05f;
static constexpr float Bayle_HITBOX_SCALE_H = 0.8f;
//w
static constexpr float HARPY_HITBOX_SCALE_W = 0.05f;
static constexpr float RAVEN_HITBOX_SCALE_W = 0.1f;
static constexpr float TANK_HITBOX_SCALE_W = 0.3f;
static constexpr float WRAITH_HITBOX_SCALE_W = 0.05f;
static constexpr float Argus_HITBOX_SCALE_W = 0.05f;
static constexpr float Bayle_HITBOX_SCALE_W = 0.35f;


class Enemy
{
public:
	enum class EnemyType
	{
		Harpy,
		Raven,
		Argus,
		Wraith,
		Tank,
		BossBayle
	};
	enum class BossAttackType {
		None,
		VerticalShot,  // pociski w dó³
		CrossShot,     // pociski w krzy¿
		SpiralShot     // pociski wiruj¹ce
	};
private:


	Texture* texture;
	Texture* projectileTexture;
	Sprite sprite;

	sf::RectangleShape hitbox;

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

	// Animacja Bayle'a'
	sf::IntRect animRect;
	int currentFrame = 0;
	int frameCount = 4;
	float animTimer = 0.f;
	float animSpeed = 0.15f;

	BossAttackType currentAttack = BossAttackType::None;
	bool isAttacking = false;      // czy Bayle jest w trakcie ataku
	float attackTimer = 0.f;       // licznik dla aktualnego ataku
	float attackCooldown = 3.f;    // losowy cooldown miêdzy atakami

	Vector2f startPos;             // miejsce startowe Bayle'a (do powrotu po dash)



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
	const sf::RectangleShape& getHitbox() const { return hitbox; }
	

	std::vector<Fire>& getFires() { return fires; }
	//funkcje
	void updateRotation(const Vector2f& targetPosition);
	void takeDamage(int damage);
	void Update(const sf::Vector2f& playerPos, float dt);
	void Draw(RenderTarget& target);
	void updateAnimation(float dt);
	void handleVerticalShot(float dt, const sf::Vector2f& playerPos);
	void handleCrossShot(float dt);
	void handleSpiralShot(float dt);
};