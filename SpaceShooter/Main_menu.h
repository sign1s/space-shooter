#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;
#define Max_main_menu 4

class Main_menu
{
public:
	Main_menu(float width, float height);

	void draw(RenderWindow& window);
	void moveUp();
	void moveDown();

	int mainMenuPressed() {
		return mainMenuSelected;
	}
	~Main_menu();
private:
	int mainMenuSelected;
	Font font;
	Text mainMenu[Max_main_menu];
	Text gameTitle;

	//background
	Texture mainTexture;
	RectangleShape background;
};

