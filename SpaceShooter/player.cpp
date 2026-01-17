#include "player.h"
#include <vector>   
#include <SFML/Audio.hpp>
#include <Windows.h>

enum controls { RIGHT = 0, LEFT, UP, DOWN, SHOOT }; 

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
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
		this->sprite.move(10.0f, 0.0f);

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
		this->sprite.move(-10.0f, 0.0f);

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
		this->sprite.move(0.0f, -10.0f);

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
		this->sprite.move(0.0f, 10.0f);

	//SHOOT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax) {
		this->fires.push_back(Fire(fireTexture, this->playerCenter));
		this->shootTimer = 0; // Reset timer!
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

void Player::addScore(int value)
{
	this->score += value;
}

void Player::addEXP(int value)
{
	this->exp += value;
}


void Player::addGold(int value) {

	this->gold += value;
}

void Player::setScore(int value)
{
	this->score = value;
}

void Player::setHP(int value)
{
	this->hp = value;
}

void Player::setEXP(int value)
{
	this->exp = value;
}

void Player::setGOLD(int value)
{
	this->gold = value;
}

void Player::setLevel(int value)
{
	this->level = value;
	this->UpdateStats();
}

void Player::UpdateStats()
{
	this->expNext = 20 + (this->level * 20);

	this->damage = this->level;

	this->hpMax = 1 + this->level * 5;

	this->levelBonus = 10 * this->level;
}

void Player::LevelUp()
{
	this->level++;
	this->UpdateStats();

	this->exp = 0;
	this->hp = this->hpMax;

}

void Player::Update(Vector2u windowBounds)
{
	//Update shootTimer //pocisk
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;

	//Update position //pocisk
	this->playerCenter.x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
	this->playerCenter.y = sprite.getPosition().y;

	this->Movement();

	this->updateWindowBoundsCollision(windowBounds);

	//pocsik
	for (size_t i = 0; i < this->fires.size(); ) {
		this->fires[i].update();

		//Check bounds
		if (this->fires[i].getPosition().y + this->fires[i].getGlobalBounds().height < 0.f) {
			this->fires.erase(this->fires.begin() + i);
		}
		else {
			i++;
		}


	}
}

void Player::Draw(RenderTarget& target)
{
	//pocisk
	for (size_t i = 0; i < this->fires.size(); i++) {
		this->fires[i].draw(target);
	}

	target.draw(this->sprite);
}

