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

class Figure {
public:
	RectangleShape block[4];
	int rotate = 0, typeF;
	int X[4], Y[4], mapX[4], mapY[4];
	long long lastTimeD = 0, lastTimeR = 0, lastTimeL = 0, lastTimeT = 0, lastTimeFullD = 0;
	bool life = 1, moving = 0, moveLR = 0; //0
	Color colorF;
	Figure() {
		mapX[0] = 7; mapY[0] = 0;
		//X[0] = 400 - 13 * 16 + mapX[0] * 32; Y[0] = 50 + mapY[0]*32;
		X[0] = 676; Y[0] = 200;
		colorF = Color(rand() % 145 + 55, rand() % 145 + 55, rand() % 145 + 55);
		block[0].setFillColor(colorF);
		block[1].setFillColor(colorF);
		block[2].setFillColor(colorF);
		block[3].setFillColor(colorF);
		block[0].setSize(Vector2f(32.f, 32.f));
		block[1].setSize(Vector2f(32.f, 32.f));
		block[2].setSize(Vector2f(32.f, 32.f));
		block[3].setSize(Vector2f(32.f, 32.f));
		block[0].setPosition(Vector2f(X[0], Y[0]));
		typeF = rand() % 7;
		createFigure();
	}
	
	void createFigure() {
		switch (typeF) {
		case 0:
			create_O();
			break;
		case 1:
			create_J();
			break;
		case 2:
			create_L();
			break;
		case 3:
			create_T(1);
			break;
		case 4:
			create_S();
			break;
		case 5:
			create_Z();
			break;
		case 6:
			create_I();
			break;
		default:
			create_O();
			break;
		}
	}

	void create_Z() {
		X[1] = X[0]; X[2] = X[0] + 32; X[3] = X[0] - 32;
		Y[1] = Y[2] = Y[0] + 32; Y[3] = Y[0];
		mapX[1] = mapX[0]; mapX[2] = mapX[0] + 1; mapX[3] = mapX[0] - 1;
		mapY[1] = mapY[2] = mapY[0] + 1; mapY[3] = mapY[0];
		updatePosition();
	}
	void create_S() {
		X[1] = X[0]; X[2] = X[0] - 32; X[3] = X[0] + 32;
		Y[1] = Y[2] = Y[0] + 32; Y[3] = Y[0];
		mapX[1] = mapX[0]; mapX[2] = mapX[0] - 1; mapX[3] = mapX[0] + 1;
		mapY[1] = mapY[2] = mapY[0] + 1; mapY[3] = mapY[0];
		updatePosition();
	}
	void create_T(bool isUpdate) {
		X[1] = X[0]; X[2] = X[0] - 32; X[3] = X[0] + 32;
		Y[2] = Y[3] = Y[0]; Y[1] = Y[0] + 32;
		mapX[1] = mapX[0]; mapX[2] = mapX[0] - 1; mapX[3] = mapX[0] + 1;
		mapY[2] = mapY[3] = mapY[0]; mapY[1] = mapY[0] + 1;
		if (isUpdate) { updatePosition(); }
	}
	void create_J() {
		X[1] = X[2] = X[0];
		X[3] = X[0] - 32;
		Y[1] = Y[0] - 32;
		Y[2] = Y[3] = Y[0] + 32;
		mapX[1] = mapX[2] = mapX[0];
		mapX[3] = mapX[0] - 1;
		mapY[1] = mapY[0] - 1;
		mapY[2] = mapY[3] = mapY[0] + 1;
		updatePosition();
	}
	void create_L() {
		X[1] = X[2] = X[0];
		X[3] = X[0] + 32;
		Y[1] = Y[0] - 32;
		Y[2] = Y[3] = Y[0] + 32;
		mapX[1] = mapX[2] = mapX[0];
		mapX[3] = mapX[0] + 1;
		mapY[1] = mapY[0] - 1;
		mapY[2] = mapY[3] = mapY[0] + 1;
		updatePosition();
	}
	void create_I() {
		X[1] = X[2] = X[3] = X[0];
		Y[1] = Y[0] - 32;
		Y[2] = Y[0] + 32;
		Y[3] = Y[0] + 64;
		mapX[1] = mapX[2] = mapX[3] = mapX[0];
		mapY[1] = mapY[0] - 1;
		mapY[2] = mapY[0] + 1;
		mapY[3] = mapY[0] + 2;
		updatePosition();
	}
	void create_O() {
		X[1] = X[0]; Y[1] = Y[0] + 32;
		X[2] = X[0] + 32; Y[2] = Y[0];
		X[3] = X[0] + 32; Y[3] = Y[0] + 32;
		mapX[1] = mapX[0]; mapY[1] = mapY[0] + 1;
		mapX[2] = mapX[0] + 1; mapY[2] = mapY[0];
		mapX[3] = mapX[0] + 1; mapY[3] = mapY[0] + 1;
		updatePosition();
	}

