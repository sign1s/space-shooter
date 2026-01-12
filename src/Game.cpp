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
	

	
	
	//init textures
	//this->playerTexture.loadFromFile("Textures/player/Green_Dragon_overhead.png");
	if (!this->playerTexture.loadFromFile("Textures/player/Green_Dragon_overhead.png"))
	{
		MessageBoxA(nullptr, "NIE ZALADOWANO TEKSTURY", "BLAD", MB_OK);
	}

	this->fireTexture.loadFromFile("Textures/bullet/fire_prev.png");
	this->feather_missileTexture.loadFromFile("Textures/bullet/feather_missile.png");

	this->enemy01Texture.loadFromFile("Textures/enemies/harpy2.png");
	
	//init player
	player = new Player(&this->playerTexture, &fireTexture);
	//this->players.push_back(Player(&playerTexture, &fireTexture));

	Enemy e1(
		&this->enemy01Texture,
		this->window->getSize(),
		/*Vector2f(
			static_cast<float>(rand() % this->window->getSize().x),
			0.f),*/ //pozycja starowa
		
		Vector2f(0.f, 0.f),
		
		Vector2f(0.f, 1.f),  //predkosc
		Vector2f(0.1f, 0.1f), //skala
		0, 1, 3, 1
	);
	this->enemiesSaved.push_back(Enemy(e1));

	this->enemySpawnTimerMax = 20;
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
	tempText.setCharacterSize(30);
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

	this->lvlText.setPosition(Vector2f(10.0f, 70.0f));
	this->lvlText.setString("LVL   " + this->player->getLVLasString());

	this->expText.setPosition(Vector2f(10.0f, 100.0f));
	this->expText.setString("EXP   " + this->player->getEXPasString());

	this->goldText.setPosition(Vector2f(10.0f, 160.0f));
	this->goldText.setString("GOLD   " + this->player->getGOLDasString());

	this->scoreText.setPosition(Vector2f(10.0f, 190.0f));
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

	//Enemies update??
	
	for (size_t i = 0; i < enemies.size();)
	{
		this->enemies[i].Update();
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
		}
		else
		{
			i++;
		}

	}
	//Update timers
	if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer++;

	//spawn enemies

	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		this->enemies.push_back(Enemy(
			&this->enemy01Texture,
			this->window->getSize(),
			Vector2f(static_cast<float>(rand() % this->window->getSize().x), 0.f),
			Vector2f(0.f, 1.f),  //predkosc
			Vector2f(0.1f, 0.1f), //skala
			0, rand() % 3 + 1, 3, 1));

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
	window->clear();
	player->Draw(*window);
	
	for (auto& e : enemies)
		e.Draw(*window);

	this->DrawUI();//rysowanie tekstu

	window->display();
}
