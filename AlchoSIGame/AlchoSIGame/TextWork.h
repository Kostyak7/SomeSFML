#pragma once
#ifndef TEXTWORK_H
#define TEXTWORK_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

template <typename T>
std::string toString(T val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

template <typename T>
T fromString(const std::string& s) {
	std::istringstream iss(s);
	T res;
	iss >> res;
	return res;
}

class TextWork {
public:
	std::string SText;
	sf::Font font;
	sf::Text text;
	sf::Vector2f textPosition = sf::Vector2f(0, 0), posTextTexture = sf::Vector2f(0, 0);
	unsigned int fontSize = 56;
	TextWork() {
		setText();
	}

	void chText(sf::String newText = "IOF") {
		SText = newText;
		text.setString(newText);
		chTextPosition(textPosition, posTextTexture);
	}

	void chFont(std::string filename) {
		if (font.loadFromFile(filename)) {
			text.setFont(font);
		}
		else {
			std::cout << "Button: Error to load font for button" << std::endl;
		}
	}

	void chFontSize(unsigned int newSize = 56) {
		fontSize = newSize;
		text.setCharacterSize(fontSize);
		chTextPosition(textPosition, posTextTexture);
	}

	void chFontColor(sf::Color RGBA = sf::Color(0, 0, 0)) {
		text.setColor(RGBA);
	}

	void chTextPosition(sf::Vector2f newPosition, sf::Vector2f newPosTextTexture) {
		posTextTexture = newPosTextTexture;
		newPosition.y -= float(fontSize) * 0.55;
		textPosition = newPosition + newPosTextTexture;
		text.setPosition(textPosition);
	}

	void chTextRotation(float newAngle) {
		text.setRotation(newAngle);
	}

	void setText(sf::String newText = "IOF", unsigned int newSize = 56, sf::Vector2f newPosition = sf::Vector2f(0, 0), sf::Color RGBA = sf::Color(0, 0, 0)) {
		chText(newText);
		chFontSize(newSize);
		chTextPosition(newPosition, posTextTexture);
		chFontColor(RGBA);
	}

	float getTextSizeX() {
		sf::Vector2f numL = getNumLetter();
		return float(fontSize) * (numL.x * 0.37 + numL.y * 0.25);
	}

private:
	sf::Vector2f getNumLetter() {
		int BigLet = 0, SmallLet = 0;
		for (int i = 0; i < SText.length(); i++) {
			if (SText[i] <= 'Z' && SText[i] >= 'A') BigLet++;
			else SmallLet++;
		}
		return sf::Vector2f(BigLet, SmallLet);
	}
};

#endif // !TEXTWORK_H