#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;
#define Max_pause_menu 2

class PauseMenu
{
public:
	PauseMenu(float width, float height);

	void draw(RenderWindow& window);
	void moveUp();
	void moveDown();
	void resetPause();

	int pausePressed() {
		return pauseSelected;
	}
	~PauseMenu();
private:
	int pauseSelected;
	Font font;
	Text pauseMenu[Max_pause_menu];
	//Text gameTitle;

	//background
	RectangleShape background;
};

