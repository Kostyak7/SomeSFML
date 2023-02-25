#pragma once
#ifndef GAME_H
#define GAME_H

#include "WindowA.h"
#include "Button.h"
#include "TextBox.h"
#include "Player.h"
#include "Field.h"
#include "FinalField.h"

#include <vector>
#include <chrono>

#include <SFML/Audio.hpp>


#define NUM_OF_ROUNDS 2
#define NUM_OF_PLAYERS 5

class Game :public WindowA {
public:
	int isGame = 0;
	Game() :WindowA() {
		isGame = 1;
	}

	void gameGo() {
		setlocale(LC_ALL, "rus");
		int stateField = 0;
		int round = 1;
		int isPlayerBusy = 0;
		auto startStopwatch = std::chrono::high_resolution_clock::now();

		Button btn;
		btn.chFont("files/textFont/PixelFont 1.ttf");
		btn.chFontSize(50);
		btn.chText("End");
		btn.chAllPosition(sf::Vector2f(20, 50), sf::Vector2f(200, 40));

		btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		btn.createPush("files/images/menu/ButtonPush.png", "mid");

		Button nextRound_btn;
		nextRound_btn.chFont("files/textFont/UZSans-Regular.ttf");
		nextRound_btn.chFontSize(45);
		nextRound_btn.chText(L"Следующий раунд");
		nextRound_btn.chAllPosition(sf::Vector2f(1700, 200), sf::Vector2f(200, 40));

		nextRound_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		nextRound_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		nextRound_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		Player player[NUM_OF_PLAYERS];
		player[0].createPlayer(0, NUM_OF_PLAYERS, "Olga", "files/images/person/kopolya.png");
		player[1].createPlayer(1, NUM_OF_PLAYERS, "Katya", "files/images/person/Katya.png");
		player[2].createPlayer(2, NUM_OF_PLAYERS, "Vlad", "files/images/person/Vlad.png");
		player[3].createPlayer(3, NUM_OF_PLAYERS, "Kolya", "files/images/person/Kolya.png");
		player[4].createPlayer(4, NUM_OF_PLAYERS, "Alexey", "files/images/person/Alexey.png");

		Player game_host;
		game_host.createPlayer(-1, 1, "skiPIDAR", "files/images/person/host.jpg");
		game_host.chPosition(sf::Vector2f(80, 300));
		game_host.name_box.chPosition(75, 500);

		Field* field = new Field(round, player);
		FinalField* F_field = NULL;

		while (isGame) {
			firstlyA(&isGame);
			long long stopwatch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startStopwatch).count();

			preUpdateA(sf::Color(0, 5, 90));
			if (btn.update(window, time) == 2) {
				delete field;
				delete F_field;
				window.close();
				return;
			}
			if (nextRound_btn.update(window, time) == 2) {
				std::wcout << L"Следующий раунд" << std::endl;
				round++;
				if (round <= NUM_OF_ROUNDS && !isPlayerBusy) {
					delete field;
					field = new Field(round, player);
				}
				if (round > NUM_OF_ROUNDS && !isPlayerBusy) {
					delete field;
					if (F_field) delete F_field;
					F_field = new FinalField();
				}
			}

			if (round <= NUM_OF_ROUNDS) stateField = field->update(window, time, stopwatch, &isPlayerBusy);
			if (round > NUM_OF_ROUNDS) stateField = F_field->update(window, time, stopwatch);


			player[0].update(window, time, &isPlayerBusy, stateField != 0);
			player[1].update(window, time, &isPlayerBusy, stateField != 0);
			player[2].update(window, time, &isPlayerBusy, stateField != 0);
			player[3].update(window, time, &isPlayerBusy, stateField != 0);
			player[4].update(window, time, &isPlayerBusy, stateField != 0);

			game_host.update(window, time, &isPlayerBusy, 0);

			//////////DRAW///////////////
			btn.draw(window);
			if (round <= NUM_OF_ROUNDS) field->draw(window);
			if (round > NUM_OF_ROUNDS) F_field->draw(window);

			player[0].draw(window);
			player[1].draw(window);
			player[2].draw(window);
			player[3].draw(window);
			player[4].draw(window);

			game_host.draw(window);

			nextRound_btn.draw(window);;

			updateA();
			window.display();
		}
		delete F_field;
	}

};

#endif // !GAME_H
