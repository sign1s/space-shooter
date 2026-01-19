#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;
#define Max_game_over 2

class GameOverMenu
{
public:
	GameOverMenu(float width, float height);

	void draw(RenderWindow& window);
	void moveUp();
	void moveDown();
	//void resetPause();

	int buttonPressed() {
		return indexSelected;
	}
	~GameOverMenu();
private:
	int indexSelected;
	Font font;
	Text gameoverMenu[Max_game_over];
	Text gameOverText;

	//background
	RectangleShape background;
};
