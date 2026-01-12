#include "PauseMenu.h"

PauseMenu::PauseMenu(float width, float height) {

	/*if (!mainTexture.loadFromFile("Textures/background2.jpg")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);*/

	this->background.setSize(Vector2f(width, height));
	this->background.setFillColor(Color(0, 0, 0, 150));

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}
	//Game title
	/*gameTitle.setFont(font);
	gameTitle.setFillColor(Color::White);
	gameTitle.setString("Dragon Shooter");
	gameTitle.setCharacterSize(70);
	gameTitle.setPosition((width - gameTitle.getGlobalBounds().width) / 2, 50);*/

	//Play
	this->pauseMenu[0].setFont(font);
	this->pauseMenu[0].setFillColor(Color::Blue);
	this->pauseMenu[0].setString("Play");
	this->pauseMenu[0].setCharacterSize(70);
	this->pauseMenu[0].setPosition((width - this->pauseMenu[0].getGlobalBounds().width) / 2,
		height / 2 - 100);

	//Shop
	this->pauseMenu[1].setFont(font);
	this->pauseMenu[1].setFillColor(Color::White);
	this->pauseMenu[1].setString("Exit");
	this->pauseMenu[1].setCharacterSize(70);
	this->pauseMenu[1].setPosition((width - this->pauseMenu[1].getGlobalBounds().width) / 2,
		height / 2);

	this->pauseSelected = 0;
}

PauseMenu::~PauseMenu() {

}

//Draw menu
void PauseMenu::draw(RenderWindow& window) {
	window.draw(this->background);
	//window.draw(gameTitle);
	for (int i = 0; i < Max_pause_menu; i++) {
		window.draw(this->pauseMenu[i]);
	}
}

//MoveUp
void PauseMenu::moveUp() {
	if (this->pauseSelected - 1 >= 0) {

		this->pauseMenu[this->pauseSelected].setFillColor(Color::White);
		this->pauseSelected--;
		//if (mainMenuSelected == -1) {
		//	mainMenuSelected = 2;
		//}
		this->pauseMenu[this->pauseSelected].setFillColor(Color::Blue);
	}
}

//MoveDown
void PauseMenu::moveDown() {
	if (this->pauseSelected + 1 < Max_pause_menu) {

		this->pauseMenu[this->pauseSelected].setFillColor(Color::White);
		this->pauseSelected++;
		//if (mainMenuSelected == 3) {

		//	mainMenuSelected = 2;
		//}
		this->pauseMenu[this->pauseSelected].setFillColor(Color::Blue);
	}
}

void PauseMenu::resetPause() {
	this->pauseSelected = 0;

	for (int i = 0; i < Max_pause_menu; i++) {
		this->pauseMenu[i].setFillColor(Color::White);
	}
	this->pauseMenu[0].setFillColor(Color::Blue);
}




