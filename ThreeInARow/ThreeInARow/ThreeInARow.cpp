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
#include <unordered_map>

using namespace std;
using namespace sf;


class Object {
public:
	Sprite sprite;
	Texture texture;
	Object(){}
	Object(const string& filename, Vector2f position = Vector2f(0,0)) {
		setParams(filename, position);
	}

	void setParams(const string& filename, Vector2f position = Vector2f(0, 0)) {
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
		sprite.setPosition(position);
	}

	void draw(RenderWindow& window) {
		window.draw(sprite);
	}
};


vector<vector<Object>> field_background(int w = 12, int h = 10) {
	vector<vector<Object>> map = vector<vector<Object>>(w, vector<Object>(h));

	float start_x = 240, start_y = 100;
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			if (((i % 2 == 0) && (j % 2 == 0)) || ((i % 2 != 0) && (j % 2 != 0))) {
				map[i][j].setParams("files/img/map_backGr_01.png", Vector2f(start_x + 60 * i, start_y + 60 * j));
			}
			else {
				map[i][j].setParams("files/img/map_backGr_02.png", Vector2f(start_x + 60 * i, start_y + 60 * j));
			}
		}
	}

	return map;
}

void draw_object_2vector(RenderWindow& window, vector<vector<Object>> _vector, int w = 12, int h = 10) {
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			_vector[i][j].draw(window);
		}
	}
}


class Item {
private:
	const string charname = "files/img/character_0";
	float dx = 0, dy = 0;
public:
	Object item;
	unsigned int type = 1;
	float x = 0, y = 0;
	int i = 0, j = 0;
	bool is_active = 0;

	Item() {}
	Item(int _i, int _j) {
		setParams(_i, _j);
	}
private:
	bool checkDown(vector<vector<int>> map) {
		return j < 9 && map[i][j + 1] == 0;
	}

public:
	void setParams(int _i, int _j) {
		is_active = 1;
		type = rand() % 6 + 1;
		i = _i;
		j = _j;
		x = float(i * 60 + 240);
		y = float(j * 60 + 100);
		item.setParams(charname + to_string(type) + ".png", Vector2f(x, y));
	}

	void update_position(int _i, int _j) {
		i = _i; j = _j;
		x = i * 60 + 240;
		y = j * 60 + 100;
		item.sprite.setPosition(x, y);
	}

	void update(float _time, vector<vector<int>>& map_type, bool& is_fall) {
		if (checkDown(map_type) && y < (j + 1) * 60 + 100) {
			is_fall = is_active = 1;
			dy = 0.15;
		}
		else {
			if (dy) {
				map_type[i][j] = 0;
				y = (++j) * 60 + 100;
				map_type[i][j] = type;
			}
			dy = 0;
			is_fall = is_active = 0;
		}
		y += dy * _time;
		item.sprite.setPosition(x, y);
	}

	void draw(RenderWindow& window) {
		item.draw(window);
	}

	~Item() {}
};


class Field {
public:
	bool is_fall = 0, is_push_button = 0;
	vector<vector<int>> map_type = vector<vector<int>>(12, vector<int>(10, 0));
	vector<Item*> map;
	Vector2i start_btn = Vector2i(-1, -1);
	Vector2f start_crd = Vector2f(-1, -1);
	unsigned int score = 0;
	Text score_text;
	Font font;

	Field(){
		fillMap();
		string fontname = "files/fonts/PixelFont 1.ttf";
		font.loadFromFile(fontname);
		if (!font.loadFromFile(fontname)) { cout << "Error to load file: Font: " << fontname << endl; }
		score_text.setFont(font);
		score_text.setCharacterSize(50);
		score_text.setColor(Color::White);
		score_text.setPosition(30, -10);
		score_text.setString("Score: " + to_string(score));
	}

	void fillMap() {
		for (int i = 0; i < 12; ++i) {
			if (!map_type[i][0]) {
				map.push_back(new Item(i, 0));
				map_type[i][0] = map[map.size() - 1]->type;
			}
		}
	}

