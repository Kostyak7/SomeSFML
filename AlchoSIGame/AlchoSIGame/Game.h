#pragma once
#ifndef GAME_H
#define GAME_H

#include "WindowA.h"
#include "Button.h"
#include "TextBox.h"
#include "Player.h"

#include <SFML/Audio.hpp>

class Game :public WindowA {
public:
	int isGame = 0;
	Game() :WindowA() {
		isGame = 1;
	}

	void gameGo() {
		Button btn;
		btn.chFont("files/textFont/PixelFont 1.ttf");
		btn.chFontSize(70);
		btn.chText("End");
		btn.chAllPosition(sf::Vector2f(100, 100), sf::Vector2f(200, 40));

		btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		btn.createPush("files/images/menu/ButtonPush.png", "mid");

		//Player player;
		//player.createPlayer(0, "Olya", "files/images/person/KOPOLYA.png");

		while (isGame) {
			firstlyA(&isGame);

			preUpdateA();
			if (btn.update(window, time) == 2) {
				std::cout << "I love Olga" << std::endl;
				window.close();
				return;
			}

			btn.draw(window);
			//player.draw(window);

			updateA();
			window.display();
		}
	}

};

#endif // !GAME_H
