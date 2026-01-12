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
	Text hpText;
	Text lvlText;
	Text expText;
	Text goldText;
	Text scoreText;

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

	//Funkcje
	void InitUI();//UI jest od tekstow(czyli hp narazie) //user interface
	void UpdateUI();
	void DrawUI();
	void CombatUpdate();
	void Update();
	void Draw();

};
