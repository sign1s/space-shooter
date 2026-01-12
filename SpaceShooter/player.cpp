#include "player.h"
#include <vector>   
#include <SFML/Audio.hpp>
#include <Windows.h>

enum controls { RIGHT = 0, LEFT, SHOOT }; //Space

Player::Player(Texture* texture, Texture* fireTexture, //Texture* fireTexture
	Vector2u windowBounds,
	int RIGHT, int LEFT, int SHOOT)
	:level(1), exp(0), expNext(100),
	hp(10), hpMax(10), damage(1), damageMax(2),
	score(0) //initiation list
{
	//Textures and Sprites
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	this->fireTexture = fireTexture; //pocsik
	//sounds
	if (!dragonHitBuffer.loadFromFile("Sounds/dragonHitSound.wav"))
		MessageBoxA(nullptr, "Nie zaladowano dzwieku trafienia smoka", "BLAD", MB_OK);
	dragonHitSound.setBuffer(dragonHitBuffer);

	this->sprite.setScale(0.15f, 0.15f); //ja potzrebuje bo moj jest wielki, SKALA
	//moj jest juz maly wiec tego nie potrzebuje xd
	this->sprite.setPosition(Vector2f((windowBounds.x - sprite.getGlobalBounds().width) / 2.f,
		windowBounds.y - sprite.getGlobalBounds().height));

	//Timers //pocisk
	this->shootTimerMax = 20;
	this->shootTimer = this->shootTimerMax;

	//Controls
	this->controls[controls::RIGHT] = RIGHT;
	this->controls[controls::LEFT] = LEFT;
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


	//SHOOT //koles stworzyl tutaj osobna funkcje combat, na razie wydaje sie zbedna (04 - 18:42)
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT]))
		&& this->shootTimer >= this->shootTimerMax) {
		this->fires.push_back(Fire(fireTexture, this->playerCenter));
		this->shootTimer = 0; // Reset timer!
	}

}

void Player::updateWindowBoundsCollision(Vector2u windowBounds)
{
	/*Vector2f playerPos = this->sprite.getPosition();
	float playerX = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width;*/

	//troche to zmienilam
	if (this->sprite.getPosition().x < 0.f)
		this->sprite.setPosition(0.f, this->sprite.getPosition().y);
	else if (this->sprite.getPosition().x + this->sprite.getGlobalBounds().width > windowBounds.x)
		this->sprite.setPosition(windowBounds.x - sprite.getGlobalBounds().width, this->sprite.getPosition().y);
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

void Player::Update(Vector2u windowBounds) //Vector2u windowBounds
{
	//Update shootTimer //pocisk
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;

	//Update position //pocisk
	this->playerCenter.x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
	this->playerCenter.y = sprite.getPosition().y;

	this->Movement();

	this->updateWindowBoundsCollision(windowBounds);

	//pocsik, KOLES ZROBIL TO W GAME::UPDATE (USUWANIE TYCH POCISKOW), ALE CHYBA LEPIEJ MIEC TO TUTAJ
	for (size_t i = 0; i < this->fires.size(); ) {
		this->fires[i].update();

		//Enemy collision check



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
