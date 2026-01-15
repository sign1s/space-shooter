#include "Shop.h"

Shop::Shop(float width, float height) {

	if (!mainTexture.loadFromFile("Textures/background/shop_background.png")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}

	this->initUI();

	//shop title
	this->title.setFont(font);
	this->title.setFillColor(Color::White);
	this->title.setString("Dragon Shop");
	this->title.setCharacterSize(100);
	this->title.setPosition((width - this->title.getGlobalBounds().width) / 2, 90);

	//DRAGON PROFILE (index 0)
	DragonProfile defaultDragon;
	defaultDragon.name = "Green";
	defaultDragon.texturePath = "Textures/player/Green_Dragon_overhead.png";
	defaultDragon.hpMax = 10;
	defaultDragon.damage = 1;
	defaultDragon.shootTimerMax = 20;
	defaultDragon.price = 0;
	defaultDragon.isUnlocked = true;

	this->Dragons.push_back(defaultDragon);

	Texture tex;
	if (!tex.loadFromFile(defaultDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex);


	//DRAGON 2
	DragonProfile redDragon;
	redDragon.name = "Red";
	redDragon.texturePath = "Textures/player/Red_Dragon.png";
	redDragon.hpMax = 20;
	redDragon.damage = 3;
	redDragon.shootTimerMax = 35;
	redDragon.price = 500;
	redDragon.isUnlocked = false;

	this->Dragons.push_back(redDragon);

	Texture tex2;
	if (!tex2.loadFromFile(redDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex2);


	//DRAGON 3
	//DragonProfile blueDragon;
	//blueDragon.name = "Blue";
	//blueDragon.texturePath = "Textures/player/dragon_blue.png";
	//blueDragon.hpMax = 20;
	//blueDragon.damage = 5;
	//blueDragon.shootTimerMax = 30;
	//blueDragon.price = 0;
	//blueDragon.isUnlocked = false;

	//this->Dragons.push_back(blueDragon);

	//Texture tex3;
	//if (!tex3.loadFromFile(blueDragon.texturePath)) {
	//	std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	//}
	//this->dragonTextures.push_back(tex3);

	//shop icon
	this->previewSprite.setTexture(this->dragonTextures[itemSelected]);

	this->previewSprite.setScale(0.5f, 0.5f);
	this->previewSprite.setPosition(width / 2.f - this->previewSprite.getGlobalBounds().width / 2.f,
		height / 2.f - 100.f);

	this->equippedIndex = 0;
	this->itemSelected = 0;
	this->updateUI();

}

Shop::~Shop() {

}

void Shop::moveRight() {
	this->itemSelected++;

	if (this->itemSelected >= this->Dragons.size()) {
		this->itemSelected = 0;
	}

	this->updateUI();
}

void Shop::moveLeft() {
	this->itemSelected--;

	if (this->itemSelected < 0) {
		this->itemSelected = this->Dragons.size() - 1;
	}

	this->updateUI();
}

void Shop::initUI() {

	//this->noGold.setFont(font);
	//this->noGold.setCharacterSize(60);
	//this->noGold.setFillColor(Color::White);
	//this->noGold.setPosition(20.f, 20.f);

	//gold amount
	this->goldAmountTex.setFont(font);
	this->goldAmountTex.setCharacterSize(40);
	this->goldAmountTex.setFillColor(Color::Yellow);
	this->goldAmountTex.setPosition(20.f, 20.f);

	this->dragonName.setFont(font);
	this->dragonName.setCharacterSize(60);
	this->dragonName.setFillColor(Color(15, 207, 255));
	//this->dragonName.setPosition(600.f, 450.f);

	this->dragonStats.setFont(font);
	this->dragonStats.setCharacterSize(35);
	this->dragonStats.setFillColor(Color::White);
	//this->dragonStats.setPosition(600.f, 530.f);

	this->priceTag.setFont(font);
	this->priceTag.setCharacterSize(45);
	//this->priceTag.setFillColor(Color::Yellow);
	//this->priceTag.setPosition(600.f, 650.f);
}

void Shop::updateUI() {

	const auto& dragon = Dragons[itemSelected];

	//dragon icon
	this->previewSprite.setTexture(this->dragonTextures[itemSelected]);

	//stats text
	this->dragonName.setString(dragon.name);
	this->dragonStats.setString("HP: " + std::to_string(dragon.hpMax) + "\n"
		+ "Damage: " + std::to_string(dragon.damage));

	//Bought?
	if (!dragon.isUnlocked) {
		this->priceTag.setString("Price: " + std::to_string(dragon.price) + " Gold");
		this->priceTag.setFillColor(Color::Yellow);
	}
	else if (itemSelected == equippedIndex) {
		this->priceTag.setString("EQUIPPED");
		this->priceTag.setFillColor(Color::Cyan);
	}
	else {
		this->priceTag.setString("UNLOCKED (PRESS ENTER TO EQUIP)");
		this->priceTag.setFillColor(Color::Green);
	}

	//Center text
	float centerX = this->background.getSize().x / 2.f;
	this->dragonName.setPosition(centerX - this->dragonName.getGlobalBounds().width / 2.f,
		780.f);
	this->dragonStats.setPosition(centerX - this->dragonStats.getGlobalBounds().width / 2.f,
		850.f);
	this->priceTag.setPosition(centerX - this->priceTag.getGlobalBounds().width / 2.f,
		950.f);
	/*this->noGold.setPosition(centerX - this->noGold.getGlobalBounds().width / 2.f,
		this->previewSprite.getGlobalBounds().height / 2
		- this->noGold.getGlobalBounds().height / 2.f);*/
}

void Shop::draw(RenderWindow& window, int goldAmount) {
	window.draw(this->background);
	window.draw(this->title);

	this->goldAmountTex.setString("GOLD:   " + std::to_string(goldAmount));
	window.draw(this->goldAmountTex);


	window.draw(this->previewSprite);
	window.draw(this->dragonName);
	window.draw(this->dragonStats);
	window.draw(this->priceTag);
}