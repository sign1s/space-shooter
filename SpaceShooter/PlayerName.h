#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class PlayerName
{
public:
	PlayerName(float width, float height);

	void draw(RenderWindow& window);
	void enterPlayerName(const Event& event);
	const String& getName() { return playerName; }
	void setName(String name);


	~PlayerName();

private:
	//font
	Font font;

	//background
	Texture mainTexture;
	RectangleShape background;
	RectangleShape background2;

	//texts
	Text nameRequest;
	Text nameText;

	//string
	String playerName;

};

