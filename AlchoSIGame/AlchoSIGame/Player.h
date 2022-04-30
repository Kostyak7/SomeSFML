#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "TextBox.h"

class Player {
public:
	int number = 0;
	int score = 0;
	unsigned int state = 0;
	sf::String name = "Player_";
	sf::Sprite img_spr;
	sf::Texture img_txtr;
	sf::Vector2f position = sf::Vector2f(0, 750);
	sf::RectangleShape outLine;
	TextBox name_box, score_box;
	Player(){}

	void createPlayer(int num_, int num_of_plyrs, std::string name_, std::string name_img) {
		number = num_;
		name = name_;
		sf::Image img;
		img.loadFromFile(name_img);
		img_txtr.loadFromImage(img);
		img_spr.setTexture(img_txtr);
		float prW = img.getSize().x, prH = img.getSize().y;
		if (prW >= prH) {
			img_spr.setScale(200 / prW, 200 / prW);
			prW = 200;
		}
		else {
			img_spr.setScale(200 / prH, 200 / prH);
			prW *= 200 / prH;
		}
		position.x = 200 - prW/2 + 760 * (2 * num_ + 1) / num_of_plyrs;
		img_spr.setPosition(position);

		std::cout << position.x << std::endl;

		name_box.chFont("files/textFont/PixelFont 1.ttf");
		name_box.chFontSize(60);
		name_box.chText(name);
		name_box.chFontColor(sf::Color::White);
		name_box.chTextPosition(sf::Vector2f(position.x - 100 + prW / 2, position.y + 200), sf::Vector2f(60, 10));

		score_box.chFont("files/textFont/PixelFont 1.ttf");
		score_box.chFontSize(50);
		score_box.chText(toString<int>(score));
		score_box.chFontColor(sf::Color::White);
		score_box.chTextPosition(sf::Vector2f(position.x - 90 + prW / 2, position.y + 250), sf::Vector2f(60, 10));

		outLine.setSize(sf::Vector2f(250, 340));
		outLine.setFillColor(sf::Color::Transparent);
		outLine.setOutlineThickness(3);
		outLine.setOutlineColor(sf::Color(217, 220, 170));
		outLine.setPosition(sf::Vector2f(position.x - 25, position.y - 5));

		std::cout << "prW " << prW << std::endl;
	}

	void chPosition(sf::Vector2f new_pos) {
		position = new_pos;
		img_spr.setPosition(position);
		name_box.chTextPosition(sf::Vector2f(position.x, position.y + 200), sf::Vector2f(60, 10));
		score_box.chTextPosition(sf::Vector2f(position.x + 10, position.y + 250), sf::Vector2f(60, 10));
	}

	void update(sf::RenderWindow& window, float time, bool *isOtherBusy) {
		
		if (number != -1) {
			if (state != 3) {
				state = 0;
				if (sf::IntRect(position.x - 25, position.y - 5, 250, 340).contains(sf::Mouse::getPosition(window))) { state = 1; }
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (state) {
					if (!*isOtherBusy) {
						*isOtherBusy = 1;
						state = 3;
					}
					else {
						if (state != 3) state = 2;
					}
				}
			}

			if (state == 1) outLine.setOutlineColor(sf::Color(217, 220, 170));
			if (state == 2 || state == 3) outLine.setOutlineColor(sf::Color(220, 170, 170));
		}
	}

	void draw(sf::RenderWindow& window) {
		window.draw(img_spr);
		name_box.draw(window);
		if (number != -1) {
			if (state) window.draw(outLine);
			score_box.draw(window);
		}
	}

};

#endif // !PLAYER_H