	void update(long long timeStopwatch, RenderWindow& window, int map[13][21]) {
		if (moving == 1) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				if (timeStopwatch - lastTimeR >= 150) { moveRight(map); lastTimeR = timeStopwatch; }
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				if (timeStopwatch - lastTimeL >= 150) { moveLeft(map); lastTimeL = timeStopwatch; }
			}
			if (Keyboard::isKeyPressed(Keyboard::C)) {
				if (timeStopwatch - lastTimeT >= 100) { 
					if (typeF == 1 || typeF == 2 || typeF == 3) {
						if (checkAround(map)) {
							if (rotate < 3) { rotate++; }
							else { rotate = 0; }
						}
					}
					else {
						if (rotate < 3) { rotate++; }
						else { rotate = 0; }
					}
						updateRotate(map); lastTimeT = timeStopwatch;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Z)) {
				if (timeStopwatch - lastTimeT >= 100) {
					if (typeF == 1 || typeF == 2 || typeF == 3) {
						if (checkAround(map)) {
							if (rotate > 0) { rotate--; }
							else { rotate = 3; }
						}
					}
					else {
						if (rotate > 0) { rotate--; }
						else { rotate = 3; }
					}
					updateRotate(map); lastTimeT = timeStopwatch;
				}
			}
			if (timeStopwatch - lastTimeD >= 350) { moveDown(); lastTimeD = timeStopwatch; }
			if (Keyboard::isKeyPressed(Keyboard::Space) && (timeStopwatch - lastTimeFullD >= 150) && (timeStopwatch - lastTimeD >= 30)) {
				moveFullDown(map); lastTimeFullD = lastTimeD = timeStopwatch;
			}
			else { checkCollision(map); }
		}
		else {

		}
	}

	bool checkAround(int map[13][21]) {
		int kolBl1Ar = 0;
		for (int i = mapX[0] - 1; i <= mapX[0] + 1; i++) {
			for (int j = mapY[0] - 1; j <= mapY[0] + 1; j++) {
				if (map[i][j] == 1) { kolBl1Ar++; }
			}
		}
		if (kolBl1Ar == 0) { return 1; }
		else { return 0; }
	}

	void updateRotate(int map[13][21]) {
		switch (typeF){
		case 1:
			switch (rotate) {
			case 0:
				create_J();
				break;
			case 1:
				X[1] = X[0] + 32; X[2] = X[3] = X[0] - 32;
				Y[1] = Y[2] = Y[0]; Y[3] = Y[0] - 32;
				mapX[1] = mapX[0] + 1; mapX[2] = mapX[3] = mapX[0] - 1;
				mapY[1] = mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				break;
			case 2:
				X[1] = X[2] = X[0]; X[3] = X[0] + 32;
				Y[1] = Y[0] + 32; Y[2] = Y[3] = Y[0] - 32;
				mapX[1] = mapX[2] = mapX[0]; mapX[3] = mapX[0] + 1;
				mapY[1] = mapY[0] + 1; mapY[2] = mapY[3] = mapY[0] - 1;
				break;
			case 3:
				X[1] = X[0] - 32; X[2] = X[3] = X[0] + 32;
				Y[1] = Y[2] = Y[0]; Y[3] = Y[0] + 32;
				mapX[1] = mapX[0] - 1; mapX[2] = mapX[3] = mapX[0] + 1;
				mapY[1] = mapY[2] = mapY[0]; mapY[3] = mapY[0] + 1;
				break;
			}
			break;
		case 2:
			switch (rotate) {
			case 0:
				create_L();
				break;
			case 1:
				X[1] = X[0] + 32; X[2] = X[3] = X[0] - 32;
				Y[1] = Y[2] = Y[0]; Y[3] = Y[0] + 32;
				mapX[1] = mapX[0] + 1; mapX[2] = mapX[3] = mapX[0] - 1;
				mapY[1] = mapY[2] = mapY[0]; mapY[3] = mapY[0] + 1;
				break;
			case 2:
				X[1] = X[2] = X[0]; X[3] = X[0] - 32;
				Y[1] = Y[0] + 32; Y[2] = Y[3] = Y[0] - 32;
				mapX[1] = mapX[2] = mapX[0]; mapX[3] = mapX[0] - 1;
				mapY[1] = mapY[0] + 1; mapY[2] = mapY[3] = mapY[0] - 1;
				break;
			case 3:
				X[1] = X[0] - 32; X[2] = X[3] = X[0] + 32;
				Y[1] = Y[2] = Y[0]; Y[3] = Y[0] - 32;
				mapX[1] = mapX[0] - 1; mapX[2] = mapX[3] = mapX[0] + 1;
				mapY[1] = mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				break;
			}
			break;
		case 3:
			switch (rotate) {
			case 0:
				create_T(1);
				break;
			case 1:
				create_T(0);
				Y[3] = Y[0] - 32; X[3] = X[0];
				mapY[3] = mapY[0] - 1; mapX[3] = mapX[0];
				break;
			case 2:
				create_T(0);
				Y[1] = Y[0] - 32; mapY[1] = mapY[0] - 1;
				break;
			case 3:
				create_T(0);
				Y[2] = Y[0] - 32; X[2] = X[0];
				mapY[2] = mapY[0] - 1; mapX[2] = mapX[0];
				break;
			}
			break;
		case 4:
			switch (rotate) {
			case 0:
				create_S();
				break;
			case 1:
				Y[2] = Y[0]; Y[3] = Y[0] - 32;
				X[3] = X[2];
				mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				mapX[3] = mapX[2];
				break;
			case 2:
				create_S();
				break;
			case 3:
				Y[2] = Y[0]; Y[3] = Y[0] - 32;
				X[3] = X[2];
				mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				mapX[3] = mapX[2];
				break;
			}
			break;
		case 5:
			switch (rotate){
			case 0:
				create_Z();
				break;
			case 1:
				Y[2] = Y[0]; Y[3] = Y[0] - 32;
				X[3] = X[0] + 32;
				mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				mapX[3] = mapX[0] + 1;
				break;
			case 2:
				create_Z();
				break;
			case 3:
				Y[2] = Y[0]; Y[3] = Y[0] - 32;
				X[3] = X[0] + 32;
				mapY[2] = mapY[0]; mapY[3] = mapY[0] - 1;
				mapX[3] = mapX[0] + 1;
				break;
			}
			break;
		case 6:
			if (map[mapX[0] + 1][mapY[0]] == 1 or map[mapX[0] - 1][mapY[0]] == 1 or map[mapX[0] - 2][mapY[0]] == 1 or map[mapX[0]][mapY[0] + 1] == 1 or map[mapX[0]][mapY[0] + 2] == 1) {
				if (rotate < 3) { rotate++; }	else { rotate = 0; }
			}
			switch (rotate){
			case 0:
				create_I();
				break;
			case 1:
				Y[1] = Y[2] = Y[3] = Y[0];
				X[1] = X[0] + 32; X[2] = X[0] - 32;	X[3] = X[0] - 64;
				mapX[1] = mapX[0] + 1;	mapX[2] = mapX[0] - 1;	mapX[3] = mapX[0] - 2;
				mapY[1] = mapY[2] = mapY[3] = mapY[0];
				break;
			case 2:
				create_I();
				break;
			case 3:
				Y[1] = Y[2] = Y[3] = Y[0];
				X[1] = X[0] + 32; X[2] = X[0] - 32;	X[3] = X[0] - 64;
				mapX[1] = mapX[0] + 1;	mapX[2] = mapX[0] - 1;	mapX[3] = mapX[0] - 2;
				mapY[1] = mapY[2] = mapY[3] = mapY[0];
				break;
			}
			break;
		default:
			create_O();
			break;
		}
	}

	void checkCollision(int map[13][21]){
		if (max(max(mapX[0], mapX[1]), max(mapX[2], mapX[3])) >= 13) { moveLeft(map); }
		if (min(min(mapX[0], mapX[1]), min(mapX[2], mapX[3])) < 0 ) { moveRight(map); }
		if ((max(max(mapY[0], mapY[1]), max(mapY[2], mapY[3])) >= 21) or map[mapX[0]][mapY[0]] == 1 or map[mapX[1]][mapY[1]] == 1 or map[mapX[2]][mapY[2]] == 1 or map[mapX[3]][mapY[3]] == 1) {
			moveUp(); Dying(map);
		}
		else {  }
	}

	void moveLeft(int map[13][21]) {
		if (map[mapX[0] - 1][mapY[0]] != 1 and map[mapX[1] - 1][mapY[1]] != 1 and map[mapX[2] - 1][mapY[2]] != 1 and map[mapX[3] - 1][mapY[3]] != 1) {
			X[0] -= 32; X[1] -= 32; X[2] -= 32; X[3] -= 32;
			mapX[0] -= 1; mapX[1] -= 1;	mapX[2] -= 1; mapX[3] -= 1;
			updatePosition();
		}
	}
	void moveRight(int map[13][21]) {
		if (map[mapX[0] + 1][mapY[0]] != 1 and map[mapX[1] + 1][mapY[1]] != 1 and map[mapX[2] + 1][mapY[2]] != 1 and map[mapX[3] + 1][mapY[3]] != 1) {
			X[0] += 32; X[1] += 32; X[2] += 32; X[3] += 32;
			mapX[0] += 1; mapX[1] += 1;	mapX[2] += 1; mapX[3] += 1;
			updatePosition();
		}
	}
	void moveDown() {
		Y[0] += 32; Y[1] += 32; Y[2] += 32; Y[3] += 32;
		mapY[0] += 1; mapY[1] += 1;	mapY[2] += 1; mapY[3] += 1;
		updatePosition();
	}

	void moveFullDown(int map[13][21]) {
		while (life) {
			moveDown(); checkCollision(map);
		}
	}

	void moveUp() {
		Y[0] -= 32; Y[1] -= 32; Y[2] -= 32; Y[3] -= 32;
		mapY[1] -= 1; mapY[0] -= 1;	mapY[2] -= 1; mapY[3] -= 1;
		updatePosition();
	}
	
	void updatePosition() {
		switch (typeF) {
		default:
			block[0].setPosition(Vector2f(X[0], Y[0]));
			block[1].setPosition(Vector2f(X[1], Y[1]));
			block[2].setPosition(Vector2f(X[2], Y[2]));
			block[3].setPosition(Vector2f(X[3], Y[3]));
			break;
		}
	}

	void Dying(int map[13][21]) {
		life = 0; moving = 0;
		for (int i = 0; i < 4; i++) { map[mapX[i]][mapY[i]] = 1; }
	}

	void drawF(RenderWindow& window) {
		for (int i = 0; i < 4; i++) {
			window.draw(block[i]);
		}
	}
};

