#include "Game.h"
#include<iostream>
#include <Windows.h>
#include <SFML/System.hpp>
#include"player.h"
//#include <SFML/Audio.hpp>
#include <filesystem>
Game::Game(RenderWindow* window, const DragonProfile& choosenDragon):totalGold(0)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	//gameover
	this->gameOver = false;

	//init fonts
	this->font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf");

	//init sounds

	if (!deathBuffer.loadFromFile("Sounds/death.wav"))
		MessageBoxA(nullptr, "Nie zaladowano dzwieku smierci", "BLAD", MB_OK);
	deathSound.setBuffer(deathBuffer);

	//background
	if (!backgroundTexture.loadFromFile("Textures/background.png"))
	{
		MessageBoxA(nullptr, "Nie zaladowano tekstury t³a", "BLAD", MB_OK);
	}

	//text timers
	this->textTimerMax = 120.f;
	this->textTimer = 0.f;

	this->infoText.setFont(this->font);
	this->infoText.setCharacterSize(60);
	this->infoText.setFillColor(sf::Color::White);



	//init textures
	//this->playerTexture.loadFromFile("Textures/player/Green_Dragon_overhead.png");
	if (!this->playerTexture.loadFromFile("Textures/player/Green_Dragon_overhead.png"))
	{
		MessageBoxA(nullptr, "NIE ZALADOWANO TEKSTURY", "BLAD", MB_OK);
	}
	//kula ognia
	this->fireTexture.loadFromFile("Textures/bullet/fire_prev.png");
	//pióra
	this->feather_missileTexture.loadFromFile("Textures/bullet/feather_missile.png");
	//pocisk widma
	this->wraithMissile.loadFromFile("Textures/bullet/wraithMissile.png");
	this->bayleMissile.loadFromFile("Textures/bullet/bayleMissile.png");

	this->enemy01Texture.loadFromFile("Textures/enemies/Harpy.png");
	this->enemyRavenTexture.loadFromFile("Textures/enemies/raven.png");
	this->enemyArgusTexture.loadFromFile("Textures/enemies/argus.png");

	//powerups textures
	powerupTextureHeart.loadFromFile("Textures/powerups/heart.png");
	powerupTextureSword.loadFromFile("Textures/powerups/sword.png");
	powerupTextureLightning.loadFromFile("Textures/powerups/lightning.png");
	powerupTexturePierce.loadFromFile("Textures/powerups/pierce.png");

	if (!enemyTankTexture.loadFromFile("Textures/enemies/Tank.png"))
		MessageBoxA(nullptr, "Nie zaladowano tekstury Tanka", "BLAD", MB_OK);
	if (!enemyWraithTexture.loadFromFile("Textures/enemies/wraith.png"))
		MessageBoxA(nullptr, "Nie zaladowano tekstury Wraitha", "BLAD", MB_OK);


	//bosik
	if (!enemyBayleTexture.loadFromFile("Textures/enemies/bossBayle_anim.png"))
	{
		MessageBoxA(nullptr, "Nie zaladowano Bayle", "BLAD", MB_OK);
	}



	backgroundSprite.setTexture(backgroundTexture);

	Vector2u windowSize = window->getSize();
	Vector2u texSize = backgroundTexture.getSize();

	backgroundSprite.setScale(
		float(windowSize.x) / texSize.x,
		float(windowSize.y) / texSize.y
	);


	//init player
	player = new Player(choosenDragon, &this->playerTexture, &fireTexture, this->window->getSize());
	//this->players.push_back(Player(&playerTexture, &fireTexture));

	/*this->enemiesSaved.emplace_back(
		&this->enemy01Texture,
		this->window->getSize(),
		Vector2f(0.f, 0.f),             // pozycja startowa
		Vector2f(0.f, 1.f),             // kierunek
		Vector2f(0.1f, 0.1f),           // skala
		Enemy::EnemyType::Harpy,                    // typ przeciwnika
		&feather_missileTexture,
		1,                               // hpMax
		3,                               // damageMax
		1                                // damageMin
	);*/
	enemiesSaved.push_back(Enemy(&enemy01Texture, window->getSize(),
		Vector2f(0.f, 0.f), Vector2f(0.f, 1.f),
		Vector2f(0.1f, 0.1f),
		Enemy::EnemyType::Harpy,
		&feather_missileTexture));

	this->enemySpawnTimerMax = 150;// spawner
	this->enemySpawnTimer = this->enemySpawnTimerMax;


	this->InitUI();
	this->InitBars();
}

