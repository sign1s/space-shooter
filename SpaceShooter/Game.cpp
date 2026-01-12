#include "Game.h"
#include<iostream>
#include <Windows.h>
//#include <SFML/Audio.hpp>
#include <filesystem>
Game::Game(RenderWindow* window)
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
	player = new Player(&this->playerTexture, &fireTexture, this->window->getSize());
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
}

Game::~Game()
{
	delete player; //czy to ma tu byc?  //to ma tu byc
	//ty mi lepiej powiedz czy CombatUpdate ma istniec //nie wiem na razie nie sobie bedzie
}

//inicjalizuje texts
void Game::InitUI()
{
	Text tempText;
	tempText.setFont(font);
	tempText.setCharacterSize(40);
	tempText.setFillColor(Color(64, 191, 222));
	tempText.setString("HP   ");
	this->hpText = tempText;

	tempText.setString("LVL   ");
	this->lvlText = tempText;

	tempText.setString("EXP   ");
	this->expText = tempText;

	tempText.setString("GOLD   ");
	this->goldText = tempText;

	tempText.setString("SCORE   ");
	this->scoreText = tempText;
}

void Game::UpdateUI()
{
	this->hpText.setPosition(Vector2f(10.0f, 10.0f));
	this->hpText.setString("HP   " + this->player->getHPasString());

	this->lvlText.setPosition(Vector2f(10.0f, 90.0f));
	this->lvlText.setString("LVL   " + this->player->getLVLasString());

	this->expText.setPosition(Vector2f(10.0f, 130.0f));
	this->expText.setString("EXP   " + this->player->getEXPasString());

	this->goldText.setPosition(Vector2f(10.0f, 210.0f));
	this->goldText.setString("GOLD   " + this->player->getGOLDasString());

	this->scoreText.setPosition(Vector2f(10.0f, 250.0f));
	this->scoreText.setString("SCORE   " + this->player->getSCOREasString());
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
						enemies.erase(enemies.begin() + j);
						player->addScore(10);
						player->addEXP();
						if (player->getEXP() == player->getEXPnext())
							player->levelUP();
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


	//KOLES ZROBIL TO TUTAJ (USUWANIE TYCH POCISKOW), ALE CHYBA LEPIEJ MIEC TO W PLAYER::UPDATE
	////Fires update
	//std::vector<Fire>& playerFires = player->getFires();
	//	//Window bounds
	//for (size_t i = 0; i < playerFires.size(); ) {

	//	//+getGlobalBounds().height zeby miec pewnosc, ze faktycznie sa poza ekranem
	//	if (playerFires[i].getPosition().y + playerFires[i].getGlobalBounds().height < 0.f) {
	//		playerFires.erase(playerFires.begin() + i);
	//	}
	//	else {
	//		i++;
	//	}
	//}



	//update UI
	this->UpdateUI();
}

void Game::DrawUI()
{
	this->window->draw(this->hpText);
	this->window->draw(this->lvlText);
	this->window->draw(this->expText);
	this->window->draw(this->goldText);
	this->window->draw(this->scoreText);
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

	//window->display();
}

void Game::Reset() {

	this->gameOver = false;

	delete player;
	player = new Player(&this->playerTexture, &fireTexture,
		this->window->getSize()); //this->window->getSize()

	//usunac pzreciwniow
	this->enemies.clear();
	this->enemySpawnTimer = 0;

	//reset statystyk

}
