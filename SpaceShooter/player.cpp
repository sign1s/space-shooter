#include "player.h"
#include <vector>   
#include <SFML/Audio.hpp>
#include <Windows.h>
#
enum controls { RIGHT = 0, LEFT, UP, DOWN, SHOOT, DASH }; 

Player::Player(const DragonProfile& profile, Texture* texture, Texture* fireTexture,
	Vector2u windowBounds,
	int RIGHT, int LEFT, int UP, int DOWN, int SHOOT,int DASH)
	:level(1), exp(0), expNext(60), 
	hpMax(profile.hpMax), damage(profile.damage), damageMax(profile.damage),
	shootTimerMax(profile.shootTimerMax),
	score(0), levelBonus(10), gold(0) //initiation list
{
	this->hp = this->hpMax;

	this->movementSpeed = profile.speed;

	// dash bar
	dashCooldownMax = dashCooldown;

	dashBar.setSize(Vector2f(100.f, 5.f));
	dashBar.setFillColor(sf::Color::White);
	dashBar.setPosition(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height + 5.f);

	dashBarInside.setSize(Vector2f(100.f, 5.f));
	dashBarInside.setFillColor(sf::Color::Yellow);
	dashBarInside.setPosition(dashBar.getPosition());


	//Textures and Sprites
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	this->fireTexture = fireTexture; //pocsik
	//sounds
	if (!dragonHitBuffer.loadFromFile("Sounds/dragonHitSound.wav"))
		MessageBoxA(nullptr, "Nie zaladowano dzwieku trafienia smoka", "BLAD", MB_OK);
	dragonHitSound.setBuffer(dragonHitBuffer);

	shootBuffer.loadFromFile("Sounds/shootSound.wav");
	shootSound.setBuffer(shootBuffer);

	this->sprite.setScale(profile.scale, profile.scale);
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
	//Dash
	this->controls[5] = DASH;

	for (int i = 0; i < 6; i++)
		originalControls[i] = controls[i];

}

Player::~Player()
{

}

void Player::restoreControls()
{
	for (int ij = 0; ij < 6; ij++)
		controls[ij] = originalControls[ij];
}

void Player::shuffleControls()
{
	std::vector<int> tempControls(originalControls, originalControls + 6); // teraz 6 klawiszy

	// Fisher-Yates shuffle
	for (int i = 5; i > 0; --i)
	{
		int j = rand() % (i + 1);
		std::swap(tempControls[i], tempControls[j]);
	}

	for (int i = 0; i < 6; i++)
		controls[i] = tempControls[i];

	controlsShuffled = true;
	shuffleTimer = 0.f;
}