Game::~Game()
{
	delete player; 
}

//inicjalizuje texts
void Game::InitUI()
{
	Text tempTextbig;
	tempTextbig.setFont(font);
	tempTextbig.setCharacterSize(40);
	tempTextbig.setFillColor(Color(15, 207, 255));

	Text tempTextsmall;
	tempTextsmall.setFont(font);
	tempTextsmall.setCharacterSize(20);
	tempTextsmall.setFillColor(Color(15, 207, 255));

	tempTextbig.setString("HP   ");
	this->hpText = tempTextbig;

	tempTextbig.setString("LVL   ");
	this->lvlText = tempTextbig;

	tempTextsmall.setString("   ");
	this->expText = tempTextsmall;

	tempTextbig.setString("SCORE   ");
	this->scoreText = tempTextbig;

	tempTextbig.setString("GOLD   ");
	this->goldText = tempTextbig;

	/*tempTextbig.setString("TOTAL GOLD   ");
	this->test = tempTextbig;*/
}

void Game::UpdateUI()
{
	this->hpText.setPosition(Vector2f(10.0f, 90.0f));
	this->hpText.setString("HP   " + this->player->getHPasString());

	this->lvlText.setPosition(Vector2f(10.0f, 10.0f));
	this->lvlText.setString("LVL   " + this->player->getLVLasString());

	this->expText.setPosition(Vector2f(150.0f, 53.0f));
	this->expText.setString("   " + this->player->getEXPasString());

	this->scoreText.setPosition(Vector2f(10.0f, 180.0f));
	this->scoreText.setString("SCORE   " + this->player->getSCOREasString());

	this->goldText.setPosition(Vector2f(10.0f, 220.0f));
	this->goldText.setString("GOLD   " + this->player->getGOLDasString());//std::to_string(this->totalGold));

	/*this->test.setPosition(Vector2f(10.0f, 260.0f));
	this->test.setString("TOTAL GOLD   " + std::to_string(this->totalGold));*/

}

void Game::InitBars()
{
	this->hpBar.setSize(Vector2f(140.f, 10.f));
	this->hpBar.setFillColor(Color::White);
	this->hpBar.setPosition(10.f, 140.f);

	this->hpBarInside.setSize(Vector2f(140.f, 10.f));
	this->hpBarInside.setFillColor(Color::Red);
	this->hpBarInside.setPosition(this->hpBar.getPosition());

	this->expBar.setSize(Vector2f(140.f, 10.f));
	this->expBar.setFillColor(Color::White);
	this->expBar.setPosition(10.f, 60.f);

	this->expBarInside.setSize(Vector2f(140.f, 10.f));
	this->expBarInside.setFillColor(Color(130, 225, 96));
	this->expBarInside.setPosition(this->expBar.getPosition());
}

void Game::updateBars()
{
	float procentHP = player->getHP() / player->getHPmax();
	hpBarInside.setSize(Vector2(140.f * procentHP, 10.f));

	float procentEXP = player->getEXP() / player->getEXPnext();
	expBarInside.setSize(Vector2(140.f * procentEXP, 10.f));
}


