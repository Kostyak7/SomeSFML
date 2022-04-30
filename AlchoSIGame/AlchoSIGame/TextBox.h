#pragma once
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "TextWork.h"

class TextBox : public TextWork {
public:
	bool isCompare = 0;
	TextBox() :TextWork() {	}

	void chPosition(float X, float Y) {
		position = sf::Vector2f(X, Y);
		chTextPosition(position, sf::Vector2f(0, 0));
	}

	sf::Vector2i chSizeTBox(float size_X, float size_Y) {
		size_X = (size_X < MIN_S_X) ? MIN_S_X : size_X;
		size_Y = (size_Y < MIN_S_Y) ? MIN_S_Y : size_Y;
		if (isCompare) {
			float textSize = getTextSizeX();

		}

		return sf::Vector2i(1, 0);
	}

	void checkReturnSizeBox(sf::Vector2i retFunc) {
		switch (retFunc.x) {
		case 1:
			if (retFunc.y == 1) {
				std::cout << "TextBox:	";
			}
			break;

		default:
			std::cout << "TextBox:	Unknown function\n";
			break;
		}
	}

	sf::Vector2f getSizeTBox() {
		return sizeTBox;
	}

	sf::Vector2f getPositionTBox() {
		return position;
	}

	void update(sf::RenderWindow& window, float time) {

	}

	void draw(sf::RenderWindow& window) {
		window.draw(text);
	}


private:
	const float MIN_S_X = 100, MIN_S_Y = 56;
	sf::Vector2f sizeTBox = sf::Vector2f(MIN_S_X, MIN_S_Y);
	sf::Vector2f position = sf::Vector2f(0, 0);
};

#endif // !TEXTBOX_H
