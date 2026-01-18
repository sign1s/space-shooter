#include "player.h"
#include <vector>   
#include <SFML/Audio.hpp>
#include <Windows.h>

enum controls { RIGHT = 0, LEFT, UP, DOWN, SHOOT }; //Space

Player::Player(const DragonProfile& profile, Texture* texture, Texture* fireTexture,
	Vector2u windowBounds,
	int RIGHT, int LEFT, int UP, int DOWN, int SHOOT)
	:level(1), exp(0), expNext(60), 
	hpMax(profile.hpMax), damage(profile.damage), damageMax(profile.damage + 2),
	shootTimerMax(profile.shootTimerMax),
	score(0), levelBonus(10), gold(0) //initiation list
{
	this->hp = this->hpMax;

	//Textures and Sprites
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	this->fireTexture = fireTexture; //pocsik
	//sounds
	if (!dragonHitBuffer.loadFromFile("Sounds/dragonHitSound.wav"))
		MessageBoxA(nullptr, "Nie zaladowano dzwieku trafienia smoka", "BLAD", MB_OK);
	dragonHitSound.setBuffer(dragonHitBuffer);

	this->sprite.setScale(0.15f, 0.15f); 
	this->sprite.setPosition(Vector2f((windowBounds.x - sprite.getGlobalBounds().width) / 2.f,
		windowBounds.y - sprite.getGlobalBounds().height));

	//Timers //pocisk
	//this->shootTimerMax = 20;
	this->shootTimer = this->shootTimerMax;

	//Controls
	this->controls[controls::RIGHT] = RIGHT;
	this->controls[controls::LEFT] = LEFT;
	this->controls[controls::UP] = UP;
	this->controls[controls::DOWN] = DOWN;
	//SHOOT
	this->controls[controls::SHOOT] = SHOOT;

}

Player::~Player()
{

}

void Player::Movement()
{
	//Ruch gracza
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[RIGHT])))
		sprite.move(baseSpeed * speedMultiplier, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[LEFT])))
		sprite.move(-baseSpeed * speedMultiplier, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[UP])))
		sprite.move(0.f, -baseSpeed * speedMultiplier);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[DOWN])))
		sprite.move(0.f, baseSpeed * speedMultiplier);

	// Strza³
	int shots = tripleShotActive ? 3 : 1;

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[SHOOT])) &&
		shootTimer >= shootTimerMax)
	{
		for (int i = 0; i < shots; i++)
		{
			float offsetX = 0.f;
			if (shots > 1)
				offsetX = (i - (shots - 1) / 2.f) * 5.f; 

			fires.emplace_back(
				fireTexture,
				playerCenter,
				Vector2f(offsetX, -10.f),
				Vector2f(0.15f, 0.15f)
			);
			fires.back().setColor(piercingActive ? sf::Color::Red : sf::Color::White);
		}
		shootTimer = 0.f;
	}
}

void Player::updateWindowBoundsCollision(Vector2u windowBounds)
{
	if (this->sprite.getPosition().x < 0.f)
		this->sprite.setPosition(0.f, this->sprite.getPosition().y);

	else if (this->sprite.getPosition().x + this->sprite.getGlobalBounds().width > windowBounds.x)
		this->sprite.setPosition(windowBounds.x - sprite.getGlobalBounds().width, this->sprite.getPosition().y);

	else if (this->sprite.getPosition().y + 2 * this->sprite.getGlobalBounds().height > windowBounds.y)
		this->sprite.setPosition(this->sprite.getPosition().x, windowBounds.y - 2 * sprite.getGlobalBounds().height);

	else if (this->sprite.getPosition().y < 300.f)
		this->sprite.setPosition(this->sprite.getPosition().x, 300.f);
}
//to
void Player::takeDamage(int damage)
{
	this->hp -= damage;
	dragonHitSound.play();
	if (this->hp < 0)
		this->hp = 0;
}
//to
sf::FloatRect Player::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}
//to
int Player::getHP() const
{
	return this->hp;
}

float Player::getHPmax() const
{
	return this->hpMax;
}

void Player::addScore(int value)
{
	this->score += value;
}

void Player::addEXP(int value)
{
	this->exp += value;
}

int Player::getEXP() const
{
	return this->exp;
}

float Player::getEXPnext() const
{
	return this->expNext;
}

int Player::getlevelBonus() const
{
	return this->levelBonus;
}

void Player::addGold(int value) {

	this->gold += value;
}

void Player::levelUP()
{
	level++;
	expNext += 20;
	exp = 0;
	damage++;
	hpMax += 2;
	hp = hpMax;
	levelBonus += 10;
}

void Player::Update(Vector2u windowBounds)
{
	// TripleShot timer
	if (tripleShotActive)
	{
		tripleShotTimer++;
		if (tripleShotTimer >= tripleShotDuration)
		{
			tripleShotActive = false;
			tripleShotTimer = 0.f;
		}
	}
	//Pierce timer
	if (piercingActive)
	{
		piercingTimer -= 1.f;
		if (piercingTimer <= 0.f)
		{
			piercingActive = false;
			piercingTimer = 0.f;
		}
	}

	// Shoot timer
	shootTimer += 1.f;

	// Aktualizujemy center gracza 
	playerCenter.x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
	playerCenter.y = sprite.getPosition().y;

	// Ruch i kolizja
	Movement();
	updateWindowBoundsCollision(windowBounds);

	// Aktualizacja pocisków
	for (size_t i = 0; i < fires.size(); )
	{
		fires[i].update();
		if (fires[i].getPosition().y + fires[i].getGlobalBounds().height < 0.f)
			fires.erase(fires.begin() + i);
		else
			i++;
	}

	// Lightning effect odliczanie
	if (lightningActive)
	{
		lightningTimer -= 1.f;
		if (lightningTimer <= 0.f)
		{
			lightningActive = false;
			speedMultiplier = 1.f; // reset prêdkoœci
			lightningTimer = 0.f;
		}
	}
}
void Player::addLightning()
{
	lightningActive = true;
	speedMultiplier = 2.f;        // ~70% szybciej
	lightningTimer = lightningDurationPerStack; // reset czasu
}
void Player::addHP(int value)
{
	this->hp += value;
	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}
void Player::enableTripleShot()
{
		tripleShotActive = true;
		tripleShotTimer = 0;
}

void Player::Draw(RenderTarget& target)
{
	//pocisk
	for (size_t i = 0; i < this->fires.size(); i++) {
		this->fires[i].draw(target);
	}

	target.draw(this->sprite);
}
void Player::enablePiercing()
{
	piercingActive = true;
	piercingTimer = piercingDuration; 
}

