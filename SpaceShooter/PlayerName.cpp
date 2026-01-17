#include "PlayerName.h"


PlayerName::PlayerName(float width, float height)
{
	if (!mainTexture.loadFromFile("Textures/background/shop_background.png")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);

	this->background2.setSize(Vector2f(500, 300));
	this->background2.setPosition(width / 2-250, height / 2-250);
	this->background2.setFillColor(Color::Black);

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}

	nameRequest.setFont(font);
	nameRequest.setFillColor(Color::White);
	nameRequest.setString("Enter your name");
	nameRequest.setCharacterSize(70);
	nameRequest.setPosition((width - nameRequest.getGlobalBounds().width) / 2,
		height / 2 - 200);

	nameText.setFont(font);
	nameText.setFillColor(Color(15, 207, 255));
	nameText.setCharacterSize(60);
	nameText.setPosition((width - nameRequest.getGlobalBounds().width) / 2,
		height / 2 - 100);
}

PlayerName::~PlayerName() {

}

void PlayerName::setName(String name)
{
	this->playerName = name;
}

void PlayerName::draw(RenderWindow& window) 
{
	window.draw(this->background);
	window.draw(this->background2);
	window.draw(this->nameRequest);
	window.draw(this->nameText);
}


void PlayerName::enterPlayerName(const Event& event)
{
	if (event.type == Event::TextEntered)
	{
		if (playerName.getSize() > 16)//limit s³ów
		{
			return;
		}

		if (event.text.unicode == 8)//obsluguje backspace
		{
			if (playerName.getSize() >= 1)

			{
				playerName.erase(playerName.getSize() - 1, 1);
			}
		}

		else if (event.text.unicode == 32)//obsluguje spacje
		{
			playerName += ' ';
		}

		else if (event.text.unicode > 32 && event.text.unicode < 126)
		{
			playerName += event.text.unicode;
		}
	}

	nameText.setString(playerName);
}