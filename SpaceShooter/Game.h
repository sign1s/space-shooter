#pragma once

#include "player.h"
#include "enemy.h"
#include <SFML/Audio.hpp>

using namespace sf;

class Game
{

private:
	RenderWindow* window;

	//background
	Texture backgroundTexture;
	Sprite backgroundSprite;

	//Text
	Font font;
	Text hpText;
	Text lvlText;
	Text expText;
	Text goldText;
	Text scoreText;

	//bars
	RectangleShape hpBar;
	RectangleShape hpBarInside;
	RectangleShape expBar;
	RectangleShape expBarInside;

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
	Texture enemyRavenTexture;
	Texture enemyArgusTexture;

	
public:
	Game(RenderWindow* window);
	virtual ~Game();

	//pocisk
	Texture fireTexture;
	Texture feather_missileTexture;
	
	//player enemy collision
	void CheckPlayerEnemyCollision();
	void CheckEnemyFiresCollision();
	void CheckProjectileCollisions();
	
	//Accessors
	inline RenderWindow& getWindow()
	{
		return *this->window;
	}

	inline const bool getGameOver()
		const {
		return this->gameOver;
	}

	//Funkcje
	//user interface
	void InitUI(); //teksty
	void UpdateUI();
	void DrawUI();
	void InitBars(); //barki
	void updateBars();
	void drawBars();

	void CombatUpdate();
	void Update();
	void Draw();
	void Reset();//resertowanie rozgrywki

};
