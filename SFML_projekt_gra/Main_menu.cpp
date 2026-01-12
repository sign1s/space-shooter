#include "Main_menu.h"

Main_menu::Main_menu(float width, float height) {

	if (!mainTexture.loadFromFile("Textures/background/background2.jpg")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}
	//Game title
	this->gameTitle.setFont(font);
	this->gameTitle.setFillColor(Color::White);
	this->gameTitle.setString("Dragon Shooter");
	this->gameTitle.setCharacterSize(70);
	this->gameTitle.setPosition((width - this->gameTitle.getGlobalBounds().width) / 2, 50);

	//Play
	this->mainMenu[0].setFont(font);
	this->mainMenu[0].setFillColor(Color::Blue);
	this->mainMenu[0].setString("Play");
	this->mainMenu[0].setCharacterSize(70);
	this->mainMenu[0].setPosition((width - this->mainMenu[0].getGlobalBounds().width) / 2,
		height / 2 - 100);

	//Shop
	this->mainMenu[1].setFont(font);
	this->mainMenu[1].setFillColor(Color::White);
	this->mainMenu[1].setString("Shop");
	this->mainMenu[1].setCharacterSize(70);
	this->mainMenu[1].setPosition((width - this->mainMenu[1].getGlobalBounds().width) / 2,
		height / 2);

	//Exit
	this->mainMenu[2].setFont(font);
	this->mainMenu[2].setFillColor(Color::White);
	this->mainMenu[2].setString("Exit");
	this->mainMenu[2].setCharacterSize(70);
	this->mainMenu[2].setPosition((width - this->mainMenu[2].getGlobalBounds().width) / 2, height / 2 + 100);

	this->mainMenuSelected = 0;
}

Main_menu::~Main_menu() {

}

//Draw menu
void Main_menu::draw(RenderWindow& window) {
	window.draw(this->background);
	window.draw(this->gameTitle);
	for (int i = 0; i < Max_main_menu; i++) {
		window.draw(this->mainMenu[i]);
	}
}

//MoveUp
void Main_menu::moveUp() {
	if (this->mainMenuSelected - 1 >= 0) {

		this->mainMenu[this->mainMenuSelected].setFillColor(Color::White);
		this->mainMenuSelected--;
		//if (mainMenuSelected == -1) {
		//	mainMenuSelected = 2;
		//}
		this->mainMenu[this->mainMenuSelected].setFillColor(Color::Blue);
	}
}

//MoveDown
void Main_menu::moveDown() {
	if (this->mainMenuSelected + 1 < Max_main_menu) {

		this->mainMenu[this->mainMenuSelected].setFillColor(Color::White);
		this->mainMenuSelected++;
		//if (mainMenuSelected == 3) {

		//	mainMenuSelected = 2;
		//}
		this->mainMenu[this->mainMenuSelected].setFillColor(Color::Blue);
	}
}



