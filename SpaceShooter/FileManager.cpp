#include "FileManager.h"

const std::string FileManager::filenames[number_of_slots] = {
	"Files/slot1.txt",
	"Files/slot2.txt",
	"Files/slot3.txt",
	"Files/slot4.txt",
	"Files/slot5.txt"
};

FileManager::FileManager(float width, float height)
{
	if(!mainTexture.loadFromFile("Textures/background/shop_background.png")) {
		std::cout << "B³¹d ³adowania t³a!" << std::endl;
	}

	this->background.setSize(Vector2f(width, height));
	this->background.setTexture(&this->mainTexture);

	this->background2.setSize(Vector2f(450, 450));
	this->background2.setPosition(width / 2 - 250, height / 2 - 250);
	this->background2.setFillColor(Color::Black);

	if (!font.loadFromFile("Assets/TrovicalCalmFreeItalic-aYAZx.otf")) {
		std::cout << "No font" << std::endl;
	}

	//slotRequest
	slot_choice.setFont(font);
	slot_choice.setFillColor(Color(15, 207, 255));
	slot_choice.setString("Choose a slot");
	slot_choice.setCharacterSize(70);
	slot_choice.setPosition((width - slot_choice.getGlobalBounds().width) / 2-30,height / 2 - 230);

	//slot 1
	this->slots[0].setFont(font);
	this->slots[0].setFillColor(Color(15, 207, 255));
	this->slots[0].setString("1   ");
	this->slots[0].setCharacterSize(50);
	this->slots[0].setPosition((width - this->slots[0].getGlobalBounds().width) / 2-180,height / 2-100);

	//slot 2
	this->slots[1].setFont(font);
	this->slots[1].setFillColor(Color::White);
	this->slots[1].setString("2   ");
	this->slots[1].setCharacterSize(50);
	this->slots[1].setPosition((width - this->slots[1].getGlobalBounds().width) / 2-180, height / 2-50);

	//slot 3
	this->slots[2].setFont(font);
	this->slots[2].setFillColor(Color::White);
	this->slots[2].setString("3   ");
	this->slots[2].setCharacterSize(50);
	this->slots[2].setPosition((width - this->slots[2].getGlobalBounds().width) / 2-180, height / 2 );

	//slot 4
	this->slots[3].setFont(font);
	this->slots[3].setFillColor(Color::White);
	this->slots[3].setString("4   ");
	this->slots[3].setCharacterSize(50);
	this->slots[3].setPosition((width - this->slots[3].getGlobalBounds().width) / 2-180, height / 2 +50);

	//slot 5
	this->slots[4].setFont(font);
	this->slots[4].setFillColor(Color::White);
	this->slots[4].setString("5   ");
	this->slots[4].setCharacterSize(50);
	this->slots[4].setPosition((width - this->slots[4].getGlobalBounds().width) / 2-180, height / 2 + 100);


	this->slotSelected = 0;

}

FileManager::~FileManager()
{

}

void FileManager::UpdateSlotUI()
{
	for (int i = 0; i < number_of_slots; i++)
	{
		std::ifstream file(filenames[i]);
		std::string playerName;

		if (file.is_open())
		{
			if (file >> playerName && playerName != "")
			{
				this->slots[i].setString(std::to_string(i + 1) + "     " + playerName);
			}
			else
			{
				this->slots[i].setString(std::to_string(i + 1) + "     <EMPTY>");
			}
			file.close();
		}
		else this->slots[i].setString(std::to_string(i + 1) + "     <EMPTY>");
	}
}


bool FileManager::LoadFile(int slotSelected, std::string& playerName,
	int& level, int& hp, int& exp, int& Totalgold, int& score, int& dragon_index) {

	std::ifstream file(filenames[slotSelected]);

	if (file.is_open())
	{
		if (file >> playerName >> level >> hp >> exp >> Totalgold >> score >> dragon_index)
		{
			//std::cout << "wczytano slot" << std::endl;
			return true;
		}
		else
		{
			//std::cout << "nie wczytano slotu" << std::endl;
			return false;
		}

		file.close();
	}
	else
	{
		//std::cout << "nie mozna otworzyc pliku" << std::endl;
		return false;
	}
}

void FileManager::SaveFile(int slotSelected, std::string playerName,
	int level, int hp, int exp, int Totalgold, int score, int dragon_index)
{
	std::ofstream file(filenames[slotSelected]);

	if (file.is_open())
	{
		file << playerName << " " << level << " " << hp << " " << exp << " " << Totalgold << " " << score << " " << dragon_index;
		file.close();
	    
		//std::cout << "zapisano slot " <<slotSelected<< std::endl;
	}
	//else std::cout << "nie wczytano slotu" << std::endl;
}

//Draw menu
void FileManager::draw(RenderWindow& window) {
	window.draw(this->background);
	window.draw(this->background2);
	window.draw(this->slot_choice);
	for (int i = 0; i < number_of_slots; i++) 
	{
		window.draw(this->slots[i]);
	}
}


//MoveUp
void FileManager::moveUp() {
	if (this->slotSelected - 1 >= 0) {

		this->slots[this->slotSelected].setFillColor(Color::White);
		this->slotSelected--;
		
		this->slots[this->slotSelected].setFillColor(Color(15, 207, 255));
	}
}

//MoveDown
void FileManager::moveDown() {

	if (this->slotSelected + 1 < number_of_slots)
	{

		this->slots[this->slotSelected].setFillColor(Color::White);
		this->slotSelected++;

		this->slots[this->slotSelected].setFillColor(Color(15, 207, 255));
	}
}