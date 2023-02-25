#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;
using namespace sf;

template <typename T>
string toString(T val) {
	ostringstream oss;
	oss << val;
	return oss.str();
}

template <typename T>
T fromString(const string& s) {
	istringstream iss(s);
	T res;
	iss >> res;
	return res;
}

class Map {
public:
	RectangleShape block[10][10], scoreRS, nameRS, comboRS;
	bool map[10][10] = { 0 }, isCombo = 0;
	chrono::steady_clock::time_point start = chrono::high_resolution_clock::now(), stop = start;
	int Score = 0;
	Font font;
	Text scoreTXT, nameTXT, comboTXT;
	Map() {
		font.loadFromFile("files/fonts/AnimeAce.ttf");
		if (!font.loadFromFile("files/fonts/AnimeAce.ttf")) { cout << "Error to load font (Map)" << endl; }
		scoreTXT.setFont(font);
		scoreTXT.setCharacterSize(26);
		scoreTXT.setColor(Color(225, 190, 120));
		scoreTXT.setPosition(Vector2f(388,130));
		scoreTXT.setString(toString(Score));	//toString(Score)

		scoreRS.setFillColor(Color(115,60,25));
		scoreRS.setPosition(Vector2f(240,125));
		scoreRS.setSize(Vector2f(320, 40));

		nameTXT.setFont(font);
		nameTXT.setCharacterSize(18);
		nameTXT.setColor(Color(225, 190, 120));
		nameTXT.setPosition(Vector2f(50,30));
		nameTXT.setString("Ставь фигуры и набери наибольшее количество очков");

		nameRS.setFillColor(Color(115, 60, 25));
		nameRS.setPosition(Vector2f(42, 25));
		nameRS.setSize(Vector2f(710, 40));

		comboTXT.setFont(font);
		comboTXT.setCharacterSize(36);
		comboTXT.setColor(Color(115, 60, 25));
		comboTXT.setPosition(Vector2f(290,340));
		comboTXT.setString("Комбо!");

		comboRS.setFillColor(Color(225, 190, 120));
		comboRS.setPosition(Vector2f(240, 320));
		comboRS.setSize(Vector2f(320, 80));

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				block[i][j].setFillColor(Color(228, 180, 90));
				block[i][j].setSize(Vector2f(40, 40));
				block[i][j].setPosition(Vector2f(195 + j * 41, 200 + i * 41));
			}
		}
	}

	void clear() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				map[i][j] = 0;
			}
		}
		Score = 0;
		isCombo = 0;
	}

	void update(int *abilityTurn) {
		vector<int> stR_ToDel;
		vector<int> stL_ToDel;
		for (int i = 0; i < 10; i++) {
			int s1 = 0, s2 = 0;
			for (int j = 0; j < 10; j++) {
				if (map[i][j]) s1++;
				if (map[j][i]) s2++;
			}
			if (s1 == 10) stR_ToDel.push_back(i);
			if (s2 == 10) stL_ToDel.push_back(i);
		}

		Score += (stR_ToDel.size() + stL_ToDel.size()) * (stR_ToDel.size() + stL_ToDel.size()) * 10;
		if (stR_ToDel.size() + stL_ToDel.size() >= 3) { 
			isCombo = 1; 
			comboTXT.setString(toString(stR_ToDel.size() + stL_ToDel.size()) + " Комбо!"); 
			start = chrono::high_resolution_clock::now();
			(*abilityTurn)++;
		}
		else { if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() > 1500) isCombo = 0; }

		for (int i = 0; i < stR_ToDel.size(); i++) {
			for (int j = 0; j < 10; j++) {
				map[stR_ToDel[i]][j] = 0;
			}
		}
		for (int i = 0; i < stL_ToDel.size(); i++) {
			for (int j = 0; j < 10; j++) {
				map[j][stL_ToDel[i]] = 0;
			}
		}

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (map[i][j]) {
					block[i][j].setFillColor(Color(228, 180, 90));
				}
				else {
					block[i][j].setFillColor(Color(70, 25, 10));
				}
			}
		}
		scoreTXT.setPosition(Vector2f(400 - toString(Score).length()*12,130));
		scoreTXT.setString(toString(Score));
	}

	void draw(RenderWindow& window) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				window.draw(block[i][j]);
			}
		}
		if (isCombo) {
			window.draw(comboRS);
			window.draw(comboTXT);
		}
		window.draw(nameRS);
		window.draw(scoreRS);
		window.draw(scoreTXT);
		window.draw(nameTXT);
	}
};

