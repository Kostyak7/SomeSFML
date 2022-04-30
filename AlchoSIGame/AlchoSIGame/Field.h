#pragma once
#ifndef FIELD_H
#define FIELD_H

#include "TextWork.h"

class Field {
public:
	const int width = 1200, height = 650;
	int state = 0;
	sf::RectangleShape main_rect;
	sf::Vector2f position = sf::Vector2f(440, 55);
	Field() {
		main_rect.setSize(sf::Vector2f(width, height));
		main_rect.setFillColor(sf::Color::Transparent);
		main_rect.setOutlineThickness(3);
		main_rect.setOutlineColor(sf::Color(217, 220, 170));
		main_rect.setPosition(position);
	}

	void update(sf::RenderWindow& window, float time, long long stopwatch) {

	}

	void draw(sf::RenderWindow& window) {
		window.draw(main_rect);
	}
};

#endif // !FIELD_H
