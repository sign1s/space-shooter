#include <iostream>
#include <cstdlib>
#include <time.h>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Main_menu.h"
#include "PauseMenu.h"
#include "GameOverMenu.h"
#include "Shop.h"

using namespace sf;



int main()
{
	enum class State { MENU, PLAYING, SHOPPING, PAUSE, GAMEOVER };
	State currentState = State::MENU;

	RenderWindow window(VideoMode({ 1920,1080 }), "dragon shooter!");

	//main menu
	Main_menu Menu(window.getSize().x, window.getSize().y);

	//pause menu
	PauseMenu Pause(window.getSize().x, window.getSize().y);

	//gameover menu
	GameOverMenu OverScreen(window.getSize().x, window.getSize().y);

	//shop
	Shop shop(window.getSize().x, window.getSize().y);

	Game game(&window, shop.getSelectedDragon());

	srand(time(NULL));

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			/*if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();*/
			if (currentState == State::MENU) {

				if (event.type == Event::KeyReleased) {

					switch (event.key.code) {
					case Keyboard::Up:
						Menu.moveUp();
						break;
					case Keyboard::Down:
						Menu.moveDown();
						break;
					case Keyboard::Enter:
						int x = Menu.mainMenuPressed();
						if (x == 0) {
							const DragonProfile& activeDragon = shop.getDragon(shop.getEquippedIndex());//Dragons[shop.getEquippedIndex()];
							game.Reset(activeDragon);
							currentState = State::PLAYING;
						}
						if (x == 1) currentState = State::SHOPPING;
						if (x == 2) window.close();
						break;
					}
				}
			}
			else if (currentState == State::SHOPPING) {
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
					currentState = State::MENU;
				if (event.type == Event::KeyReleased) {
					if (event.key.code == Keyboard::Left) shop.moveLeft();
					if (event.key.code == Keyboard::Right) shop.moveRight();
					if (event.key.code == Keyboard::Enter) {
						int x = shop.itemPressed();
						const DragonProfile& selectedDragon = shop.getSelectedDragon();
						if (!selectedDragon.isUnlocked) {
							if (game.enoughGold(selectedDragon.price)) {
								shop.unlockDragon(x);
								//std::cout << "Kupiono smoka: " << selectedDragon.name << std::endl;
							}
							else {
								game.showText("NOT ENOUGH GOLD!");
								//std::cout << "Nie masz wystarczajacej ilosci zlota!" << std::endl;
							}
						}
						else {
							//wybieranie skina
							shop.equipDragon(x);
							std::cout << "Zmieniono smoka na: " << selectedDragon.name << std::endl;
							//std::cout << "Masz juz tego smoka!" << std::endl;
							shop.updateUI();
						}
					}

				}
			}
			else if (currentState == State::PLAYING) {
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
					Pause.resetPause();
					currentState = State::PAUSE;
				}
			}
			else if (currentState == State::PAUSE) {
				if (event.type == Event::KeyReleased) {
					if (event.key.code == Keyboard::Up) Pause.moveUp();
					if (event.key.code == Keyboard::Down) Pause.moveDown();
					if (event.key.code == Keyboard::Enter) {
						int x = Pause.pausePressed();
						if (x == 0) currentState = State::PLAYING; 
						if (x == 1) currentState = State::MENU; 					
					}
				}
			}
			else if (currentState == State::GAMEOVER) {
				if (event.type == Event::KeyReleased) {
					if (event.key.code == Keyboard::Up) OverScreen.moveUp();
					if (event.key.code == Keyboard::Down) OverScreen.moveDown();
					if (event.key.code == Keyboard::Enter) {
						int x = OverScreen.buttonPressed();
						if (x == 0) {
							game.Reset(shop.getSelectedDragon());
							currentState = State::PLAYING;
						}
						if (x == 1) currentState = State::MENU;					
					}
				}
			}
		}

		//DRAWING
		window.clear();

		if (currentState == State::MENU) {
			Menu.draw(window);
		}
		else if (currentState == State::SHOPPING) {
			game.updateNotifications();
			shop.draw(window, game.getTotalGold());
			game.drawNotifications();
		}
		else if (currentState == State::PLAYING) {
			game.Update();
			if (game.getGameOver()) {
				currentState = State::GAMEOVER;
			}
			game.Draw();
			game.drawNotifications();
		}
		else if (currentState == State::PAUSE) {
			game.Draw();
			Pause.draw(window);
		}
		else if (currentState == State::GAMEOVER) {
			game.Draw();
			OverScreen.draw(window);
		}


		window.display();
	}
}