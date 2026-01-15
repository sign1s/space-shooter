#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include "DragonProfile.h"

using namespace sf;

#define Max_shop 3

class Shop
{
private:
	int itemSelected;
	int equippedIndex;
	


	std::vector<DragonProfile> Dragons;
	std::vector<Texture> dragonTextures;



	//TEXT
	Font font;
	Text title;
	Text goldAmountTex;
	//Text noGold;

	//dragon statistics
	Text dragonName;
	Text dragonStats;
	Text priceTag;
	Sprite previewSprite; //dragon image

	//background
	Texture mainTexture;
	RectangleShape background;

public:
	Shop(float width, float height);
	~Shop();

	//Functions
	void updateUI();
	void initUI();
	void draw(RenderWindow& window, int goldAmount);
	void moveLeft();
	void moveRight();
	void unlockDragon(int index) {
		if (index >= 0 && index < Dragons.size()) {
			this->Dragons[index].isUnlocked = true;
			this->updateUI();
		}
	}

	int itemPressed() {
		return itemSelected;
	}

	void equipDragon(int index) { this->equippedIndex = index; }
	int getEquippedIndex() const { return equippedIndex; }


	//accessors
	const DragonProfile& getSelectedDragon() const { return Dragons[itemSelected]; }
	const DragonProfile& getDragon(int index) { return Dragons[index]; }
	

};

