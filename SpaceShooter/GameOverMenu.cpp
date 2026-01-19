#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(float width, float height) {

	/*if (!mainTexture.loadFromFile("Textures/background2.jpg")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);*/

	this->background.setSize(Vector2f(width, height));
	this->background.setFillColor(Color(255, 0, 0, 70));

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}
	//Game over text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(100);
	this->gameOverText.setFillColor(Color::Black); //Color(105, 5, 5, 255) czerwony
	this->gameOverText.setString("YOU DIED");
	this->gameOverText.setPosition((width
		- this->gameOverText.getGlobalBounds().width) / 2.f, 100);

	//Play AGain
	this->gameoverMenu[0].setFont(this->font);
	this->gameoverMenu[0].setFillColor(Color(15, 207, 255));
	this->gameoverMenu[0].setString("Play Again");
	this->gameoverMenu[0].setCharacterSize(70);
	this->gameoverMenu[0].setPosition((width - this->gameoverMenu[0].getGlobalBounds().width) / 2,
		height / 2 - 100);

	//Menu
	this->gameoverMenu[1].setFont(this->font);
	this->gameoverMenu[1].setFillColor(Color::White);
	this->gameoverMenu[1].setString("Back to Menu");
	this->gameoverMenu[1].setCharacterSize(70);
	this->gameoverMenu[1].setPosition((width - this->gameoverMenu[1].getGlobalBounds().width) / 2,
		height / 2);

	this->indexSelected = 0;
}

GameOverMenu::~GameOverMenu() {

}

//Draw menu
void GameOverMenu::draw(RenderWindow& window) {
	window.draw(this->background);
	window.draw(this->gameOverText);
	for (int i = 0; i < Max_game_over; i++) {
		window.draw(this->gameoverMenu[i]);
	}
}

//MoveUp
void GameOverMenu::moveUp() {
	if (this->indexSelected - 1 >= 0) {

		this->gameoverMenu[this->indexSelected].setFillColor(Color::White);
		this->indexSelected--;
		//if (mainMenuSelected == -1) {
		//	mainMenuSelected = 2;
		//}
		this->gameoverMenu[this->indexSelected].setFillColor(Color(15, 207, 255));
	}
}

//MoveDown
void GameOverMenu::moveDown() {
	if (this->indexSelected + 1 < Max_game_over) {

		this->gameoverMenu[this->indexSelected].setFillColor(Color::White);
		this->indexSelected++;
		//if (mainMenuSelected == 3) {

		//	mainMenuSelected = 2;
		//}
		this->gameoverMenu[this->indexSelected].setFillColor(Color(15, 207, 255));
	}
}




