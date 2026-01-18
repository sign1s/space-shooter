#pragma once

#include "player.h"
#include "enemy.h"
#include "PowerUp.h"
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

using namespace sf;

class Game
{

private:
	RenderWindow* window;
	sf::Clock deltaClock;

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
	Text infoText;

	//text timers
	float textTimer;
	float textTimerMax;

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
	std::vector<PowerUp> powerups;
	int enemySpawnTimer;
	int enemySpawnTimerMax;


	//Enemy* enemy01;

	//tekstury przeciwnikow
	//std::vector<Texture> enemyTextures;
	Texture enemy01Texture;
	Texture enemyRavenTexture;
	Texture enemyArgusTexture;
	Texture enemyWraithTexture;
	Texture enemyTankTexture;
	Texture enemyBayleTexture;

	//tekstury powerupow
	sf::Texture powerupTextureHeart;
	sf::Texture powerupTextureLightning;
	sf::Texture powerupTextureSword;
	sf::Texture powerupTexturePierce;


	bool showHitboxes = true;

	// licznik efektu Wraitha
	int wraithFogTimer = 3.f;
	float wraithBlinkTime = 0.f;

	//waluta
	int totalGold;

	//boss kontrolki
	bool bossReadyToSpawn = false;   // czy Bayle mo¿e siê pojawiæ
	bool bossSpawned = false;        // czy Bayle ju¿ siê pojawi³
	Enemy* bayle = nullptr;          // wskaŸnik na bossa
	sf::Clock gameClock;             // licznik czasu gry
	bool normalEnemiesSpawnEnabled = true; // kontrola respów zwyk³ych wrogów
	bool bayleSpawned = false;
	float gameTimer = 0.f; // licznik czasu gry w sekundach
	float enemyRespawnEnabled = true;


public:
	Game(RenderWindow* window, const DragonProfile& choosenDragon);
	virtual ~Game();

	//pocisk
	Texture fireTexture;
	Texture feather_missileTexture;
	Texture wraithMissile;
	Texture bayleMissile;

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

	inline const int getTotalGold() const {

		return this->totalGold;
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
	void Reset(const DragonProfile& profile);//resertowanie rozgrywki
	bool enoughGold(int amount) {
		if (this->totalGold >= amount) {
			this->totalGold -= amount;
			return true;
		}
		return false;
	}

	void showText(std::string message);
	void updateNotifications(); //for text timers
	void drawNotifications();   //for infoText

	void TrySpawnPowerUp(const sf::Vector2f& position);
	void CheckPlayerPowerUpCollision();

	//funkcja ktora bierze wskaznik do playera
	 Player* getPlayer() const { return this->player; }
};
