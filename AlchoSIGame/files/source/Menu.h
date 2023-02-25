#pragma once
#ifndef MENU_H
#define MENU_H

#include "WindowA.h"
#include "Button.h"
#include "Game.h"

class Menu :public WindowA
{
public:
	int isMenu = 0;
	Menu() :WindowA() {
		isMenu = 1;
	}

	void menuGo() {
		Button start_btn, exit_btn;
		start_btn.chFont("files/textFont/UZSans-Regular.ttf");
		start_btn.chFontSize(45);
		start_btn.chText(L"Начать игру");
		start_btn.chAllPosition(sf::Vector2f(750, 350), sf::Vector2f(200, 40));

		start_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		start_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		start_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		exit_btn.chFont("files/textFont/UZSans-Regular.ttf");
		exit_btn.chFontSize(45);
		exit_btn.chText(L"Выход");
		exit_btn.chAllPosition(sf::Vector2f(750, 550), sf::Vector2f(200, 40));

		exit_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		exit_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		exit_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		while (isMenu) {
			firstlyA(&isMenu);

			//////////UPDATE////////////////
			preUpdateA(sf::Color(0, 5, 90));
			if (exit_btn.update(window, time) == 2) {
				std::wcout << L"Выход" << std::endl;
				window.close();
				return;
			}
			if (start_btn.update(window, time) == 2) {
				std::wcout << L"Начать игру" << std::endl;

				window.close();
				Game game;
				game.gameGo();
				std::cout << "End Game" << std::endl;
			}

			start_btn.draw(window);
			exit_btn.draw(window);

			updateA();
			window.display();
		}
	}

private:

};

#endif // !MENU_H