#include "Menu.h"

int main() {
	Menu menu;

	menu.menuGo();

	return 0;
}

/*#include <iostream>
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

int main() {
	srand(time(NULL));
	const int scrS = 800, scrW = 800;
	auto startStopwatch = chrono::high_resolution_clock::now();
	long long AllTimeSecInt;

	string nameFileFont = "files/fonts/PixelFont 1.ttf";

	Font font;
	font.loadFromFile(nameFileFont);
	if (!font.loadFromFile(nameFileFont)) { cout << "Error to load file: Font: " << toString(nameFileFont) << endl; }

	RenderWindow window(VideoMode(scrS, scrS), "Name of program", Style::Default);
	View view(FloatRect(0, 0, scrW, scrS));

	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();

	Time SEC_03 = seconds(0.3), SEC_02 = seconds(0.2), SEC_01 = seconds(0.1), SEC_1 = seconds(1);

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

		window.clear(Color(20, 20, 20));

		window.display();
	}

	return 0;
}*/