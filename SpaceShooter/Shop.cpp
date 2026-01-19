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
	defaultDragon.name = "La Lloyd";
	defaultDragon.texturePath = "Textures/player/Green_Dragon_overhead.png";
	defaultDragon.hpMax = 7;
	defaultDragon.damage = 1;
	defaultDragon.shootTimerMax = 25;
	defaultDragon.price = 0;
	defaultDragon.isUnlocked = true;
	defaultDragon.scale = 0.15f;
	defaultDragon.speed = 7.0f;

	this->Dragons.push_back(defaultDragon);

	Texture tex;
	if (!tex.loadFromFile(defaultDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex);


	//DRAGON 2
	DragonProfile redDragon;
	redDragon.name = "Koksik";
	redDragon.texturePath = "Textures/player/Red_Dragon.png";
	redDragon.hpMax = 20;
	redDragon.damage = 3;
	redDragon.shootTimerMax = 35;
	redDragon.price = 100;
	redDragon.isUnlocked = false;
	redDragon.scale = 0.15f;
	redDragon.speed = 11.0f;

	this->Dragons.push_back(redDragon);

	Texture tex2;
	if (!tex2.loadFromFile(redDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex2);


	//DRAGON 3
	DragonProfile blueDragon;
	blueDragon.name = "Bluey";
	blueDragon.texturePath = "Textures/player/dragon_blue.png";
	blueDragon.hpMax = 20;
	blueDragon.damage = 5;
	blueDragon.shootTimerMax = 30;
	blueDragon.price = 200;
	blueDragon.isUnlocked = false;
	blueDragon.scale = 0.15f;
	blueDragon.speed = 5.5f;

	this->Dragons.push_back(blueDragon);

	Texture tex3;
	if (!tex3.loadFromFile(blueDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex3);


	//DRAGON 4
	DragonProfile whiteDragon;
	whiteDragon.name = "Flopek";
	whiteDragon.texturePath = "Textures/player/white_dragon.png";
	whiteDragon.hpMax = 5;
	whiteDragon.damage = 1;
	whiteDragon.shootTimerMax = 15;
	whiteDragon.price = 300;
	whiteDragon.isUnlocked = false;
	whiteDragon.scale = 0.15f;
	whiteDragon.speed = 10.0f;

	this->Dragons.push_back(whiteDragon);

	Texture tex4;
	if (!tex4.loadFromFile(whiteDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex4);

	//DRAGON 5
	DragonProfile skyrimDragon;
	skyrimDragon.name = "Rusty";
	skyrimDragon.texturePath = "Textures/player/Skyrim_Dragon.png";
	skyrimDragon.hpMax = 20;
	skyrimDragon.damage = 7;
	skyrimDragon.shootTimerMax = 40;
	skyrimDragon.price = 350;
	skyrimDragon.isUnlocked = false;
	skyrimDragon.scale = 0.20f;
	skyrimDragon.speed = 9.0f;


	this->Dragons.push_back(skyrimDragon);

	Texture tex5;
	if (!tex5.loadFromFile(skyrimDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex5);

	//DRAGON 6
	DragonProfile coolDragon;
	coolDragon.name = "Jaszczur";
	coolDragon.texturePath = "Textures/player/cool_dragon.png";
	coolDragon.hpMax = 20;
	coolDragon.damage = 9;
	coolDragon.shootTimerMax = 30;
	coolDragon.price = 400;
	coolDragon.isUnlocked = false;
	coolDragon.scale = 0.14f;
	coolDragon.speed = 12.0f;


	this->Dragons.push_back(coolDragon);

	Texture tex6;
	if (!tex6.loadFromFile(coolDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex6);

	//DRAGON 7
	DragonProfile enderDragon;
	enderDragon.name = "Ender Dragon";
	enderDragon.texturePath = "Textures/player/Ender_Dragon.png";
	enderDragon.hpMax = 30;
	enderDragon.damage = 20;
	enderDragon.shootTimerMax = 20;
	enderDragon.price = 1000;
	enderDragon.isUnlocked = false;
	enderDragon.scale = 0.29f;
	enderDragon.speed = 11.0f;


	this->Dragons.push_back(enderDragon);

	Texture tex7;
	if (!tex7.loadFromFile(enderDragon.texturePath)) {
		std::cout << "Blad ladowania tekstury smoka!" << std::endl;
	}
	this->dragonTextures.push_back(tex7);

	//shop icon
	//this->previewSprite.setTexture(this->dragonTextures[itemSelected]);

	//this->previewSprite.setScale(0.5f, 0.5f);
	//this->previewSprite.setPosition(width / 2.f - this->previewSprite.getGlobalBounds().width / 2.f,
	//	height / 2.f - 100.f);

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
	this->previewSprite.setTexture(this->dragonTextures[itemSelected], true);


	FloatRect spriteBounds = this->previewSprite.getLocalBounds();
	this->previewSprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);

	//dragon size in shop
	float maxAllowedWidth = 700.f;
	float maxAllowedHeight = 500.f;

	//scale
	float scaleX = maxAllowedWidth / spriteBounds.width;
	float scaleY = maxAllowedHeight / spriteBounds.height;
	float finalScale = std::min(scaleX, scaleY);

	if (finalScale > 0.6f) finalScale = 0.6f;

	this->previewSprite.setScale(finalScale, finalScale);

	this->previewSprite.setPosition(
		this->background.getSize().x / 2.f,
		this->background.getSize().y / 2.f); //- 100.f


	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << dragon.speed;
	std::string speedFormatted = ss.str();

	//stats text
	this->dragonName.setString(dragon.name);
	this->dragonStats.setString("HP: " + std::to_string(dragon.hpMax) + "\n"
		+ "Damage: " + std::to_string(dragon.damage) + "\n"
		+ "Speed: " + speedFormatted);

	//Bought?
	if (!dragon.isUnlocked) {
		this->priceTag.setString("Price: " + std::to_string(dragon.price)
			+ " Gold (PRESS ENTER TO BUY)");
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
		985.f);

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