void Game::CombatUpdate()
{

	auto& fires = player->getFires();

	for (size_t i = 0; i < fires.size(); )
	{
		bool fireDeleted = false;

		for (size_t j = 0; j < enemies.size() && !fireDeleted; )
		{
			if (fires[i].getGlobalBounds().intersects(
				enemies[j].getGlobalBounds()))
			{
				// obra¿enia
				enemies[j].takeDamage(fires[i].getDamage());


				if (!player->isPiercingActive())
				{
					fires.erase(fires.begin() + i);
					fireDeleted = true;
				}

				// jeœli wróg martwy
				if (enemies[j].getHP() <= 0)
				{
					//rozna ilosc exp i score w zaleznosci od typu pokonanego wroga
					if (enemies[j].getEnemyType() == Enemy::EnemyType::Harpy)
					{
						player->addEXP(10);
						player->addScore(50);
						this->totalGold += 5;
						player->addGold(5);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Raven)
					{
						player->addEXP(20);
						player->addScore(100);
						this->totalGold += 10;
						player->addGold(10);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Argus)
					{
						player->addEXP(30);
						player->addScore(150);
						this->totalGold += 15;
						player->addGold(15);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Tank)
					{
						player->addEXP(30);
						player->addScore(150);
						this->totalGold += 20;
						player->addGold(20);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Wraith)
					{
						player->addEXP(30);
						player->addScore(150);
						this->totalGold += 25;
						player->addGold(25);
					}

					//sprawdzam czy gracz osiagnal nowy poziom
					if (player->getEXP() >= player->getEXPnext())
					{
						int temp = player->getEXP() - player->getEXPnext();
						player->LevelUp();
						player->addEXP(temp);
						player->addScore(player->getlevelBonus());
					}

					TrySpawnPowerUp(enemies[j].getPosition());
					enemies.erase(enemies.begin() + j);

				}
				else
					j++;
			}
			else
			{
				j++;
			}
		}

		if (!fireDeleted)
			i++;
	}


}
void Game::CheckPlayerEnemyCollision()
{
	for (size_t i = 0; i < enemies.size(); )
	{
		if (player->getGlobalBounds().intersects(enemies[i].getHitbox().getGlobalBounds()))
		{
			// gracz dostaje obra¿enia
			player->takeDamage(enemies[i].getDamage());

			// wróg znika po zderzeniu
			if (enemies[i].getEnemyType() != Enemy::EnemyType::BossBayle)
			{
				enemies.erase(enemies.begin() + i);
			}
			else
			{
				enemies[i].takeDamage(1);
				i++;
			}

		}
		else
		{
			i++;
		}
	}
}
void Game::CheckEnemyFiresCollision()
{
	{
		auto& playerFires = player->getFires();

		for (auto& e : enemies) // dla ka¿dego wroga
		{
			auto& enemyFires = e.getFires();
			for (size_t i = 0; i < enemyFires.size(); )
			{
				if (player->getGlobalBounds().intersects(enemyFires[i].getGlobalBounds()))
				{
					// Gracz dostaje obra¿enia
					player->takeDamage(enemyFires[i].getDamage());

					// Specjalny efekt Wraitha
					if (e.getEnemyType() == Enemy::EnemyType::Wraith)
					{
						player->shuffleControls();      // losowe klawisze
						player->startWraithEffect();    // w³¹cz efekt migania
					}


					// Usuñ pocisk
					enemyFires.erase(enemyFires.begin() + i);
				}
				else
				{
					i++;
				}
			}
		}

	}
}
void Game::CheckProjectileCollisions()
{
	auto& playerFires = player->getFires();

	for (auto& e : enemies)
	{
		auto& enemyFires = e.getFires();

		for (size_t i = 0; i < playerFires.size(); )
		{
			bool destroyed = false;

			for (size_t j = 0; j < enemyFires.size() && !destroyed; )
			{
				if (playerFires[i].getGlobalBounds().intersects(enemyFires[j].getGlobalBounds()))
				{
					// oba pociski zniszczone
					playerFires.erase(playerFires.begin() + i);
					enemyFires.erase(enemyFires.begin() + j);
					destroyed = true;
				}
				else
				{
					j++;
				}
			}

			if (!destroyed)
				i++;
		}
	}
}

