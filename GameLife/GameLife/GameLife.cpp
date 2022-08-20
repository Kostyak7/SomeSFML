#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;
using namespace sf;

#define SIZE 50

class Cell {
public:
	RectangleShape setDefault() {
		RectangleShape rect;
		rect.setFillColor(Color::White);
		rect.setSize(Vector2f(20, 20));
		rect.setPosition(Vector2f(0,0));
		rect.setOutlineThickness(2);
		rect.setOutlineColor(Color::Black);
		return rect;
	}

public:
	RectangleShape shape = setDefault();
	Vector2f position = Vector2f(0,0);
	bool isLive = 0;
	Cell() {}

	void changeLive() {
		if (!isLive) shape.setFillColor(Color::White);
		else shape.setFillColor(Color::Black);
	}

	void update_cell_pos() {
		shape.setPosition(position);
	}

	void draw(RenderWindow &window) {
		window.draw(shape);
	}
};

class Square : public Cell {
public:
	Cell cells[SIZE][SIZE];
	bool nextGen[SIZE][SIZE];
	Square() :Cell() {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				nextGen[i][j] = 0;
				cells[i][j].position = Vector2f(i * 20, j * 20);
				cells[i][j].update_cell_pos();
			}
		}
	}

	void update() {
		for (int i = 1; i < SIZE - 1; i++) {
			for (int j = 1; j < SIZE - 1; j++) {
				int numLive = cells[i + 1][j + 1].isLive + cells[i][j + 1].isLive+ cells[i + 1][j].isLive + cells[i - 1][j + 1].isLive + 
					cells[i + 1][j - 1].isLive+ cells[i - 1][j - 1].isLive+ cells[i - 1][j].isLive + cells[i][j - 1].isLive;
				if (numLive == 3 || numLive == 2 && cells[i][j].isLive) nextGen[i][j] = 1;
				else nextGen[i][j] = 0;
			}
		}
		for (int i = 1; i < SIZE - 1; i++) {
			for (int j = 1; j < SIZE - 1; j++) {
				cells[i][j].isLive = nextGen[i][j];
				cells[i][j].changeLive();
			}
		}
	}

	virtual void draw(RenderWindow& window) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				cells[i][j].draw(window);
			}
		}
	}
};


int main() {
	srand(time(NULL));
	const int scrS = 800, scrW = 800;
	auto startStopwatch = chrono::high_resolution_clock::now();
	long long AllTimeSecInt;

	RenderWindow window(VideoMode(scrS, scrS), "Life Game", Style::Default);
	View view(FloatRect(0, 0, scrW, scrS));

	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();

	Time SEC_03 = seconds(0.3), SEC_02 = seconds(0.2), SEC_01 = seconds(0.1), SEC_1 = seconds(1);

	Square square;

	//start
	square.cells[2][2].isLive = 1;
	square.cells[2][2].changeLive();

	square.cells[3][3].isLive = 1;
	square.cells[3][3].changeLive();

	square.cells[3][4].isLive = 1;
	square.cells[3][4].changeLive();

	square.cells[2][4].isLive = 1;
	square.cells[2][4].changeLive();

	square.cells[1][4].isLive = 1;
	square.cells[1][4].changeLive();

	bool isPress = 0;
	bool isPause = 1, isPressSpace = 0;
	bool isPlant = 1, isPressE = 0;
	bool isEChange = 0;
	Vector2f oldPos(0,0);

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

		if (Keyboard::isKeyPressed(Keyboard::E)) {
			if (!isPressE) {
				isPlant = !isPlant;
				isPause = isPressE = 1;
			}
		}
		else {
			isPressE = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) && !isPlant) {
			if (!isPressSpace) {
				isPause = !isPause;
				isPressSpace = 1;
			}
		}
		else {
			isPressSpace = 0;
		}

		if (Mouse::isButtonPressed(Mouse::Right)) {
			if (isPress) {
				Vector2f newPos(Mouse::getPosition(window));
				view.setCenter(view.getCenter() - newPos + oldPos);
				window.setView(view);
				oldPos = newPos;
			}
			else {
				isPress = 1;
				oldPos = Vector2f(Mouse::getPosition(window));
			}
		}
		else {
			isPress = 0;
		}

		if (!isPause) {
			AllTimeSecInt =  chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startStopwatch).count();
			if (AllTimeSecInt > 400) {
				startStopwatch = chrono::high_resolution_clock::now();
				square.update();
			}
		}
		else {
			system("cls");
			Vector2i mouse_pos = Mouse::getPosition(window) + Vector2i(view.getCenter() - Vector2f(scrS/2, scrW/2));
			cout << mouse_pos.x << "\t" << mouse_pos.y << endl;
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (!isEChange) {
					isEChange = 1;
					int i_x = mouse_pos.x / 20, j_y = mouse_pos.y / 20;
					if (i_x >= 0 && i_x <= SIZE && j_y >= 0 && j_y <= SIZE) {
						square.cells[i_x][j_y].isLive = !square.cells[i_x][j_y].isLive;
						square.cells[i_x][j_y].changeLive();
					}
				}
			}
			else {
				isEChange = 0;
			}
		}

		window.clear(Color(20, 20, 20));

		square.draw(window);

		window.display();
	}

	return 0;
}