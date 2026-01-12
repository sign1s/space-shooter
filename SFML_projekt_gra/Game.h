//#pragma once
//
//#include "player.h"
//
//using namespace sf;
//
//class Game
//{
//
//private:
//	RenderWindow* window;
//
//	//Text
//	Font font;
//	Text staticPlayerText;
//
//
//	//std::vector<Player> players;
//	Player* player;
//	Texture playerTexture;
//
//
//public:
//	Game(RenderWindow* window);
//	virtual ~Game();
//
//	//pocisk
//	Texture fireTexture;
//
//	//Accessors
//	inline RenderWindow& getWindow()
//	{
//		return *this->window;
//	}
//
//	//Funkcje
//	void InitUI();//UI jest od tekstow(czyli hp narazie) //user interface
//	void UpdateUI();
//	void DrawUI();
//	void CombatUpdate();
//	void Update();
//	void Draw();
//	void Reset(); //resertowanie rozgrywki
//
//
//};
//
//




#pragma once

#include "player.h"
#include "enemy.h"
#include <SFML/Audio.hpp>

using namespace sf;

class Game
{

private:
	RenderWindow* window;

	//Text
	Font font;
	Text staticPlayerText;
	//Text gameOverText; //gameovertext

	//gameover
	bool gameOver;
	//sounds

	sf::SoundBuffer deathBuffer;
	sf::Sound deathSound;




	//std::vector<Player> players;
	Player* player;
	Texture playerTexture;

	//przeciwnicy
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	int enemySpawnTimer;
	int enemySpawnTimerMax;
	//Enemy* enemy01;

	//tekstury przeciwnikow
	//std::vector<Texture> enemyTextures;
	Texture enemy01Texture;


public:
	Game(RenderWindow* window);
	virtual ~Game();

	//pocisk
	Texture fireTexture;
	Texture feather_missileTexture;

	//player enemy collision
	void CheckPlayerEnemyCollision();

	//Accessors
	inline RenderWindow& getWindow()
	{
		return *this->window;
	}

	inline const bool getGameOver() 
		const { return this->gameOver; }

	//Funkcje
	void InitUI();//UI jest od tekstow(czyli hp narazie) //user interface
	void UpdateUI();
	void DrawUI();
	void CombatUpdate();
	void Update();
	void Draw();
	void Reset();//resertowanie rozgrywki
};
