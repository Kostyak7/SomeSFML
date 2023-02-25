#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include "TextWork.h"

#include <vector>

class Button :public TextWork
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	std::vector<sf::Texture> textureV;
	std::vector<sf::Color> colorV;
	std::vector<sf::Vector2f> posTextTextureV;
	bool isFirstPush = 1;
	unsigned int state = 0;

	Button() :TextWork() {
		textureV.resize(3);
		colorV.resize(3);
		posTextTextureV.resize(3);
	}

	void chSprite(int onState) {
		sprite.setTexture(textureV[onState]);
	}

	void chTexture(std::string filename) {
		if (texture.loadFromFile(filename)) {
			sprite.setTexture(texture);
		}
		else {
			std::cout << "Button: Error to load texture for button" << std::endl;
		}
	}

	void chScale() {

	}

	void chAllPosition(sf::Vector2f newPosition, sf::Vector2f newPosTextTexture) {
		position = newPosition;
		sprite.setPosition(newPosition);
		chTextPosition(newPosition, newPosTextTexture);
	}

	void createDefault(std::string filename, std::string keyWord, sf::Color RGBA = sf::Color(0, 0, 0), sf::Vector2f newPosTextTexture = sf::Vector2f(0, 0)) {
		createSomething(filename, keyWord, 0, RGBA, newPosTextTexture);
	}

	void setDefault() {
		state = 0;
		setSomething(state);
	}

	void createOnPlace(std::string filename, std::string keyWord, sf::Color RGBA = sf::Color(240, 250, 160), sf::Vector2f newPosTextTexture = sf::Vector2f(0, 0)) {
		createSomething(filename, keyWord, 1, RGBA, newPosTextTexture);
	}

	void setOnPlace() {
		state = 1;
		setSomething(state);
	}

	void createPush(std::string filename, std::string keyWord, sf::Color RGBA = sf::Color(255, 255, 255), sf::Vector2f newPosTextTexture = sf::Vector2f(0, 0)) {
		createSomething(filename, keyWord, 2, RGBA, newPosTextTexture);
	}

	void setPush() {
		state = 2;
		setSomething(state);
	}

	int update(sf::RenderWindow& window, float time) {
		buttonNum = 0;
		if (sf::IntRect(position.x, position.y, texture.getSize().x, texture.getSize().y).contains(sf::Mouse::getPosition(window))) { buttonNum = 1; }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (buttonNum == 1) {
				setPush();
				if (isFirstPush) {
					isFirstPush = 0;
					return state;
				}
				else return 1;
			}
		}
		else {
			isFirstPush = 1;
			if (buttonNum == 1) {
				setOnPlace();
				return state;
			}
		}
		setDefault();

		return 0;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
		window.draw(text);
	}

private:
	int buttonNum = 0;

	void createSomething(std::string filename, std::string keyWord, int ii = 0, sf::Color RGBA = sf::Color(0, 0, 0), sf::Vector2f newPosTextTexture = sf::Vector2f(0, 0)) {
		if (!texture.loadFromFile(filename)) {
			std::cout << "Button: Error to load texture for button" << std::endl;
		}
		textureV[ii] = texture;
		colorV[ii] = RGBA;
		anKeyTextBut(keyWord, &newPosTextTexture);
		posTextTextureV[ii] = newPosTextTexture;
	}

	void setSomething(int newState = 0) {
		chSprite(newState);
		chFontColor(colorV[newState]);
		chAllPosition(position, posTextTextureV[newState]);
	}

	void anKeyTextBut(std::string keyWord, sf::Vector2f* newPosTextTexture) {
		if (keyWord == "old") {
			*newPosTextTexture = posTextTexture;
		}
		if (keyWord == "new") {
		}
		if (keyWord == "mid") {
			std::cout << getTextSizeX() << std::endl;
			(*newPosTextTexture).x = (float(texture.getSize().x) - getTextSizeX()) / 2;
			(*newPosTextTexture).y = (float(texture.getSize().y) - float(fontSize) * 0.45) / 2;
		}
	}
};

#endif // !BUTTON_H