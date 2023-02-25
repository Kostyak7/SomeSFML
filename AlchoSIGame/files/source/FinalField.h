#pragma once
#pragma once
#ifndef FINALFIELD_H
#define FINALFIELD_H

#define FILE_NAME_F "files/FinalPack.txt"
#define PATH_IMG_F L"files/images/rounds/" 

#include "TextWork.h"
#include "Button.h"
#include "Player.h"
#include "MyMusic.h"
#include <fstream>


class FinalCell {
public:
	bool isChoose = 0, isDelete = 0;
	int state = 0;
	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::RectangleShape shape;
	TextBox str;
	FinalCell(){}

	void createCell(sf::String str_, sf::Vector2f pos) {
		position = pos;
		str.chFont("files/textFont/UZSans-Regular.ttf");
		str.chFontSize(30);
		str.chFontColor(sf::Color(217, 220, 170));
		str.chText(str_);
		str.chTextPosition(position, sf::Vector2f(520, 30));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color(217, 220, 170));
		shape.setPosition(position);
		shape.setSize(sf::Vector2f(1200, 93));
	}

	void update(sf::RenderWindow& window, float time, bool *isQuest) {
		if (state != 3) {
			state = 0;
			shape.setOutlineColor(sf::Color(217, 220, 170));
			shape.setOutlineThickness(2);
			if (!isChoose && !isDelete && sf::IntRect(position.x, position.y, 1200, 93).contains(sf::Mouse::getPosition(window))) { state = 1; }
			
			if (state && sf::Mouse::isButtonPressed(sf::Mouse::Right) && !isChoose && !isDelete) {
				isDelete = 1;
			}

			if (state && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isChoose && !isDelete) {
				if (!*isQuest) {
					*isQuest = 1;
					isChoose = 1;
					state = 3;
				}
			}

			if (state != 0) {
				shape.setOutlineThickness(4);
				if (state == 2 || state == 3) shape.setOutlineColor(sf::Color(220, 110, 110));
			}
		
		}
	}

	void draw(sf::RenderWindow &window) {
		window.draw(shape);
		if (!isDelete && !isChoose) str.draw(window);
	}
};


class FinalField {
public:
	std::wifstream fin;
	std::wstring gameName;
	std::vector<sf::String> theme;
	std::vector<QuestAns> questAll;
	int number_of_question = 0, numer = 0;

	MyImage* img = NULL;
	MyMusic* msc = NULL;
	TextBox quest_str;
	QuestAns quest;
	Button further_btn;
	
	std::vector<FinalCell> cell;

	const int width = 1200, height = 650;
	int state = 0;
	bool isQuest = 0;
	bool showImage = 0, playMusic = 0;
	long long startStopwatch = 0, stopStopwatch = 0;
	long long timeQuestion = 6000, timeImage = 3000;
	sf::RectangleShape main_rect;
	sf::Vector2f position = sf::Vector2f(440, 55);

	FinalField() {
		outFile(FILE_NAME_F);
		main_rect.setSize(sf::Vector2f(width, height));
		main_rect.setFillColor(sf::Color::Transparent);
		main_rect.setOutlineThickness(3);
		main_rect.setOutlineColor(sf::Color(217, 220, 170));
		main_rect.setPosition(position);

		quest_str.chFont("files/textFont/UZSans-Regular.ttf");

		further_btn.chFont("files/textFont/UZSans-Regular.ttf");
		further_btn.chFontSize(50);
		further_btn.chText(L"Дальше");
		further_btn.chAllPosition(sf::Vector2f(1700, 900), sf::Vector2f(200, 40));

		further_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		further_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		further_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		cell = setCell(position);
	}
	~FinalField() {
		if (img) delete img;
		if (msc) {
			msc->pause();
			delete msc;
		}
	}

