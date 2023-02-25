#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <list>

using namespace sf;


/*
vector <bool> Visited;
vector <int> Path;
 void hamilton(int curr)
{
	Path.append(curr);
	if (Path.size() == n)
		if (A[Path[0]][Path.back()] == 1)
			return true;
		else
		{
			Path.pop_back();
			return false;
		}
	Visited[curr] = true;
	for (int nxt = 0; nxt < n; ++nxt)
		if (A[curr][nxt] == 1 && !Visited[nxt])
			if (hamilton(next))
				return true;
	Visited[curr] = false;
	Path.pop_back();
	return false;
}
*/

class Apple {
public:
	RectangleShape apple;
	bool life = 0;
	int x_num =0, y_num =0;
	Apple(){
		apple.setFillColor(Color::Red);
		apple.setSize(Vector2f(32,32));
	}

	void update(){
		apple.setPosition(x_num * 32, y_num * 32);
	}

	void draw(RenderWindow& window) {
		window.draw(apple);
	}
};

class Snake {
public:
	RectangleShape snake;
	bool life = 1;
	int number_at_list,x_num, y_num, xLast, yLast, side_of_move =1, speed;
	Snake(int X_Num, int Y_Num, int Number_At_List, int Speed){
		x_num = X_Num; y_num = Y_Num; number_at_list = Number_At_List; speed = Speed;
		snake.setFillColor(Color::White);
		snake.setSize(Vector2f(32, 32));
		snake.setPosition(x_num*32,y_num*32);
	}

	void update(int xNew, int yNew, int side_of_move_new) {
		if (life) {
			xLast = x_num; yLast = y_num;
			x_num = xNew; y_num = yNew; side_of_move = side_of_move_new;
			snake.setPosition(x_num * 32, y_num * 32);
		}
	}

	void draw(RenderWindow& window) {
		window.draw(snake);
	}
};

int main() {
	srand(time(NULL));
	setlocale(0, "ru");
	const int scrW = 800, scrH = 800;
	int map[25][25], Alltime =0, length_of_snake;

	auto startChrono = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			map[i][j] = 0;
		}
	}

	RenderWindow window(VideoMode(scrW, scrH), "MegaSnake", Style::Default);

	Apple apple;
	
	std::list<Snake*> snake;
	std::list<Snake*>::iterator it; 
	std::list<Snake*>::iterator it2;

	snake.push_back(new Snake(12, 12, 0, 1)); map[12][12] = 2;
	snake.push_back(new Snake(12, 13, 1, 1)); map[12][13] = 2;
	length_of_snake = 2;

	Clock clock;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) { window.close(); }
			if (event.type == Event::Resized) { window.setSize(Vector2u(scrW, scrH)); }
		}

		float timeW = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		timeW /= 1500;

		if (Keyboard::isKeyPressed(Keyboard::R)) {
			for (it = snake.begin(); it != snake.end();) {
				Snake* p = *it;
				it = snake.erase(it); delete p;
			}
			for (int i = 0; i < 25; i++) {
				for (int j = 0; j < 25; j++) {
					map[i][j] = 0;
				}
			}
			snake.push_back(new Snake(12, 12, 0, 1)); map[12][12] = 2;
			snake.push_back(new Snake(12, 13, 1, 1)); map[12][13] = 2;
			length_of_snake = 2;
			apple.life = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::W)) { it = snake.begin(); if ((*it)->side_of_move != 3) (*it)->side_of_move = 1; }
		else if (Keyboard::isKeyPressed(Keyboard::D)) { it = snake.begin(); if ((*it)->side_of_move != 4) (*it)->side_of_move = 2; }
		else if (Keyboard::isKeyPressed(Keyboard::S)) { it = snake.begin(); if ((*it)->side_of_move != 1) (*it)->side_of_move = 3; }
		else if (Keyboard::isKeyPressed(Keyboard::A)) { it = snake.begin(); if ((*it)->side_of_move != 2) (*it)->side_of_move = 4; }

		if (Alltime != std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startChrono).count()/100) {
			Alltime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startChrono).count()/100;
			for (it = snake.begin(); it != snake.end(); it++) {
				if ((*it)->life) {
					if ((*it)->number_at_list != 0) {
						it2 = it;
						it2--;
						if ((*it2)->life) {
							(*it)->update((*it2)->xLast, (*it2)->yLast, (*it2)->side_of_move);
							it2++;
							if ((*it2)->number_at_list+1 == length_of_snake) {
								map[(*it)->xLast][(*it)->yLast] = 0;
							}
						}
					}
					else {
						if ((*it)->side_of_move == 1) { (*it)->update((*it)->x_num, (*it)->y_num - 1, 1); }
						if ((*it)->side_of_move == 2) { (*it)->update((*it)->x_num + 1, (*it)->y_num, 2); }
						if ((*it)->side_of_move == 3) { (*it)->update((*it)->x_num, (*it)->y_num + 1, 3); }
						if ((*it)->side_of_move == 4) { (*it)->update((*it)->x_num - 1, (*it)->y_num, 4); }
						if (map[(*it)->x_num][(*it)->y_num] == 2 || (*it)->x_num > 24 || (*it)->x_num < 0 || (*it)->y_num > 24 || (*it)->y_num < 0) { (*it)->life = 0; (*it)->update((*it)->xLast, (*it)->yLast, (*it)->speed); }
						else {  
							if (map[(*it)->x_num][(*it)->y_num] == 1) {
								apple.life = 0;
								int X_Num , Y_Num, Number_At_List, Speed;
								for (it2 = snake.begin(); it2 != snake.end(); it2++) {
									if ((*it2)->number_at_list + 1 == length_of_snake) {
										X_Num = (*it2)->xLast; Y_Num = (*it2)->yLast; Number_At_List = (*it2)->number_at_list; Speed = (*it2)->speed;
									}
								}
								snake.push_back(new Snake(X_Num, Y_Num, Number_At_List + 1, Speed)); length_of_snake++;
							}
							map[(*it)->x_num][(*it)->y_num] = 2;
						}
					}
				}
			}
		}


		if (!apple.life) {
			while (map[apple.x_num][apple.y_num] != 0) {
				apple.x_num = rand() % 25; apple.y_num = rand() % 25;
				apple.update();
				apple.life = 1;
			}
			map[apple.x_num][apple.y_num] = 1;
		}

		window.clear(Color(100,100,100));
		
		//system("cls");
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				if (map[i][j] == 1) { apple.draw(window); }
			}
		}

		for (it = snake.begin(); it != snake.end(); it++) {
			(*it)->draw(window);
		}

		window.display();
	}

	std::cout << "Я люблю Олю Фугу" << std::endl;
	return 0;
}