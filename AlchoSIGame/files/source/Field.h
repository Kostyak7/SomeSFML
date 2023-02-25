#pragma once
#ifndef FIELD_H
#define FIELD_H

#define FILE_NAME "files/pack_"
#define PATH_IMG L"files/images/rounds/" 

#include "TextWork.h"
#include "Button.h"
#include "Player.h"
#include "MyMusic.h"
#include <fstream>


class Cell {
public:
	bool isAnswer = 0;
	int type = 0, state = 0;
	sf::Vector2f position = sf::Vector2f(0, 0);
	TextBox str;
	sf::RectangleShape shape;
	Cell(){}

	void createCell(int type_, sf::String str_, sf::Vector2f pos_) {
		type = type_;
		position = pos_;
		str.chFont("files/textFont/UZSans-Regular.ttf");
		str.chFontSize(30);
		str.chFontColor(sf::Color(217, 220, 170));
		str.chText(str_);
		str.chTextPosition(position, sf::Vector2f(20, 30));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color(217, 220, 170));
		shape.setPosition(position);
		switch (type) {
		case 1:
			shape.setSize(sf::Vector2f(240, 90));
			break;
		case 2:
			shape.setSize(sf::Vector2f(160, 110));
			break;
		default:
			shape.setSize(sf::Vector2f(160, 90));
			break;
		}
	}

	void update(sf::RenderWindow & window, float time, bool* isQuest) {
		if (type == 0) {
			if (state != 3) {
				state = 0;
				shape.setOutlineColor(sf::Color(217, 220, 170));
				shape.setOutlineThickness(2);
				if (!isAnswer && sf::IntRect(position.x, position.y, 160, 90).contains(sf::Mouse::getPosition(window))) { state = 1; }
			}

			if (state && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isAnswer) {
				if (!*isQuest) {
					*isQuest = 1;
					isAnswer = 1;
					state = 3;
				}
				else {
					if (state != 3) state = 2;
				}
			}
			if (state == 1) shape.setOutlineThickness(4);
			if (state == 2 || state == 3) {
				shape.setOutlineThickness(4);
				shape.setOutlineColor(sf::Color(220, 110, 110));
			}
		}
	}


	void draw(sf::RenderWindow& window) {
		window.draw(shape);
		if (!isAnswer) str.draw(window);
	}
};


class Field {
private:
	std::wifstream fin;
	typedef struct sizeField {
		int W, H;
	} sizeField;

	std::vector<std::vector<Cell>> setCellField(int m, int n, sf::Vector2f pos_) {
		std::vector<std::vector<Cell>> cellField(m, std::vector<Cell>(n));
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				cellField[i][j].createCell(0, toString<int>(100 + 100 * j), pos_ + sf::Vector2f(160*j,i*90));
			}
		}
		return cellField;
	}

	std::vector<Cell> setCellThemeNames(int m,std::vector<sf::String> theme, sf::Vector2f pos_) {
		std::vector<Cell> cellTheme(m);
		for (int i = 0; i < m; i++) {
			cellTheme[i].createCell(1, theme[i], pos_ + sf::Vector2f(0, 90 * i));
		}
		return cellTheme;
	}

	std::vector<Cell> setCellAlcho(int n, sf::Vector2f pos_) {
		sf::String alcho[] = { L"����", L"�������", L"����", L"���", L"�����", L"�����", L"������", L"����", L"������", L"������" };
		std::vector<Cell> cellAlcho(n);
		for (int i = 0; i < n; i++) {
			cellAlcho[i].createCell(2, alcho[i], pos_ + sf::Vector2f(160 * i, 0));
		}
		return cellAlcho;
	}

	int getIntFromFile() {
		std::wstring iof;
		std::getline(fin, iof);
		return std::stoi(iof);
	}