	int update(sf::RenderWindow& window, float time, long long stopwatch) {
		if (further_btn.update(window, time) == 2) {
			std::wcout << L"Дальше" << "\n";
			if (state == 2) {
				if (quest.isImageQ) delete img;
				if (quest.isMusicQ) {
					msc->pause();
					delete msc;
				}
				state++;
			}
			if (state == 4) {
				state = 0;
				if (quest.isImageA) delete img;
			}
			return state;
		}
		if (state == 0) {
			for (int i = 0; i < number_of_question; i++) {
				cell[i].update(window, time, &isQuest);
			}
			if (isQuest && !(startStopwatch - stopStopwatch)) {
				startStopwatch = stopwatch;
				state = 1;
			}
			return state;
		}
		if (state == 1) {
			if (stopwatch - startStopwatch > 800) {
				state = 2;
				for (int i = 0; i < number_of_question; i++) {
					if (cell[i].state == 3) {
						numer = i;
						startStopwatch = stopwatch;
						cell[numer].state = 0;
					}
				}
			}
		}
		if (state == 2) {
			state = showQuestion(stopwatch - startStopwatch);
		}
		if (state == 3) {
			state = 4;
			startStopwatch = stopwatch;
		}
		if (state == 4) {
			state = showAnswer(stopwatch - startStopwatch);
			stopStopwatch = startStopwatch;
		}

		return state;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(main_rect); 
		if (state == 0 || state == 1) {
			for (int i = 0; i < number_of_question; i++) {
				cell[i].draw(window);
			}
		}
		if (state == 2) {
			if (!showImage) quest_str.draw(window);
			if (showImage) img->draw(window);
		}
		if (state == 4) {
			if (!showImage) quest_str.draw(window);
			if (showImage) img->draw(window);
		}
		further_btn.draw(window);
	}

private:
	int showQuestion(long long stopwatch) {
		if (stopwatch == 0) {
			quest = questAll[numer];
			int d = height;
			quest.textQ = insertNL(quest.textQ, 40, width, &d);
			quest_str.setText(sf::String(quest.textQ), 40, position, sf::Vector2f(60, d), sf::Color::White);
			showImage = playMusic = 0;
			timeImage = 4000;
			if (quest.isImageQ) img = new MyImage(quest.linkImgQ);
			if (quest.isMusicQ) {
				msc = new MyMusic(quest.linkMscQ);
				msc->play(sf::seconds(5), sf::seconds(timeQuestion));
			}
		}
		if (stopwatch > timeQuestion - timeImage && quest.isImageQ || 
			!quest.isTextQ) { showImage = 1; }
		/*if (stopwatch > timeQuestion) {
			if (quest.isImageQ) delete img;
			if (quest.isMusicQ) {
				msc->pause();
				delete msc;
			}
			return 3;
		}*/
		return 2;
	}

	int showAnswer(long long stopwatch) {
		if (stopwatch == 0) {
			int d = height;
			std::wcout << quest.textA << "\n";
			quest.textA = insertNL(quest.textA, 40, width, &d);
			quest_str.setText(sf::String(quest.textA), 40, position, sf::Vector2f(60, d), sf::Color::White);
			//std::cout << quest.textA.length() << std::endl;
			showImage = 0;
			timeImage = 2000;
			if (quest.isImageA) img = new MyImage(quest.linkImgA);
		}
		if (stopwatch > timeQuestion - timeImage && quest.isImageA) { showImage = 1; }
		return 4;
	}

	std::vector<FinalCell> setCell(sf::Vector2f pos_) {
		std::vector<FinalCell>  vscell(number_of_question);
		for (int i = 0; i < number_of_question; i++) {
			vscell[i].createCell(theme[i], pos_ + sf::Vector2f(0, i * 93));
		}
		return vscell;
	}

	int getIntFromFile() {
		std::wstring iof;
		std::getline(fin, iof);
		return std::stoi(iof);
	}

	bool outFile(std::string filename) {
		fin.imbue(std::locale("rus_rus.1251"));
		std::wcout.imbue(std::locale("rus_rus.1251"));

		fin.open(filename);
		if (!fin.is_open()) {
			std::cout << "File: \"" << filename << "\" hasn't been opened" << std::endl;
			return 0;
		}
		else {
			std::getline(fin, gameName);
			std::wcout << gameName << std::endl;
			number_of_question = getIntFromFile();
			std::cout << number_of_question << std::endl;
			std::vector<sf::String> theme_(number_of_question);
			std::vector<QuestAns> quest_(number_of_question);
			for (int i = 0; i < number_of_question; i++) {
				std::wstring th_;
				std::getline(fin, th_);
				theme_[i] = th_;
				std::wcout << th_ << std::endl;
				quest_[i].isTextQ = getIntFromFile();
				std::wcout << quest_[i].isTextQ << "\t";
				if (quest_[i].isTextQ) {
					std::getline(fin, quest_[i].textQ);
					std::wcout << quest_[i].textQ << "\t";
				}

				quest_[i].isImageQ = getIntFromFile();
				std::wcout << quest_[i].isImageQ << "\t";
				if (quest_[i].isImageQ) {
					std::getline(fin, quest_[i].linkImgQ);
					std::wcout << quest_[i].linkImgQ << "\t";
				}

				quest_[i].isMusicQ = getIntFromFile();
				std::wcout << quest_[i].isMusicQ << "\t";
				if (quest_[i].isMusicQ) {
					std::getline(fin, quest_[i].linkMscQ);
					std::wcout << quest_[i].linkMscQ << "\t";
				}

				quest_[i].isTextA = getIntFromFile();
				std::wcout << quest_[i].isTextA << "\t";
				if (quest_[i].isTextA) {
					std::getline(fin, quest_[i].textA);
					std::wcout << quest_[i].textA << "\t";
				}

				quest_[i].isImageA = getIntFromFile();
				std::wcout << quest_[i].isImageA << "\t";
				if (quest_[i].isImageA) {
					std::getline(fin, quest_[i].linkImgA);
					std::wcout << quest_[i].linkImgA << "\t";
				}
				std::wcout << std::endl;
			}
			questAll = quest_;
			theme = theme_;
		}
		fin.close();

		return 1;
	}
};


#endif // !FINALFIELD_H