class Figure {
public:
	int type = 1, place = 1;
	bool figureB[3][3] = {0};
	bool grab = 0, isPress = 0;
	int numTurn = 0;
	RectangleShape figure[3][3], turnRS;
	Font font;
	Text turnTXT;
	float X = 177 + 163*place, Y = 650;
	Vector2f last_cr;
	Figure() {}
	Figure(int placeIn){
		place = placeIn;
		X = 177 + 163 * place;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				figure[i][j].setFillColor(Color(228, 180, 90));
				figure[i][j].setSize(Vector2f(40, 40));
				figure[i][j].setOrigin(20, 20);
				figure[i][j].setPosition(Vector2f(X + j * 41, Y + i * 41));
			}
		}
		turnRS.setFillColor(Color(115, 60, 25));
		turnRS.setSize(Vector2f(122, 40));
		turnRS.setPosition(Vector2f(X-20, 755));

		font.loadFromFile("files/fonts/AnimeAce.ttf");
		if (!font.loadFromFile("files/fonts/AnimeAce.ttf")) { cout << "Error to load font (Map)" << endl; }
		turnTXT.setFont(font);
		turnTXT.setCharacterSize(16);
		turnTXT.setColor(Color(225, 190, 120));
		turnTXT.setPosition(Vector2f(X-15, 765));
		turnTXT.setString("повернуть");

		ReCreate();
	}

	void Create(int placeIn = 1) {
		place = placeIn;
		X = 177 + 163 * place;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				figure[i][j].setFillColor(Color(228, 180, 90));
				figure[i][j].setSize(Vector2f(40, 40));
				figure[i][j].setOrigin(20, 20);
				figure[i][j].setPosition(Vector2f(X + j * 41, Y + i * 41));
			}
		}
		turnRS.setFillColor(Color(115, 60, 25));
		turnRS.setSize(Vector2f(122, 40));
		turnRS.setPosition(Vector2f(X - 20, 755));

		font.loadFromFile("files/fonts/AnimeAce.ttf");
		if (!font.loadFromFile("files/fonts/AnimeAce.ttf")) { cout << "Error to load font (Map)" << endl; }
		turnTXT.setFont(font);
		turnTXT.setCharacterSize(16);
		turnTXT.setColor(Color(225, 190, 120));
		turnTXT.setPosition(Vector2f(X - 15, 765));
		turnTXT.setString("повернуть");

		ReCreate();
	};

	void ReCreate() {
		X = 177 + 163 * place; Y = 650;
		numTurn = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				figureB[i][j] = 0;
			}
		}
		type = rand() % 7 + 1;

		switch (type){
		case 1:
			figureB[1][1] = 1;
			break;
		case 2:
			figureB[0][1] = figureB[0][2] = figureB[1][1] = figureB[1][2] = 1;
			break;
		case 3:
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					figureB[i][j] = 1;
				}
			}
			break;
		case 4:
			figureB[0][1] = figureB[1][1] = 1;
			break;
		case 5:
			figureB[0][1] = figureB[1][1] = figureB[2][1] = 1;
			break;
		case 6:
			figureB[0][1] = figureB[1][1] = figureB[1][2] = 1;
			break;
		case 7:
			figureB[0][0] = figureB[1][0] = figureB[2][0] = figureB[2][1] = figureB[2][2] = 1;
			break;
		}

		if (type > 3) {
			for (int i = 0; i < rand() % 4; i++) rotate();
		}
	}

	void rotate() {
		bool vsp[3][3] = {0};
		vsp[0][2] = figureB[0][0];
		vsp[1][2] = figureB[0][1];
		vsp[2][2] = figureB[0][2];
		vsp[0][1] = figureB[1][0];
		vsp[1][1] = figureB[1][1];
		vsp[2][1] = figureB[1][2];
		vsp[0][0] = figureB[2][0];
		vsp[1][0] = figureB[2][1];
		vsp[2][0] = figureB[2][2];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				figureB[i][j] = vsp[i][j];
			}
		}
	}

	void update(RenderWindow& window, bool map[10][10], bool *isTap, int *Score, int* abilityTurn, bool* isLose) {
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos_cr = window.mapPixelToCoords(pixelPos);
		if (!(*isLose) && *abilityTurn > 0 && !isPress && !(*isTap) && pos_cr.x > X - 20 && pos_cr.x < X + 102 && pos_cr.y > 755 && pos_cr.y < 795 && Mouse::isButtonPressed(Mouse::Left)) {
			*isTap = 1;
			isPress = 1;
			numTurn++;
			turnRS.setFillColor(Color(225, 190, 120));
			turnTXT.setColor(Color(115, 60, 25));
			rotate();
		}
		if (isPress && !Mouse::isButtonPressed(Mouse::Left)) {
			isPress = 0;
			*isTap = 0;
			turnTXT.setColor(Color(225, 190, 120));
			turnRS.setFillColor(Color(115, 60, 25));
		}

		if (!(*isLose) && !(*isTap) && !grab && pos_cr.x > X && pos_cr.x < X + 122 && pos_cr.y > Y && pos_cr.y < Y + 122 && Mouse::isButtonPressed(Mouse::Left)) {
			grab = 1;
			last_cr = pos_cr;
			*isTap = 1;
		}

		if (grab && Mouse::isButtonPressed(Mouse::Left)) {
			X += pos_cr.x - last_cr.x;
			Y += pos_cr.y - last_cr.y;
			last_cr = pos_cr;
		}

		if (grab && !Mouse::isButtonPressed(Mouse::Left) && (numTurn%4 !=0 && *abilityTurn>0 || !(numTurn%4))) {
			grab = 0;
			int xi = floor(float(X)/41. - 195./41.), yi = floor(float(Y)/41. - 200./41.);
			bool GoOn = 1;
			vector<Vector2i> numer;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (figureB[i][j]) {
						if (xi + j >= 0 && xi + j <= 9 && yi + i >= 0 && yi + i <= 9) {
							if (!(map[yi + i][xi + j])) {
								numer.push_back(Vector2i(j, i));
							}
							else {
								GoOn = 0;
							}
						}
						else {
							GoOn = 0;
						}
					}
				}
			}

			if (GoOn) {
				for (int i = 0; i < numer.size(); i++) {
					map[yi + numer[i].y][xi + numer[i].x] = 1;
				}
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (figureB[i][j]) (*Score)++;
					}
				}
				if (numTurn % 4 != 0) {
					(*abilityTurn)--;
				}
				ReCreate();
			}
			else {
				X = 177 + 163 * place; Y = 650;
			}
			*isTap = 0;
		}
		else {
			if (grab && !Mouse::isButtonPressed(Mouse::Left) && numTurn % 4 != 0 && *abilityTurn < 1){
				grab = 0;
				do {
					numTurn--;
					rotate();
				} while (numTurn);
				X = 177 + 163 * place; Y = 650;
			}
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				figure[i][j].setPosition(Vector2f(X + j * 41, Y + i * 41));
			}
		}
		if (!abilityPut(map, *abilityTurn)) *isLose = 1;
	}

	bool abilityPut(bool map[10][10], int abilityTurn) {
		int res = 0;
		for (int r = 0; r < 4 && abilityTurn > 0 || r < 1; r++) {
			int index = 0;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					int iof = 0;
					for (int fi = 0; fi < 3; fi++) {
						for (int fj = 0; fj < 3; fj++) {
							if (figureB[fi][fj] && map[i + fi][j + fj]) {
								iof++;
								break;
							}
						}
						if (iof) break;
					}
					index += iof;
				}
			}
			if (index == 64) res++;
			rotate();
		}
		return res < 4;
	}


	void draw(RenderWindow& window) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (figureB[i][j])	window.draw(figure[i][j]);
			}
		}
		window.draw(turnRS);
		window.draw(turnTXT);
	}
};