public:
	std::wstring gameName;
	std::vector<sf::String> theme;
	std::vector<std::vector<QuestAns>> questAll;
	sizeField sf;

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
			sf.W = getIntFromFile();
			std::cout << sf.W << std::endl;
			sf.H = getIntFromFile();
			std::cout << sf.H << std::endl;
			std::vector<sf::String> theme_(sf.H); //std::vector<sf::String>(sf.H)
			std::vector<std::vector<QuestAns>> quest_(sf.W, std::vector<QuestAns>(sf.H));
			for (int i = 0; i < sf.H; i++) {
				std::wstring th_;
				std::getline(fin, th_);
				theme_[i] = th_;
				std::wcout << th_ << std::endl;
				for (int j = 0; j < sf.W; j++) {
					quest_[i][j].isTextQ = getIntFromFile();
					std::wcout << quest_[i][j].isTextQ << "\t";
					if (quest_[i][j].isTextQ) {
						std::getline(fin, quest_[i][j].textQ);
						std::wcout << quest_[i][j].textQ << "\t";
					}

					quest_[i][j].isImageQ = getIntFromFile();
					std::wcout << quest_[i][j].isImageQ << "\t";
					if (quest_[i][j].isImageQ) {
						std::getline(fin, quest_[i][j].linkImgQ);
						std::wcout << quest_[i][j].linkImgQ << "\t";
					}

					quest_[i][j].isMusicQ = getIntFromFile();
					std::wcout << quest_[i][j].isMusicQ << "\t";
					if (quest_[i][j].isMusicQ) {
						std::getline(fin, quest_[i][j].linkMscQ);
						std::wcout << quest_[i][j].linkMscQ << "\t";
					}

					quest_[i][j].isTextA = getIntFromFile();
					std::wcout << quest_[i][j].isTextA << "\t";
					if (quest_[i][j].isTextA) {
						std::getline(fin, quest_[i][j].textA);
						std::wcout << quest_[i][j].textA << "\t";
					}

					quest_[i][j].isImageA = getIntFromFile();
					std::wcout << quest_[i][j].isImageA << "\t";
					if (quest_[i][j].isImageA) {
						std::getline(fin, quest_[i][j].linkImgA);
						std::wcout << quest_[i][j].linkImgA << "\t";
					}
					std::wcout << std::endl;
				}
			}
			questAll = quest_;
			theme = theme_;
		}

		fin.close();

		return 1;
	}