int update_Map(int map[13][21], RectangleShape mapBlock[13][21]) {
	int kolFull_In_Line = 0, kolFullLine = 0;
	for (int j = 0; j < 21; j++) {
		for (int i = 0; i < 13; i++) {
			if (map[i][j] == 1) { kolFull_In_Line++; }
			else { break; }
		}
		if (kolFull_In_Line == 13) {
			kolFullLine++;
			for (int ii = 0; ii < 13; ii++) { map[ii][j] = 0; }//доделать опускание блоков карты
			for (int jj = j - 1; jj >= 0; jj--) {
				for (int ii = 0; ii < 13; ii++) {
					if (map[ii][jj] != 0) { 
						map[ii][jj] = 0; map[ii][jj + 1] = 1; 
						mapBlock[ii][jj + 1].setFillColor(mapBlock[ii][jj].getFillColor());
					}
				}
			}
		}
		kolFull_In_Line = 0;
	}
	if (kolFullLine == 4) { return 16 * 4 * 130; }
	else {
		if (kolFullLine > 0 && kolFullLine < 4) { return 4 * 130;  cout << 011 << endl;}
		else { return 0; cout << 000 << endl; }
	}
}

void clearMap(int map[13][21], RectangleShape mapBlock[13][21]) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 21; j++) {
			map[i][j] = 0;
			mapBlock[i][j].setFillColor(Color::Black);
		}
	}
}


