#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>

#define number_of_slots 5

using namespace sf;

class FileManager
{
public:
	FileManager(float width, float height);

	void draw(RenderWindow& window);
	void moveUp();
	void moveDown();

	void UpdateSlotUI();

	int slotPressed() {
		return slotSelected;
	}

	//funkja tez sprawdza czy dany slot jest "<EMPTY>"
	bool LoadFile(int slotSelected, std::string& playerName,
		int& level, int&hp, int& exp,int&gold, int& score, int& dragon_index);

	void SaveFile(int slotSelected, std::string playerName,
		int level, int hp, int exp, int gold, int score, int dragon_index);

	~FileManager();
private:
	//do save i load
	//std::vector<std::string>tempContents;

	//font
	Font font;

	//background
	RectangleShape background;
	Texture mainTexture;
	RectangleShape background2;


	Text save1;
	Text save2;
	Text save3;
	Text save4;
	Text save5;

	int slotSelected;
	//nazwy plikow tekstowych
	static const std::string filenames[number_of_slots];

	//texts
	Text slot_choice;
	Text slots[number_of_slots];

};