int main() {
	srand(time(NULL));
	const int scrS = 800, scrW = 800;
	auto startStopwatch = chrono::high_resolution_clock::now();
	long long AllTimeSecInt;
	bool isTap = 0, isPress = 0, isLose = 0;
	int abilityTurn = 0;

	string nameFileFont = "files/fonts/AnimeAce.ttf";

	Font font;
	font.loadFromFile(nameFileFont);
	if (!font.loadFromFile(nameFileFont)) { cout << "Error to load file: Font: " << toString(nameFileFont) << endl; }

	RenderWindow window(VideoMode(scrS, scrS), "Block Clear", Style::Default);
	View view(FloatRect(0, 0, scrW, scrS));

	RectangleShape perRS, atRS, loseRS; 
	Text perTXT, atTXT, loseTXT;
	perRS.setFillColor(Color(115, 60, 25));
	perRS.setSize(Vector2f(122, 40));
	perRS.setPosition(Vector2f(20, 755));

	perTXT.setFont(font);
	perTXT.setCharacterSize(13);
	perTXT.setColor(Color(225, 190, 120));
	perTXT.setPosition(Vector2f(30, 765));
	perTXT.setString("перезапуск");

	atRS.setFillColor(Color(115, 60, 25));
	atRS.setSize(Vector2f(122, 40));
	atRS.setPosition(Vector2f(700, 755));

	atTXT.setFont(font);
	atTXT.setCharacterSize(13);
	atTXT.setColor(Color(225, 190, 120));
	atTXT.setPosition(Vector2f(745, 765));
	atTXT.setString(toString(abilityTurn));

	loseTXT.setFont(font);
	loseTXT.setCharacterSize(36);
	loseTXT.setColor(Color(115, 60, 25));
	loseTXT.setPosition(Vector2f(290, 340));
	loseTXT.setString("РАЗГРОМ");

	loseRS.setFillColor(Color(225, 190, 120));
	loseRS.setPosition(Vector2f(240, 320));
	loseRS.setSize(Vector2f(320, 80));

	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();

	Time SEC_03 = seconds(0.3), SEC_02 = seconds(0.2), SEC_01 = seconds(0.1), SEC_1 = seconds(1);

	Map map;
	Figure f[3];
	for (int i = 0; i < 3; i++) {
		f[i].Create(i);
	}

	Clock clock;
	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) { window.close(); }
			if (event.type == Event::Resized) { window.setSize(Vector2u(scrS, scrS)); }
		}

		float timeW = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		timeW /= 1000;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos_cr = window.mapPixelToCoords(pixelPos);

		if (!isTap && !isPress && pos_cr.x > 20 && pos_cr.x < 142 && pos_cr.y > 755 && pos_cr.y < 795 && Mouse::isButtonPressed(Mouse::Left)) {
			isTap = 1;
			isPress = 1;
			isLose = 0; //!isLose
			perRS.setFillColor(Color(225, 190, 120));
			perTXT.setColor(Color(115, 60, 25));
			map.clear();
			for (int i = 0; i < 3; i++) {
				f[i].ReCreate();
			}
		}

		if (isPress && !Mouse::isButtonPressed(Mouse::Left)) {
			isPress = 0;
			isTap = 0;
			perRS.setFillColor(Color(115, 60, 25));
			perTXT.setColor(Color(225, 190, 120));
		}
		
		map.update(&abilityTurn);
		for (int i = 0; i < 3; i++) {
			f[i].update(window, map.map, &isTap, &map.Score, &abilityTurn, &isLose);
		}

		atTXT.setString(toString(abilityTurn));

		window.clear(Color(160, 100, 40));

		map.draw(window);
		for (int i = 0; i < 3; i++) {
			f[i].draw(window);
		}

		window.draw(perRS);
		window.draw(perTXT);

		window.draw(atRS);
		window.draw(atTXT);

		if (isLose) {
			window.draw(loseRS);
			window.draw(loseTXT);
		}

		window.display();
	}

	return 0;
}
/*
проигрыш
*/