public:
	MyImage* img = NULL;
	MyMusic* msc = NULL;
	TextBox quest_str;
	QuestAns quest;
	Player *players;
	Button further_btn;
	Button answer_btn[2];

	const int width = 1200, height = 650;
	int state = 0, round = 0;
	bool imgIsDel = 1, mscIsDel = 1;
	bool isOut, isQuest = 0;
	bool showImage = 0, playMusic = 0;
	bool isAnswering = 0;
	long long startStopwatch = 0, stopStopwatch = 0;
	long long deftimeQuestion = 4000, deftimeAnswer = 4000;
	long long timeQuestion = 4000, timeAnswer = 4000;
	long long timeImage = 5000, timePlayerAnswer = 5000;
	long long laststartStopWatch = 0, startAnswering = 0;
	sf::RectangleShape main_rect, timeLine_rect[2];
	sf::Vector2f position = sf::Vector2f(440, 55);
	sf::Vector2i ijQ;
	std::vector<std::vector<Cell>> cell_0;
	std::vector<Cell> cell_1;
	std::vector<Cell> cell_2;
	Field(int num_round, Player *plyrs) {
		round = num_round;
		players = plyrs;
		isOut = outFile(FILE_NAME + toString<int>(round) + ".txt");
		main_rect.setSize(sf::Vector2f(width, height));
		main_rect.setFillColor(sf::Color::Transparent);
		main_rect.setOutlineThickness(3);
		main_rect.setOutlineColor(sf::Color(217, 220, 170));
		main_rect.setPosition(position);

		timeLine_rect[0].setSize(sf::Vector2f(width, 8));
		timeLine_rect[0].setFillColor(sf::Color(217, 220, 170));
		timeLine_rect[0].setPosition(position);

		timeLine_rect[1].setSize(sf::Vector2f(8, height));
		timeLine_rect[1].setFillColor(sf::Color(217, 220, 170));
		timeLine_rect[1].setPosition(position);

		quest_str.chFont("files/textFont/UZSans-Regular.ttf");

		further_btn.chFont("files/textFont/UZSans-Regular.ttf");
		further_btn.chFontSize(50);
		further_btn.chText(L"������");
		further_btn.chAllPosition(sf::Vector2f(1700, 900), sf::Vector2f(200, 40));

		further_btn.createDefault("files/images/menu/ButtonDefault.png", "mid");
		further_btn.createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		further_btn.createPush("files/images/menu/ButtonPush.png", "mid");

		answer_btn[0].chFont("files/textFont/UZSans-Regular.ttf");
		answer_btn[0].chFontSize(50);
		answer_btn[0].chText(L"��!");
		answer_btn[0].chAllPosition(sf::Vector2f(440, 600), sf::Vector2f(200, 40));

		answer_btn[0].createDefault("files/images/menu/ButtonDefault.png", "mid");
		answer_btn[0].createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		answer_btn[0].createPush("files/images/menu/ButtonPush.png", "mid");

		answer_btn[1].chFont("files/textFont/UZSans-Regular.ttf");
		answer_btn[1].chFontSize(50);
		answer_btn[1].chText(L"���!");
		answer_btn[1].chAllPosition(sf::Vector2f(1140, 600), sf::Vector2f(200, 40));

		answer_btn[1].createDefault("files/images/menu/ButtonDefault.png", "mid");
		answer_btn[1].createOnPlace("files/images/menu/ButtonOnPlace.png", "mid");
		answer_btn[1].createPush("files/images/menu/ButtonPush.png", "mid");

		cell_0 = setCellField(sf.W, sf.H, position + sf::Vector2f(240, 110));
		cell_1 = setCellThemeNames(sf.H, theme, position + sf::Vector2f(0, 110));
		cell_2 = setCellAlcho(sf.W, position + sf::Vector2f(240, 0));
	}
	~Field() {
		std::cout << "IOF" << std::endl;
		if (!imgIsDel) delete img;
		if (!mscIsDel) {
			msc->pause();
			delete msc;
		}
	}
	
	int showQuestion(long long stopwatch) {
		if (stopwatch == 0) {
			isAnswering = 0;
			quest = questAll[ijQ.x][ijQ.y];
			int d = height;
			quest.textQ = insertNL(quest.textQ, 40, width, &d);
			quest_str.setText(sf::String(quest.textQ), 40, position, sf::Vector2f(60, d), sf::Color::White);
			showImage = playMusic = 0;
			timeImage = 3500;

			if (!quest.isMusicQ && !quest.isTextQ && quest.isImageQ) timeQuestion = 2*deftimeQuestion;
			if (!quest.isMusicQ && !quest.isTextQ && quest.isImageQ) timeImage = 5000;
			if (!quest.isMusicQ && quest.isTextQ) timeQuestion = deftimeQuestion + quest.textQ.length() * 65;
			if (quest.isImageQ) {
				imgIsDel = 0;
				img = new MyImage(quest.linkImgQ);
			}
			if (quest.isMusicQ) {
				timeQuestion = 15000;
				mscIsDel = 0;
				msc = new MyMusic(quest.linkMscQ);
				msc->play(sf::seconds(5), sf::seconds(timeQuestion));
			}
			timeLine_rect[0].setScale(1, 1);
			timeLine_rect[0].setPosition(position);
			timeLine_rect[1].setScale(1, 1);
			timeLine_rect[1].setPosition(position);
		}
		if (stopwatch > 500) {
			timeLine_rect[0].setScale(1 - float(stopwatch - 500)/(timeQuestion - 500),1);
			timeLine_rect[0].setPosition(position.x + float(stopwatch - 500) / (timeQuestion - 500) * float(width) / 2, timeLine_rect[0].getPosition().y);
			timeLine_rect[1].setScale(1, 1 - float(stopwatch - 500) / (timeQuestion - 500));
			timeLine_rect[1].setPosition(timeLine_rect[1].getPosition().x, position.y + float(stopwatch - 500) / (timeQuestion - 500) * float(height) / 2);
		}
		if (stopwatch > timeQuestion - timeImage && quest.isImageQ ||
			!quest.isTextQ) { showImage = 1; }
		if (stopwatch > timeQuestion) {
			if (quest.isImageQ && !imgIsDel) {
				delete img;
				imgIsDel = 1;
			}
			if (quest.isMusicQ && !mscIsDel) {
				msc->pause();
				delete msc;
				mscIsDel = 1;
			}
			return 3;
		}
		return 2;
	}

	void playerAnswer(sf::RenderWindow& window, float time, long long stopwatch, int *num_ans_plyer) {
		if (!isAnswering) {
			isAnswering = 1;
			laststartStopWatch = startStopwatch;
			startAnswering = stopwatch;
			if (quest.isMusicQ) msc->pause();
		}

		if (answer_btn[0].update(window, time) == 2) {
			std::wcout << L"��!" << "\n";
			state = 3;
			if (quest.isImageQ && !imgIsDel) {
				delete img;
				imgIsDel = 1;
			}
			if (quest.isMusicQ && !mscIsDel) {
				msc->pause();
				delete msc;
				mscIsDel = 1;
			}
			isAnswering = 0;
			players[*num_ans_plyer - 1].state = 4;
			players[*num_ans_plyer - 1].score += 100 + 100 * ijQ.y;
			*num_ans_plyer = 0;
		}
		if (answer_btn[1].update(window, time) == 2 || stopwatch - startAnswering > timePlayerAnswer) {
			std::wcout << L"���!" << "\n";
			isAnswering = 0;
			players[*num_ans_plyer - 1].score -= 100 + 100 * ijQ.y;
			players[*num_ans_plyer - 1].state = 4;
			*num_ans_plyer = 0;
			if (quest.isMusicQ) msc->play(*(msc->strt), *(msc->tm_pl));
		}

		startStopwatch = laststartStopWatch + stopwatch - startAnswering;
	}

	int showAnswer(long long stopwatch) {
		if (stopwatch == 0) {
			int d = height;
			std::wcout << quest.textA << "\n";
			quest.textA = insertNL(quest.textA, 40, width, &d);
			quest_str.setText(sf::String(quest.textA), 40, position, sf::Vector2f(60, d), sf::Color::White);
			showImage = 0;
			timeImage = 3000;
			timeAnswer = deftimeAnswer + quest.textA.length() * 60;
			if (quest.isImageA) {
				imgIsDel = 0;
				img = new MyImage(quest.linkImgA);
			}
		}
		if (stopwatch > timeAnswer - timeImage && quest.isImageA || !quest.isTextA) { showImage = 1; }
		if (stopwatch > timeAnswer) {
			if (quest.isImageA && !imgIsDel) { 
				imgIsDel = 1;
				delete img;
			}
			return 0;
		}
		return 4;
	}

	int update(sf::RenderWindow& window, float time, long long stopwatch, int *num_ans_plyer) {
		if (further_btn.update(window, time) == 2) {
			std::wcout << L"������" << "\n";
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
			for (int i = 0; i < sf.W; i++) {
				for (int j = 0; j < sf.H; j++) {
					cell_0[i][j].update(window, time, &isQuest);
				}
			}
			if (isQuest && !(startStopwatch - stopStopwatch)) {
				startStopwatch = stopwatch;
				state = 1;
				return state;
			}
		}
		if (state == 1) {
			if (stopwatch - startStopwatch > 800) {
				state = 2;
				for (int i = 0; i < sf.W; i++) {
					for (int j = 0; j < sf.H; j++) {
						if (cell_0[i][j].state == 3) {
							ijQ = sf::Vector2i(i, j);
							startStopwatch = stopwatch;
							cell_0[i][j].state = 0;
						}
					}
				}
			}
		}
		if (state == 2) {
			state = showQuestion(stopwatch - startStopwatch);
			if (*num_ans_plyer) {
				playerAnswer(window, time, stopwatch, num_ans_plyer);
			}
		}
		if (state == 3) {
			state = 4;
			startStopwatch = stopwatch;
		}
		if (state == 4) {
			state = showAnswer(stopwatch - startStopwatch);
			isQuest = 0;
			stopStopwatch = startStopwatch;
		}
		return  state;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(main_rect);
		if (state == 0 || state == 1) {
			for (int i = 0; i < sf.H; i++) {
				cell_1[i].draw(window);
				for (int j = 0; j < sf.W; j++) {
					cell_0[i][j].draw(window);
				}
			}
			for (int i = 0; i < sf.W; i++) {
				cell_2[i].draw(window);
			}
		}
		if (state == 2) {
			timeLine_rect[0].setPosition(timeLine_rect[0].getPosition().x, timeLine_rect[0].getPosition().y + height);
			window.draw(timeLine_rect[0]);
			timeLine_rect[0].setPosition(timeLine_rect[0].getPosition().x, timeLine_rect[0].getPosition().y - height);
			window.draw(timeLine_rect[0]);

			timeLine_rect[1].setPosition(timeLine_rect[1].getPosition().x + width, timeLine_rect[1].getPosition().y);
			window.draw(timeLine_rect[1]);
			timeLine_rect[1].setPosition(timeLine_rect[1].getPosition().x - width, timeLine_rect[1].getPosition().y);
			window.draw(timeLine_rect[1]);
			if (!showImage) quest_str.draw(window);
			if (showImage) img->draw(window);

			if (isAnswering) {
				answer_btn[0].draw(window);
				answer_btn[1].draw(window);
			}
		}
		if (state == 4) {
			if (!showImage) quest_str.draw(window);
			if (showImage) img->draw(window);
		}
		further_btn.draw(window);
	}
};

#endif // !FIELD_H
