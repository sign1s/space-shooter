#pragma once

#include"fire.h"
class Enemy
{
	enum class EnemyType
	{
		Harpy,
		Raven
};
private:
	Texture* texture;
	Sprite sprite;
	
	Vector2f direction;
	float speed;


	int type;
	int hp;
	int hpMax;
	int damageMax;
	int damageMin;

public:
	Enemy(Texture* texture, Vector2u windowBounds,
		Vector2f position, Vector2f direction,
		Vector2f scale, int type,
		int hpMax, int damageMax, int damageMin);

	virtual ~Enemy();


	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const bool isDead()const { return this->hp <= 0; }
	inline const FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f getPosition()const { return this->sprite.getPosition(); }


	//funkcje
	void takeDamage(int damage);
	void Update();
	void Draw(RenderTarget& target);
};