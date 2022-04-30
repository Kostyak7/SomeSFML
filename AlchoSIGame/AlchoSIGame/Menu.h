#pragma once
#ifndef MENU_H
#define MENU_H

#include "WindowA.h"
#include "Button.h"
#include "Game.h"
#include "Field.h"

#include <vector>
#include <chrono>
#include <SFML/Audio.hpp>

class Menu :public WindowA
{
public:
	int isMenu = 0;
	Menu() :WindowA() {
		isMenu = 1;
	}

	void menuGo() {
		int num_of_players = 3;
		bool isPlayerBusy = 0;
		auto startStopwatch = std::chrono::high_resolution_clock::now();

		Button start_btn, exit_btn;
		start_btn.chFont("files/textFont/PixelFont 1.ttf");
		start_btn.chFontSize(70);
		start_btn.chText("Start Game");
		start_btn.chAllPosition(sf::Vector2f(750, 350), sf::Vector2f(200, 40));

		start_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		start_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		start_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		exit_btn.chFont("files/textFont/PixelFont 1.ttf");
		exit_btn.chFontSize(70);
		exit_btn.chText("Exit");
		exit_btn.chAllPosition(sf::Vector2f(750, 550), sf::Vector2f(200, 40));

		exit_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		exit_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		exit_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		std::vector<Player> player(num_of_players);
		player[0].createPlayer(0, num_of_players, "Olya", "files/images/person/kopolya.png");
		player[1].createPlayer(1, num_of_players, "Kostya", "files/images/person/kostya.png");
		player[2].createPlayer(2, num_of_players, "Platypus", "files/images/person/platypus.png");

		Player game_host;
		game_host.createPlayer(-1, 1, "skiPIDAR", "files/images/person/host.jpg");
		game_host.chPosition(sf::Vector2f(80, 300));
		game_host.name_box.chPosition(75, 500);

		Field field;

		while (isMenu) {
			firstlyA(&isMenu);
			long long stopwatch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startStopwatch).count();

			//////////UPDATE////////////////
			preUpdateA(sf::Color(0, 5, 90));
			if (exit_btn.update(window, time) == 2) {
				std::cout << "Exit" << std::endl;
				window.close();
				return;
			}
			if (start_btn.update(window, time) == 2) {
				std::cout << "Start Game" << std::endl;

				//Game game;
				//game.gameGo();
				//std::cout << "End Game" << std::endl;
			}
			field.update(window, time, stopwatch);

			player[0].update(window, time, &isPlayerBusy);
			player[1].update(window, time, &isPlayerBusy);
			player[2].update(window, time, &isPlayerBusy);

			game_host.update(window, time, &isPlayerBusy);

			//////////DRAW///////////////
			field.draw(window);

			player[0].draw(window);
			player[1].draw(window);
			player[2].draw(window);

			game_host.draw(window);

			start_btn.draw(window);
			exit_btn.draw(window);

			updateA();
			window.display();
		}
	}

private:

};

#endif // !MENU_H