void Player::Movement()
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[RIGHT])))
		sprite.move(this->movementSpeed * speedMultiplier, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[LEFT])))
		sprite.move(-this->movementSpeed * speedMultiplier, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[UP])))
		sprite.move(0.f, -this->movementSpeed * speedMultiplier);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[DOWN])))
		sprite.move(0.f, this->movementSpeed * speedMultiplier);

	//SHOOT
	int shots = tripleShotActive ? 3 : 1;

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[SHOOT])) &&
		shootTimer >= shootTimerMax)
	{
		shootSound.play();
		for (int i = 0; i < shots; i++)
		{
			float offsetX = 0.f;
			if (shots > 1)
				offsetX = (i - (shots - 1) / 2.f) * 5.f;

			fires.emplace_back(
				fireTexture,
				playerCenter,
				Vector2f(offsetX, -10.f),
				Vector2f(0.15f, 0.15f),
				damageMax
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

	//else if (this->sprite.getPosition().y + 2 * this->sprite.getGlobalBounds().height > windowBounds.y)
	//	this->sprite.setPosition(this->sprite.getPosition().x, windowBounds.y - 2 * sprite.getGlobalBounds().height);
	else if (this->sprite.getPosition().y + sprite.getGlobalBounds().height > windowBounds.y + sprite.getGlobalBounds().height / 2.f)
		this->sprite.setPosition(this->sprite.getPosition().x, windowBounds.y - sprite.getGlobalBounds().height / 2.f);



	else if (this->sprite.getPosition().y < 300.f)
		this->sprite.setPosition(this->sprite.getPosition().x, 300.f);
}
//to
void Player::takeDamage(int damage)
{
	if (isInvincible)
		return;

	this->hp -= damage;
	dragonHitSound.play();

	isInvincible = true;
	invincibilityTimer = 0.f;

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
	if (value > this->hpMax) this->hp = this->hpMax;
	else this->hp = value; 
}

void Player::setEXP(int value)
{
	this->exp = value;
}

void Player::setGOLD(int value)
{
	this->gold = value;
}

void Player::StartDash(const Vector2f& dir)
{
	if (!isDashing && dashCooldownTimer <= 0.f)
	{
		isDashing = true;
		dashDirection = dir;
		dashTimer = 0.f;
		dashCooldownTimer = dashCooldown;
		isInvincible = true;
		invincibilityTimer = 0.f;
	}
}

void Player::setLevel(int value)
{
	this->level = value;
	this->UpdateStats();
}

void Player::UpdateStats()
{
	this->expNext = 40 + (this->level * 20);
	this->damage = this->level;
	this->hpMax += level/2 - (level-1)/2;
	this->levelBonus = 10 * this->level;
}

void Player::LevelUp()
{
	this->level++;
	this->UpdateStats();

	this->exp = 0;
	
	if (this->hp + 0.1f * hp > this->hpMax)
		this->hp = this->hpMax;
	else
		this->hp += 0.1f * hp;
		

}

void Player::Update(Vector2u windowBounds)
{
	float deltaTime = deltaClock.restart().asSeconds(); // czas od ostatniego restartu
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
	/*if (isWraithEffectActive)
	{
		wraithEffectTimer += deltaTime;


		// miganie na czarno
		float blink = std::sin(wraithEffectTimer * 5.f); // wolniejsze miganie
		int alpha = (blink > 0) ? 100 : 255;
		sprite.setColor(sf::Color(0, 0, 0, alpha));

		if (wraithEffectTimer >= wraithEffectDuration)
		{
			// koniec efektu
			isWraithEffectActive = false;
			sprite.setColor(sf::Color::White);
			restoreControls(); // przywróæ klawisze
		}
	}*/
	if (isWraithEffectActive)
	{
		wraithEffectTimer += deltaTime;

		// Kontrolowanie zamiany klawiszy
		if (wraithEffectTimer >= shuffleDuration)
		{
			restoreControls();
			isWraithEffectActive = false;
			wraithEffectTimer = 0.f;
		}
	}
	if (isWraithEffectActive)
	{
		float blink = std::sin(wraithEffectTimer * 5.f);
		int alpha = (blink > 0) ? 100 : 255;
		sprite.setColor(sf::Color(0, 0, 0, alpha));
	}
	else
		sprite.setColor(sf::Color::White);


	// --- Aktualizacja paska dash ---
	float percent = 1.f - std::min(dashCooldownTimer / dashCooldownMax, 1.f); // 0..1
	dashBarInside.setSize(Vector2f(100.f * percent, dashBarInside.getSize().y));

	// ustawienie pozycji pod graczem (jeœli sprite siê porusza)
	float barWidth = dashBar.getSize().x;
	float spriteCenterX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;

	dashBar.setPosition(spriteCenterX - barWidth / 2.f,
		sprite.getPosition().y + sprite.getGlobalBounds().height + 5.f);

	dashBarInside.setPosition(dashBar.getPosition());



	if (controlsShuffled)
	{
		shuffleTimer += deltaTime;
		if (shuffleTimer >= shuffleDuration)
		{
			for (int i = 0; i < 6; i++)
				controls[i] = originalControls[i];

			controlsShuffled = false;
		}
	}


	//Update shootTimer //pocisk
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;

	if (dashCooldownTimer > 0.f)
		dashCooldownTimer -= deltaTime;

	//Update position //pocisk
	this->playerCenter.x = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
	this->playerCenter.y = sprite.getPosition().y;

	

	

	// --- Input Dash ---
	static bool dashKeyHeld = false;
	if (Keyboard::isKeyPressed(Keyboard::LShift))
	{
		if (!dashKeyHeld)
		{
			sf::Vector2f dashDir(0.f, 0.f);

			if (Keyboard::isKeyPressed(Keyboard::Up))    dashDir.y -= 1.f;
			if (Keyboard::isKeyPressed(Keyboard::Down))  dashDir.y += 1.f;
			if (Keyboard::isKeyPressed(Keyboard::Left))  dashDir.x -= 1.f;
			if (Keyboard::isKeyPressed(Keyboard::Right)) dashDir.x += 1.f;

			if (dashDir.x != 0.f || dashDir.y != 0.f)
			{
				float len = std::sqrt(dashDir.x * dashDir.x + dashDir.y * dashDir.y);
				dashDir /= len;

				StartDash(dashDir); // w³¹cz dash i iframe
			}

			dashKeyHeld = true;
		}
	}
	else
	{
		dashKeyHeld = false;
	}

	// --- W³aœciwy Dash ---
	if (isDashing)
	{
		sprite.move(dashDirection * dashSpeed * deltaTime); // poruszamy gracza
		dashTimer += deltaTime;

		if (dashTimer >= dashTime)
		{
			isDashing = false;
			dashTimer = 0.f;

			// Krótki iframe po dash (opcjonalnie)
			isInvincible = true;
			invincibilityTimer = 0.f;
		}
	}
	else
	{
		// Normalny ruch (WASD/strza³ki) dzia³a tylko, gdy nie dashujesz
		Movement();
	}

	// --- I-frames po obra¿eniach ---
	if (isInvincible)
	{
		invincibilityTimer += deltaTime;
		if (invincibilityTimer >= invincibilityTime)
		{
			isInvincible = false;
			invincibilityTimer = 0.f;
		}

		if (!getWraithEffectActive()) // u¿ywamy getter
		{
			int alpha = static_cast<int>(std::sin(invincibilityTimer * 40.f) * 127.f + 128.f);
			sprite.setColor(sf::Color(255, 255, 255, alpha));
		}
	}
	else if (!getWraithEffectActive())
	{
		sprite.setColor(sf::Color::White);
	}





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
	target.draw(dashBar);
	target.draw(dashBarInside);

	sf::FloatRect bounds = sprite.getGlobalBounds();
	sf::RectangleShape rect;
	rect.setPosition(bounds.left, bounds.top);
	rect.setSize({ bounds.width, bounds.height });
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(2.f);

	target.draw(rect);

}
void Player::addLightning()
{
	lightningActive = true;
	speedMultiplier = 2.f;        // ~70% szybciej
	lightningTimer = lightningDurationPerStack; // reset czasu
}

void Player::addHP(int value)
{
	this->hp += this->hpMax / 10;
	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}
void Player::enableTripleShot()
{
	tripleShotActive = true;
	tripleShotTimer = 0;
}


void Player::enablePiercing()
{
	piercingActive = true;
	piercingTimer = piercingDuration;
}


