#pragma once
#ifndef MENU_H
#define MENU_H

#include "WindowA.h"
#include "Button.h"
#include "Game.h"
#include "Field.h"

#include <vector>
#include <chrono>

#define NUM_OF_ROUNDS 2
#define NUM_OF_PLAYERS 3

class Menu :public WindowA
{
public:
	int isMenu = 0;
	Menu() :WindowA() {
		isMenu = 1;
	}

	void menuGo() {
		setlocale(LC_ALL, "rus");
		int stateField = 0;
		//int num_of_players = 3;
		int round = 1;
		bool isPlayerBusy = 0;
		auto startStopwatch = std::chrono::high_resolution_clock::now();

		Button start_btn, exit_btn;
		start_btn.chFont("files/textFont/PixelFont 1.ttf");
		start_btn.chFontSize(70);
		start_btn.chText(L"Начать игру");
		start_btn.chAllPosition(sf::Vector2f(750, 350), sf::Vector2f(200, 40));

		start_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		start_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		start_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		exit_btn.chFont("files/textFont/PixelFont 1.ttf");
		exit_btn.chFontSize(70);
		exit_btn.chText(L"Выход");
		exit_btn.chAllPosition(sf::Vector2f(750, 550), sf::Vector2f(200, 40));

		exit_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		exit_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		exit_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		Button nextRound_btn;
		nextRound_btn.chFont("files/textFont/PixelFont 1.ttf");
		nextRound_btn.chFontSize(70);
		nextRound_btn.chText(L"Следующий раунд");
		nextRound_btn.chAllPosition(sf::Vector2f(1700, 200), sf::Vector2f(200, 40));

		nextRound_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		nextRound_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		nextRound_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		std::vector<Player> player(NUM_OF_PLAYERS);
		player[0].createPlayer(0, NUM_OF_PLAYERS, "Olya", "files/images/person/kopolya.png");
		player[1].createPlayer(1, NUM_OF_PLAYERS, "Kostya", "files/images/person/kostya.png");
		player[2].createPlayer(2, NUM_OF_PLAYERS, "Platypus", "files/images/person/platypus.png");

		Player game_host;
		game_host.createPlayer(-1, 1, "skiPIDAR", "files/images/person/host.jpg");
		game_host.chPosition(sf::Vector2f(80, 300));
		game_host.name_box.chPosition(75, 500);

		//Field field(1);
		Field* field = new Field(round);

		while (isMenu) {
			firstlyA(&isMenu);
			long long stopwatch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startStopwatch).count();

			//////////UPDATE////////////////
			preUpdateA(sf::Color(0, 5, 90));
			if (exit_btn.update(window, time) == 2) {
				std::wcout << L"Выход" << std::endl;
				delete field;
				window.close();
				return;
			}
			if (start_btn.update(window, time) == 2) {
				std::wcout << L"Начать игру" << std::endl;

				//window.close();
				//Game game;
				//game.gameGo();
				//std::cout << "End Game" << std::endl;
			}
			if (nextRound_btn.update(window, time) == 2) {
				std::wcout << L"Следующий раунд" << std::endl;
				if (round < NUM_OF_ROUNDS && !isPlayerBusy) {
					round++;
					delete field;
					field = new Field(round);
				}
			}
			for (int i = 0 , count = 0; i < 3; i++) {
				if (player[i].state == 3) {
					stateField = field->update(window, time, stopwatch, &player[i]);
					isPlayerBusy = 0;
					break;
				}
				count++;
				if (count == 3) stateField = field->update(window, time, stopwatch, NULL);
			}


			player[0].update(window, time, &isPlayerBusy, stateField != 0);
			player[1].update(window, time, &isPlayerBusy, stateField != 0);
			player[2].update(window, time, &isPlayerBusy, stateField != 0);

			game_host.update(window, time, &isPlayerBusy, 0);

			//////////DRAW///////////////
			field->draw(window);

			player[0].draw(window);
			player[1].draw(window);
			player[2].draw(window);

			game_host.draw(window);

			start_btn.draw(window);
			exit_btn.draw(window);
			nextRound_btn.draw(window);

			updateA();
			window.display();
		}
	}

private:

};

#endif // !MENU_H