int main(){
	srand(time(NULL));
	const int scrS = 800;
	auto startStopwatch = chrono::high_resolution_clock::now();
	long long AllTimeSecInt;
	int map[13][21];
	int Score = 0, numGuid = 0;
	bool isGameOver = 0, isRestart = 0;

	string nameFileFont = "files/fonts/PixelFont 1.ttf",
		scoreName_str = "Score:", scoreString_str = toString(Score),
		nextFigure_str = "Next Figure:",
		gameOver_str = "GAME OVER!",
		restartButt_str = "RESTART";

	Font font;
	font.loadFromFile(nameFileFont);
	if (!font.loadFromFile(nameFileFont)) { cout << "Error to load file: Font: " << toString(nameFileFont) << endl; }

	RenderWindow window(VideoMode(scrS, scrS), "Tetris", Style::Default);
	View view(FloatRect(0, 0, scrS, scrS));

	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();

	Text scoreName_txt, scoreString_txt, nextFigure_txt, gameOver_txt, restartButt_txt;

	scoreName_txt.setFont(font);
	scoreName_txt.setCharacterSize(50);
	scoreName_txt.setColor(Color::White);
	scoreName_txt.setPosition(Vector2f(55, 530));
	scoreName_txt.setString(scoreName_str);

	scoreString_txt.setFont(font);
	scoreString_txt.setCharacterSize(35);
	scoreString_txt.setColor(Color::White);
	scoreString_txt.setOrigin(Vector2f(scoreString_str.length() * 13 / 2, 0));
	scoreString_txt.setPosition(Vector2f(98, 600));
	scoreString_txt.setString(scoreString_str);

	nextFigure_txt.setFont(font);
	nextFigure_txt.setCharacterSize(50);
	nextFigure_txt.setColor(Color::White);
	nextFigure_txt.setPosition(Vector2f(630 , 74 ));
	nextFigure_txt.setString(nextFigure_str);

	gameOver_txt.setFont(font);
	gameOver_txt.setCharacterSize(100);
	gameOver_txt.setColor(Color::White);
	gameOver_txt.setPosition(Vector2f(235, 290));
	gameOver_txt.setString(gameOver_str);

	restartButt_txt.setFont(font);
	restartButt_txt.setCharacterSize(35);
	restartButt_txt.setColor(Color::White);
	restartButt_txt.setPosition(Vector2f(30, 20));
	restartButt_txt.setString(restartButt_str);

	RectangleShape mapBorder[2], nextFigurueBox[2], scoreBox[2], gameOverBox[2], restartButtBox[2];
	mapBorder[0].setFillColor(Color(255, 255, 255));
	mapBorder[0].setPosition(Vector2f(400-13*16-3, 47));
	mapBorder[0].setSize(Vector2f(13*32 + 6, 21*32 + 6));

	mapBorder[1].setFillColor(Color(90, 90, 90));
	mapBorder[1].setPosition(Vector2f(400 - 13 * 16 - 1, 49));
	mapBorder[1].setSize(Vector2f(13 * 32 + 2, 21 * 32 + 2));

	nextFigurueBox[0].setFillColor(Color(255, 255, 255));
	nextFigurueBox[0].setPosition(Vector2f(640 - 3, 144-3));
	nextFigurueBox[0].setSize(Vector2f(4 * 34 + 6, 6 * 30 + 6));

	nextFigurueBox[1].setFillColor(Color(90, 90, 90));
	nextFigurueBox[1].setPosition(Vector2f(640 - 1, 144-1));
	nextFigurueBox[1].setSize(Vector2f(4 * 34 + 2, 6 * 30 + 2));

	scoreBox[0].setFillColor(Color(255, 255, 255));
	scoreBox[0].setPosition(Vector2f(20 - 3, 600-3));
	scoreBox[0].setSize(Vector2f(5 * 30 + 6, 2 * 30 + 6));

	scoreBox[1].setFillColor(Color(90, 90, 90));
	scoreBox[1].setPosition(Vector2f(20 - 1, 600 - 1));
	scoreBox[1].setSize(Vector2f(5 * 30 + 2, 2 * 30 + 2));

	gameOverBox[0].setFillColor(Color(255, 255, 255));
	gameOverBox[0].setPosition(Vector2f(235 -24, 347 -12));
	gameOverBox[0].setSize(Vector2f(337 + 48, 43 + 24));

	gameOverBox[1].setFillColor(Color(90, 90, 90));
	gameOverBox[1].setPosition(Vector2f(235 - 22, 347 - 10));
	gameOverBox[1].setSize(Vector2f(337 + 44, 43 +20));

	restartButtBox[0].setFillColor(Color(255, 255, 255));
	restartButtBox[0].setPosition(Vector2f(30-6, 40-4));
	restartButtBox[0].setSize(Vector2f(89 + 12, 15 + 8));

	restartButtBox[1].setFillColor(Color(90, 90, 90));
	restartButtBox[1].setPosition(Vector2f(30 - 4, 40 - 2));
	restartButtBox[1].setSize(Vector2f(89 + 8, 15 + 4));

	RectangleShape mapBlock[13][21];
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 21; j++) {
			mapBlock[i][j].setSize(Vector2f(32.f, 32.f));
			mapBlock[i][j].setFillColor(Color::Black);
			mapBlock[i][j].setPosition(400 - 13 * 16 + i * 32, 50 + j*32);
		}
	}

	Time SEC_03 = seconds(0.3), SEC_02 = seconds(0.2), SEC_01 = seconds(0.1), SEC_1 = seconds(1);

	clearMap(map, mapBlock);

	list<Figure*> iBlock;
	list<Figure*>::iterator iIt;
	list<Figure*>::iterator iIt2;

	list<RectangleShape*> mBlock;
	list<RectangleShape*>::iterator mIt2;

	iBlock.push_back(new Figure);

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

		if (IntRect(30 - 6, 40 - 4, 89 + 12, 15 + 8).contains(Mouse::getPosition(window))) { numGuid = 1; }

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (numGuid == 1){
				restartButtBox[0].setPosition(Vector2f(30 - 4, 40 - 2));
				restartButtBox[0].setSize(Vector2f(89 + 8, 15 + 4));
				restartButtBox[1].setPosition(Vector2f(30 - 2, 40));
				restartButtBox[1].setSize(Vector2f(89 + 4, 15));
				isRestart = 1;
				isGameOver = 0;
			}
		}
		else {
			isRestart = 0;
			numGuid = 0;
			restartButtBox[0].setPosition(Vector2f(30 - 6, 40 - 4));
			restartButtBox[0].setSize(Vector2f(89 + 12, 15 + 8));
			restartButtBox[1].setPosition(Vector2f(30 - 4, 40 - 2));
			restartButtBox[1].setSize(Vector2f(89 + 8, 15 + 4));
		}

		if (isRestart) {
			clearMap(map,mapBlock);
			Score = 0;
			for (iIt = iBlock.begin(); iIt != iBlock.end();) { iIt = iBlock.erase(iIt); }
			iBlock.push_back(new Figure);
			startStopwatch = chrono::high_resolution_clock::now();
		}
		else {
			AllTimeSecInt = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startStopwatch).count();
		}

		window.clear(Color(90, 90, 90));
		
		for (int i = 0; i < 2; i++) {
			window.draw(mapBorder[i]);
			window.draw(nextFigurueBox[i]);
			window.draw(scoreBox[i]);
		}

		if (!isRestart) {
			if (!isGameOver) {
				for (iIt = iBlock.begin(); iIt != iBlock.end();) {
					(*iIt)->update(AllTimeSecInt, window, map);
					if (!(*iIt)->life) {
						mapBlock[(*iIt)->mapX[0]][(*iIt)->mapY[0]].setFillColor((*iIt)->colorF);
						mapBlock[(*iIt)->mapX[1]][(*iIt)->mapY[1]].setFillColor((*iIt)->colorF);
						mapBlock[(*iIt)->mapX[2]][(*iIt)->mapY[2]].setFillColor((*iIt)->colorF);
						mapBlock[(*iIt)->mapX[3]][(*iIt)->mapY[3]].setFillColor((*iIt)->colorF);
						iIt = iBlock.erase(iIt);
						Score += 4;
					}
					else {
						(*iIt)->drawF(window);
						iIt++;
					}
				}

				int kolFigureOnMap = 0;
				for (iIt = iBlock.begin(); iIt != iBlock.end();) { kolFigureOnMap++; iIt++; }

				if (kolFigureOnMap == 1) {
					iIt = iBlock.begin();
					(*iIt)->moving = 1;
					(*iIt)->X[0] = 400 - 13 * 16 + (*iIt)->mapX[0] * 32; (*iIt)->Y[0] = 50 + (*iIt)->mapY[0] * 32;
					(*iIt)->createFigure();

					iBlock.push_back(new Figure);
				}

				Score += update_Map(map, mapBlock);
			}
			else {
				for (iIt = iBlock.begin(); iIt != iBlock.end();) {
					(*iIt)->drawF(window);
					iIt++;
				}
			}
		}

		system("cls");
		cout  << "Score: "<< Score << endl;
		if (isGameOver) { cout << "GameOver" << endl; }


		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 21; j++) {
				if (j < 3 && map[i][j] != 0) { isGameOver = 1; }
				if (map[i][j] != 0) {
					window.draw(mapBlock[i][j]);
				}
			}
		}

		scoreString_str = toString(Score);
		scoreString_txt.setOrigin(Vector2f(scoreString_str.length() * 13 / 2, 0));
		scoreString_txt.setPosition(Vector2f(98, 600));
		scoreString_txt.setString(scoreString_str);

		window.draw(scoreName_txt);
		window.draw(scoreString_txt);
		window.draw(nextFigure_txt);

		window.draw(restartButtBox[0]);
		window.draw(restartButtBox[1]);
		window.draw(restartButt_txt);

		if (isGameOver) { 
			window.draw(gameOverBox[0]);
			window.draw(gameOverBox[1]);
			window.draw(gameOver_txt); 
		}

		window.display();
	}

	return 0;
}