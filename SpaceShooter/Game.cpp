#include "Game.h"
#include<iostream>
#include <Windows.h>
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

	this->fireTexture.loadFromFile("Textures/bullet/fire_prev.png");
	this->feather_missileTexture.loadFromFile("Textures/bullet/feather_missile.png");

	this->enemy01Texture.loadFromFile("Textures/enemies/Harpy.png");
	this->enemyRavenTexture.loadFromFile("Textures/enemies/raven.png");
	this->enemyArgusTexture.loadFromFile("Textures/enemies/argus.png");



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

	this->enemySpawnTimerMax = 150;
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

				// usuñ pocisk
				fires.erase(fires.begin() + i);
				fireDeleted = true;

				// jeœli wróg martwy
				if (enemies[j].getHP() <= 0)
				{
					//rozna ilosc exp i score w zaleznosci od typu pokonanego wroga
					if (enemies[j].getEnemyType() == Enemy::EnemyType::Harpy)
					{
						player->addEXP(10);
						player->addScore(50);
						this->totalGold += 2;
						player->addGold(2);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Raven)
					{
						player->addEXP(20);
						player->addScore(100);
						this->totalGold += 5;
						player->addGold(5);
					}
					else if (enemies[j].getEnemyType() == Enemy::EnemyType::Argus)
					{
						player->addEXP(30);
						player->addScore(150);
						this->totalGold += 1;
						player->addGold(1);
					}

					//sprawdzam czy gracz osiagnal nowy poziom
					if (player->getEXP() >= player->getEXPnext())
					{
						int temp = player->getEXP() - player->getEXPnext();
						player->setLevel(player->getLevel()+1);
						player->addEXP(temp);
						player->addScore(player->getlevelBonus());
					}

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
		if (player->getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
		{
			// gracz dostaje obra¿enia
			player->takeDamage(1);

			// wróg znika po zderzeniu
			enemies.erase(enemies.begin() + i);
		}
		else
		{
			i++;
		}
	}
}
void Game::CheckEnemyFiresCollision()
{
	for (auto& e : enemies) // dla ka¿dego wroga
	{
		auto& enemyFires = e.getFires();
		for (size_t i = 0; i < enemyFires.size(); )
		{
			if (player->getGlobalBounds().intersects(enemyFires[i].getGlobalBounds()))
			{
				// Gracz dostaje obra¿enia
				player->takeDamage(enemyFires[i].getDamage());

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

	for (size_t i = 0; i < enemies.size();)
	{
		// Aktualizacja pozycji wroga (Harpy, Raven, Argus)
		enemies[i].Update(player->getPosition());

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

	//spawn enemies

	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		// Losujemy typ wroga
		Enemy::EnemyType type;
		int r = rand() % 3; // 0=Harpy, 1=Raven, 2=Argus
		switch (r)
		{
		case 0: type = Enemy::EnemyType::Harpy; break;
		case 1: type = Enemy::EnemyType::Raven; break;
		case 2: type = Enemy::EnemyType::Argus; break;
		}






		Texture* tex = nullptr;
		if (type == Enemy::EnemyType::Argus)
			tex = &enemyArgusTexture;
		else if (type == Enemy::EnemyType::Raven)
			tex = &enemyRavenTexture;
		else if (type == Enemy::EnemyType::Harpy)
			tex = &enemy01Texture;

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

		this->enemySpawnTimer = 0;
	}



	//update players
	//tu nie ma tej petli bo ona jest do kilku graczy
	player->Update(this->window->getSize()); //this->window->getSize()

	//update UI
	this->UpdateUI();
	//update bars
	this->updateBars();
}


void Game::showText(std::string message) {
	this->infoText.setString(message);

	this->infoText.setPosition(this->window->getSize().x / 2.f
		- this->infoText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f);

	this->textTimer = this->textTimerMax;
}

void Game::updateNotifications() {
	if (this->textTimer > 0.f) {
		this->textTimer -= 1.f;
	}

	int alpha = static_cast<int>((this->textTimer / this->textTimerMax) * 255);
	this->infoText.setFillColor(sf::Color(255, 255, 255, alpha));
	this->infoText.setOutlineColor(sf::Color(0, 0, 0, alpha));
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

