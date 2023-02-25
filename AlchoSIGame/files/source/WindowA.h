#pragma once
#ifndef WINDOWA_H
#define WINDOWA_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class WindowA
{
public:
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	int scrW = desktop.width, scrH = desktop.height;
	float time = 0;
	bool isOtherCursor = 0;
	sf::RenderWindow window;
	sf::Sprite cursor_Sp;
	sf::Texture cursor_x;
	sf::Clock clock;
	sf::Vector2f pos_cr;
	WindowA() {
		window.create(sf::VideoMode(scrW, scrH, desktop.bitsPerPixel), "AlchoSIGame", sf::Style::Default);
	}
	void chIcon(std::string filename) {
		sf::Image imageIcon;
		if (imageIcon.loadFromFile(filename)) {
			std::cout << imageIcon.getSize().x << "\t" << imageIcon.getSize().y << std::endl;
			window.setIcon(imageIcon.getSize().x, imageIcon.getSize().y, imageIcon.getPixelsPtr());
		}
		else {
			std::cout << "WindowA: Error to load icon of app" << std::endl;
		}
	}

	void chTitle(sf::String title) {
		window.setTitle(title);
	}

	void chFramerateLimit(unsigned int limit) {
		window.setFramerateLimit(limit);
	}

	void chCursor(std::string filename) {
		if (!cursor_x.loadFromFile(filename)) {
			std::cout << "WindowA: Error to load image of cursor" << std::endl;
		}
		else {
			cursor_Sp.setTexture(cursor_x);
			window.setMouseCursorVisible(0);
			isOtherCursor = 1;
		}
	}

	void chBackCursor() {
		window.setMouseCursorVisible(1);
		isOtherCursor = 0;
	}

	int checkEvent() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); return 0; }
			if (event.type == sf::Event::Resized) { window.setSize(sf::Vector2u(scrW, scrH)); }
		}

		return 1;
	}

	void firstlyA(int* isBool) {
		*isBool = checkEvent();

		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
	}

	void preUpdateA(sf::Color RGBA = sf::Color(0, 0, 0)) {
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		pos_cr = window.mapPixelToCoords(pixelPos);

		if (isOtherCursor) {
			cursor_Sp.setPosition(pos_cr.x, pos_cr.y);
		}

		window.clear(RGBA);
	}

	void updateA() {


		if (isOtherCursor) window.draw(cursor_Sp);
	}
};

#endif // !WINDOWA_H