	int clearMap() {
		int res = 0;
		unordered_map<int, int> to_delete;
		for (int j = 0; j < 10; ++j) {
			int pre_type = 0, len = 0;
			for (int i = 0; i < 12; ++i) {
				if (pre_type == map_type[i][j] && map_type[i][j] != 0) ++len;
				else {
					if (len >= 3) {
						for (int ind = i - len; ind < i; ++ind) {
							to_delete[ind*100 + j] = 1;
						}
					}
					len = 1;
					pre_type = map_type[i][j];
				}
			}
			if (len >= 3) {
				for (int ind = 12 - len; ind < 12; ++ind) {
					to_delete[ind * 100 + j] = 1;
				}
			}
		}
		for (int i = 0; i < 12; ++i) {
			int pre_type = 0, len = 0;
			for (int j = 0; j < 10; ++j) {
				if (pre_type == map_type[i][j] && map_type[i][j] != 0) ++len;
				else {
					if (len >= 3) {
						for (int jnd = j - len; jnd < j; ++jnd) {
							to_delete[i * 100 + jnd] = 1;
						}
					}
					len = 1;
					pre_type = map_type[i][j];
				}
			}
			if (len >= 3) {
				for (int jnd = 10 - len; jnd < 10; ++jnd) {
					to_delete[i * 100 + jnd] = 1;
				}
			}
		}
		for (int i = 0; i < map.size();) {
			if (to_delete[map[i]->i * 100 + map[i]->j] == 1) {
				map_type[map[i]->i][map[i]->j] = 0;
				Item* el = map[i];
				map.erase(map.begin() + i);
				delete el;
				++res;
			}
			else ++i;
		}
		return res;
	}

private:
	int swapUp() {
		int res = 0;
		if (start_btn.y > 0) {
			Vector2i last_ind = Vector2i(-1, -1);
			int _i = 0;
			for (auto el : map) {
				if (start_btn.x == el->i) {
					if (start_btn.y - 1 == el->j) {
						last_ind.x = _i;
						el->update_position(el->i, el->j + 1);
					}
					else if (start_btn.y == el->j) {
						last_ind.y = _i;
						el->update_position(el->i, el->j - 1);
					}
					if (last_ind.x != -1 && last_ind.y != -1) break;
				}
				_i++;
			}
			int swap_type = map_type[start_btn.x][start_btn.y];
			map_type[start_btn.x][start_btn.y] = map_type[start_btn.x][start_btn.y - 1];
			map_type[start_btn.x][start_btn.y - 1] = swap_type;
			res = clearMap();
			if (!res) {
				swap_type = map_type[start_btn.x][start_btn.y];
				map_type[start_btn.x][start_btn.y] = map_type[start_btn.x][start_btn.y - 1];
				map_type[start_btn.x][start_btn.y - 1] = swap_type;
				map[last_ind.x]->update_position(map[last_ind.x]->i, map[last_ind.x]->j - 1);
				map[last_ind.y]->update_position(map[last_ind.y]->i, map[last_ind.y]->j + 1);
			}
		}
		return res;
	}

	int swapRight() {
		int res = 0;
		if (start_btn.x < 11) {
			Vector2i last_ind = Vector2i(-1, -1);
			int _i = 0;
			for (auto el : map) {
				if (start_btn.y == el->j) {
					if (start_btn.x + 1 == el->i) {
						last_ind.x = _i;
						el->update_position(el->i - 1, el->j);
					}
					else if (start_btn.x == el->i) {
						last_ind.y = _i;
						el->update_position(el->i + 1, el->j);
					}
					if (last_ind.x != -1 && last_ind.y != -1) break;
				}
				_i++;
			}
			int swap_type = map_type[start_btn.x][start_btn.y];
			map_type[start_btn.x][start_btn.y] = map_type[start_btn.x + 1][start_btn.y];
			map_type[start_btn.x + 1][start_btn.y] = swap_type;
			res = clearMap();
			if (!res) {
				swap_type = map_type[start_btn.x][start_btn.y];
				map_type[start_btn.x][start_btn.y] = map_type[start_btn.x + 1][start_btn.y];
				map_type[start_btn.x + 1][start_btn.y] = swap_type;
				map[last_ind.x]->update_position(map[last_ind.x]->i + 1, map[last_ind.x]->j);
				map[last_ind.y]->update_position(map[last_ind.y]->i - 1, map[last_ind.y]->j);
			}
		}
		return res;
	}

	int swapDown() {
		int res = 0;
		if (start_btn.y < 9) {
			Vector2i last_ind = Vector2i(-1, -1);
			int _i = 0;
			for (auto el : map) {
				if (start_btn.x == el->i) {
					if (start_btn.y == el->j) {
						last_ind.x = _i;
						el->update_position(el->i, el->j + 1);
					}
					else if (start_btn.y + 1 == el->j) {
						last_ind.y = _i;
						el->update_position(el->i, el->j - 1);
					}
					if (last_ind.x != -1 && last_ind.y != -1) break;
				}
				_i++;
			}
			int swap_type = map_type[start_btn.x][start_btn.y];
			map_type[start_btn.x][start_btn.y] = map_type[start_btn.x][start_btn.y + 1];
			map_type[start_btn.x][start_btn.y + 1] = swap_type;
			res = clearMap();
			if (!res) {
				swap_type = map_type[start_btn.x][start_btn.y];
				map_type[start_btn.x][start_btn.y] = map_type[start_btn.x][start_btn.y + 1];
				map_type[start_btn.x][start_btn.y + 1] = swap_type;
				map[last_ind.x]->update_position(map[last_ind.x]->i, map[last_ind.x]->j + 1);
				map[last_ind.y]->update_position(map[last_ind.y]->i, map[last_ind.y]->j - 1);
			}
		}
		return res;
	}