void Game::CheckPlayerPowerUpCollision()
{
	for (size_t i = 0; i < powerups.size(); )
	{
		if (player->getGlobalBounds().intersects(
			powerups[i].sprite.getGlobalBounds()))
		{
			// efekt powerupa
			switch (powerups[i].type)
			{
			case PowerUpType::Heart:
				player->addHP(1);
				showText("+1 HP");
				break;

			case PowerUpType::Sword:
				showText("TRIPLE SHOT!");
				player->enableTripleShot();
				break;
			case PowerUpType::Lightning:
				showText("SPEED BOOST!");
				player->addLightning();
				break;
			case PowerUpType::Pierce:
				showText("PIERCING SHOTS!");
				player->enablePiercing();
				break;

			}

			// usuwamy powerupa po zebraniu
			powerups.erase(powerups.begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void Game::Update()
{
	//gameover
	if (player->getHP() <= 0 && !gameOver)
	{
		this->gameOver = true;
		deathSound.play();
	}
	if (this->gameOver)
		return; // 
	//collision
	this->CombatUpdate();

	//player enemy collision
	this->CheckPlayerEnemyCollision();
	this->CheckEnemyFiresCollision();
	this->CheckProjectileCollisions();
	//Enemies update??


	float dt = deltaClock.restart().asSeconds();
	for (size_t i = 0; i < enemies.size();)
	{
		// Aktualizacja pozycji wroga (Harpy, Raven, Argus)
		enemies[i].Update(player->getPosition(), dt);

		// Aktualizacja pocisków wroga
		auto& enemyFires = enemies[i].getFires();
		for (size_t j = 0; j < enemyFires.size(); )
		{
			enemyFires[j].update(); // ruch pocisku
			// usuñ pocisk, jeœli poza ekranem
			if (enemyFires[j].getPosition().y > window->getSize().y)
				enemyFires.erase(enemyFires.begin() + j);
			else
				++j;
		}

		// Usuñ wroga, jeœli wyszed³ poza ekran
		if (enemies[i].getPosition().y > window->getSize().y)
			enemies.erase(enemies.begin() + i);
		else
			++i;
	}
	//Update timers
	if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer++;

	//dashujemy
	Vector2f dashDir(0.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::LShift))
	{
		// np. kierunek w zale¿noœci od W,A,S,D
		if (Keyboard::isKeyPressed(Keyboard::Up)) dashDir.y -= 1.f;
		if (Keyboard::isKeyPressed(Keyboard::Down)) dashDir.y += 1.f;
		if (Keyboard::isKeyPressed(Keyboard::Left)) dashDir.x -= 1.f;
		if (Keyboard::isKeyPressed(Keyboard::Right)) dashDir.x += 1.f;

		// normalizacja
		float len = std::sqrt(dashDir.x * dashDir.x + dashDir.y * dashDir.y);
		if (len != 0.f) dashDir /= len;

		player->StartDash(dashDir);
	}


	//spawn enemies
	gameTimer += 1.f / 60.f; // ka¿da klatka = 1/60 sekundy

	// --- Wy³¹czamy resp zwyk³ych przeciwników po 3 minutach ---
	if (gameTimer >= 3.f) {
		normalEnemiesSpawnEnabled = false;
	}

	// --- Sprawdzenie, czy wszyscy zwykli przeciwnicy pokonani ---
	bool allEnemiesDead = true;
	for (auto& e : enemies) {
		if (e.getEnemyType() != Enemy::EnemyType::BossBayle && !e.isDead()) {
			allEnemiesDead = false;
			break;
		}
	}

	// --- Spawn Bayle’a tylko raz ---
	if (!bayleSpawned && gameTimer >= 3.f && !normalEnemiesSpawnEnabled && allEnemiesDead) {
		bayleSpawned = true;

		sf::Vector2f spawnPos(window->getSize().x / 2.f, -200.f); // nad ekranem
		sf::Vector2f direction(0.f, 1.f); // ruch w dó³

		enemies.emplace_back(
			&enemyBayleTexture,
			window->getSize(),
			spawnPos,
			direction,
			sf::Vector2f(2.f, 2.f),      // skala sprite
			Enemy::EnemyType::BossBayle,
			&bayleMissile
		);
		bayle = &enemies.back();
	}

	// Aktualizacja Bayle'a
	if (bayleSpawned && bayle)
	{


		if (bayle->isDead())
		{
			/*for (auto it = enemies.begin() + 1; it != enemies.end(); ++it)
			{
				if (&(*it) == bayle) {
					enemies.erase(it); 
					break;
				}
			}*/

			enemies.pop_back();
			bayle = nullptr;
			bayleSpawned = false;
			bossReadyToSpawn = false;
			normalEnemiesSpawnEnabled = true; // opcjonalnie w³¹czamy resp zwyk³ych wrogów
		}
	}

	// Spawn zwyk³ych wrogów
	if (normalEnemiesSpawnEnabled)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			// Losujemy typ wroga

			Enemy::EnemyType type;
			int r = rand() % 100; // 0=Harpy, 1=Raven, 2=Argus, 3=Wraith,4=Tank

			if (r < 20) type = Enemy::EnemyType::Harpy;//20%
			else if (r >= 20 && r < 55) type = Enemy::EnemyType::Raven;//35%
			else if (r >= 55 && r < 85) type = Enemy::EnemyType::Argus;//30%
			else if (r >= 85 && r < 90) type = Enemy::EnemyType::Wraith;//5%
			else if (r >= 90 && r < 100) type = Enemy::EnemyType::Tank;//10%







			Texture* tex = nullptr;
			if (type == Enemy::EnemyType::Argus)
				tex = &enemyArgusTexture;
			else if (type == Enemy::EnemyType::Raven)
				tex = &enemyRavenTexture;
			else if (type == Enemy::EnemyType::Tank)
				tex = &enemyTankTexture;
			else if (type == Enemy::EnemyType::Harpy)
				tex = &enemy01Texture;
			else if (type == Enemy::EnemyType::Wraith)
				tex = &enemyWraithTexture;

			if (!tex)
				return; // lub continue w pêtli spawn

			Vector2f spawnPos;
			float halfWidth = tex->getSize().x * 0.1f / 2.f; // przeskalowana po³owa szerokoœci sprite
			spawnPos.x = halfWidth + static_cast<float>(rand() % (window->getSize().x - static_cast<int>(halfWidth * 2)));
			spawnPos.y = 0.f;
			Vector2f dir(0.f, 1.f);

			if (type == Enemy::EnemyType::Raven)
			{
				dir = player->getPosition() - spawnPos;
				float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
				if (len != 0.f) dir /= len;
			}

			Vector2f projectileScale(0.1f, 0.2f); // np. pióro Harpy jest wê¿sze i d³u¿sze

			Texture* texProjectile = nullptr;
			Vector2f projectileSpeed(0.f, 10.f); // prêdkoœæ pocisku w dó³
			if (type == Enemy::EnemyType::Harpy)
			{
				texProjectile = &feather_missileTexture;
				projectileScale = Vector2f(0.05f, 0.2f);
				projectileSpeed = Vector2f(0.f, 5.f);
			}
			else if (type == Enemy::EnemyType::Wraith)
			{
				texProjectile = &wraithMissile;
				projectileScale = Vector2f(0.05f, 0.2f);
				projectileSpeed = Vector2f(0.f, 5.f);
			}
			else
				texProjectile = nullptr;

			this->enemies.emplace_back(
				tex,                   // tekstura wroga
				window->getSize(),     // bounds
				spawnPos,              // pozycja startowa
				dir,                   // kierunek
				Vector2f(0.1f, 0.1f), // skala
				type,                  // typ
				texProjectile,         // tekstura pocisku
				2, 3, 1                // hpMax, damageMax, damageMin
			);

			for (size_t i = 0; i < powerups.size(); )
			{
				powerups[i].update(1.f / 60.f); // dt ~ 1/60 sekundy

				if (powerups[i].sprite.getPosition().y > window->getSize().y)
					powerups.erase(powerups.begin() + i);
				else
					++i;
			}
			this->CheckPlayerPowerUpCollision();
			this->enemySpawnTimer = 0;
		}
	}

	
	

	//update players
	//tu nie ma tej petli bo ona jest do kilku graczy
	player->Update(this->window->getSize()); //this->window->getSize()

	//update UI
	this->UpdateUI();
	//update bars
	this->updateBars();

	for (size_t i = 0; i < powerups.size(); )
	{
		powerups[i].update(1.f / 60.f); // dt ~ 1/60 sekundy

		if (powerups[i].sprite.getPosition().y > window->getSize().y)
			powerups.erase(powerups.begin() + i);
		else
			++i;
	}
	this->CheckPlayerPowerUpCollision();
	this->updateNotifications();
}


void Game::showText(std::string message) {
	this->infoText.setString(message);

	this->infoText.setPosition(this->window->getSize().x / 2.f
		- this->infoText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f);

	this->textTimer = this->textTimerMax;
}

void Game::updateNotifications()
{
	if (this->textTimer > 0.f)
	{
		this->textTimer -= 1.f;
	}

	int alpha = static_cast<int>((this->textTimer / this->textTimerMax) * 255);
	this->infoText.setFillColor(sf::Color(255, 255, 255, alpha));
	this->infoText.setOutlineColor(sf::Color(0, 0, 0, alpha));

	// Efekt Wraith: miganie gracza na czarno
	//float deltaTime = deltaClock.restart().asSeconds(); // czas od ostatniego update
	float deltaTime = 1.f / 60.f;
	if (wraithFogTimer > 0.f)
	{
		wraithFogTimer -= deltaTime;
		player->setWraithEffectActive(true);

		// Miganie na czarno
		wraithBlinkTime += deltaTime;
		float blink = std::sin(wraithBlinkTime * 5.f); // wolniejsze miganie
		int colorAlpha = (blink > 0) ? 100 : 255;
		player->getSprite().setColor(sf::Color(0, 0, 0, colorAlpha));
	}
	else if (player->getWraithEffectActive()) // efekt siê koñczy
	{
		player->setWraithEffectActive(false);
		wraithBlinkTime = 0.f;
		player->getSprite().setColor(sf::Color::White);
		player->restoreControls(); // przywróæ klawisze
	}



}

void Game::drawNotifications() {
	if (this->textTimer > 0.f) {
		this->window->draw(this->infoText);
	}
}


void Game::DrawUI()
{
	this->window->draw(this->hpText);
	this->window->draw(this->lvlText);
	this->window->draw(this->expText);
	this->window->draw(this->scoreText);
	this->window->draw(this->goldText);
}

void Game::drawBars()
{
	this->window->draw(this->hpBar);
	this->window->draw(this->hpBarInside);
	this->window->draw(this->expBar);
	this->window->draw(this->expBarInside);
}

void Game::Draw()
{
	//window->clear();

	window->draw(backgroundSprite);

	player->Draw(*window);

	for (auto& e : enemies)
	{
		e.Draw(*window);

		// rysowanie pocisków wroga
		for (auto& f : e.getFires())
			f.draw(*window);
	}

	this->DrawUI();//rysowanie tekstu
	this->drawBars();//rysowanie barków

	//window->display();
	for (auto& pu : powerups)
		window->draw(pu.sprite);
}

void Game::Reset(const DragonProfile& choosenDragon) {

	this->gameOver = false;

	std::string newTexturePath = choosenDragon.texturePath;
	if (!this->playerTexture.loadFromFile(newTexturePath))
	{
		MessageBoxA(nullptr, "NIE ZALADOWANO TEKSTURY", "BLAD", MB_OK);
	}

	delete player;
	player = new Player(choosenDragon, &this->playerTexture, &fireTexture,
		this->window->getSize()); //this->window->getSize()

	//usunac pzreciwniow
	this->enemies.clear();
	this->enemySpawnTimer = 0;

	//reset statystyk

}
void Game::TrySpawnPowerUp(const sf::Vector2f& position)
{
	int chance = rand() % 100;
	if (chance < 30)//szansa na drop 
	{
		int r = rand() % 4;
		PowerUpType type;
		sf::Texture* tex = NULL;

		switch (r)
		{
		case 0: type = PowerUpType::Heart; tex = &powerupTextureHeart; break;
		case 1: type = PowerUpType::Sword; tex = &powerupTextureSword; break;
		case 2: type = PowerUpType::Lightning; tex = &powerupTextureLightning; break;
		case 3: type = PowerUpType::Pierce; tex = &powerupTexturePierce; break;
		}

		powerups.emplace_back(type, position, *tex);
	}
}