	int swapLeft() {
		int res = 0;
		if (start_btn.x > 0) {
			Vector2i last_ind = Vector2i(-1, -1);
			int _i = 0;
			for (auto el : map) {
				if (start_btn.y == el->j) {
					if (start_btn.x == el->i) {
						last_ind.x = _i;
						el->update_position(el->i - 1, el->j);
					}
					else if (start_btn.x - 1 == el->i) {
						last_ind.y = _i;
						el->update_position(el->i + 1, el->j);
					}
					if (last_ind.x != -1 && last_ind.y != -1) break;
				}
				_i++;
			}
			int swap_type = map_type[start_btn.x][start_btn.y];
			map_type[start_btn.x][start_btn.y] = map_type[start_btn.x - 1][start_btn.y];
			map_type[start_btn.x - 1][start_btn.y] = swap_type;
			res = clearMap();
			if (!res) {
				swap_type = map_type[start_btn.x][start_btn.y];
				map_type[start_btn.x][start_btn.y] = map_type[start_btn.x - 1][start_btn.y];
				map_type[start_btn.x - 1][start_btn.y] = swap_type;
				map[last_ind.x]->update_position(map[last_ind.x]->i + 1, map[last_ind.x]->j);
				map[last_ind.y]->update_position(map[last_ind.y]->i - 1, map[last_ind.y]->j);
			}
		}
		return res;
	}

	void checkMoving(RenderWindow& window, float _time) {
		Vector2f pos_cr = window.mapPixelToCoords(Mouse::getPosition(window));
		int i = (pos_cr.x - 240) / 60, j = (pos_cr.y - 100) / 60;
		bool is_on_cell = 0;
		if (IntRect(240 + i * 60, 100 + j * 60, 60, 60).contains(Mouse::getPosition(window))) { 
			is_on_cell = 1;
		}
		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (is_on_cell && !is_push_button) {
				is_push_button = 1;
				start_btn = Vector2i(i, j);
				start_crd = pos_cr;
			}
		}
		else {
			if (is_push_button) {
				int pre_score = 0;
				if (pos_cr.y < -pos_cr.x + (start_crd.x + start_crd.y) && pos_cr.y < pos_cr.x + (start_crd.y - start_crd.x)) {
					pre_score += swapUp();
					cout << "UP\n";
				} else if (pos_cr.y > -pos_cr.x + (start_crd.x + start_crd.y) && pos_cr.y > pos_cr.x + (start_crd.y - start_crd.x)) {
					pre_score += swapDown();
					cout << "Down\n";
				} else if (pos_cr.y > -pos_cr.x + (start_crd.x + start_crd.y) && pos_cr.y < pos_cr.x + (start_crd.y - start_crd.x)) {
					pre_score += swapRight();
					cout << "Right\n";
				}
				else {
					pre_score += swapLeft();
					cout << "Left\n";
				}
				score += pre_score * sqrt(pre_score * 10);
				start_btn = Vector2i(-1, -1);
				start_crd = Vector2f(-1, -1);
			}
			is_push_button = 0;
		}
	}

public:
	void update(float _time, RenderWindow& window) {
		for (auto el : map) {
			el->update(_time, map_type, is_fall);
		}
		fillMap();
		int pre_score = 0;
		if (!is_fall) pre_score += clearMap();
		if (!is_fall) checkMoving(window, _time);
		score += pre_score * sqrt(pre_score * 10);
		score_text.setString("Score: " + to_string(score));
		//cout << "Score: " << score << "\n";
	}


	void draw(RenderWindow& window) {
		for (auto el : map) {
			el->draw(window);
		}
		window.draw(score_text);
	}

	~Field(){
		for (auto el : map) {
			delete el;
		}
	}
};




int main() {
	srand(time(NULL));
	const int scrS = 1200, scrW = 800;
	auto startStopwatch = chrono::high_resolution_clock::now();
	long long AllTimeSecInt;

	RenderWindow window(VideoMode(scrS, scrW), "Three-In-A-Row", Style::Default);
	View view(FloatRect(0, 0, scrS, scrW));

	Vector2f center = window.getView().getCenter();
	Vector2f size = window.getView().getSize();


	Object background("files/img/Fon.png");
	vector<vector<Object>> map = field_background();
	Field field;


	Clock clock;
	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) { window.close(); }
			if (event.type == Event::Resized) { window.setSize(Vector2u(scrS, scrW)); }
		}

		float timeW = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		timeW /= 1000;


		///	Updates
		field.update(timeW, window);



		/// Draw
		window.clear(Color(20, 20, 20));

		background.draw(window);
		draw_object_2vector(window, map);
		field.draw(window);

		window.display();
	}

	return 0;
}