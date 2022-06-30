#pragma once
#ifndef MYMUSIC_H
#define MYMUSIC_H

#define PATH_MSC L"files/music/" 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>


class MyMusic {
public:
	int type = 1;
	bool isPl = 0;
	sf::Music* msc;
	sf::Sound* snd;
	sf::SoundBuffer* snd_bfr;
	sf::Time* tm_pl, *strt;
	long long timer = 0;
	std::chrono::steady_clock::time_point startTimer;
	MyMusic(std::wstring file_name, int TYPE = 1) {
		type = TYPE;
		tm_pl = new sf::Time;
		strt = new sf::Time(sf::seconds(0));
		if (type == 1) {
			msc = new sf::Music;
			msc->openFromFile(wstring_to_string(PATH_MSC + file_name));
			*tm_pl = msc->getDuration();
		}
		else {
			snd = new sf::Sound;
			snd_bfr = new sf::SoundBuffer;
			snd_bfr->loadFromFile(wstring_to_string(PATH_MSC + file_name));
			snd->setBuffer(*snd_bfr);
			*tm_pl = snd_bfr->getDuration();
		}
	}
	~MyMusic() {
		if (type == 1) delete msc;
		else {
			delete snd;
			delete snd_bfr;
		}
		delete tm_pl;
		delete strt;
	}

	void pause() {
		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTimer).count();
		isPl = 0;
		if (type == 1) msc->pause();
		else snd->pause();
	}

	void setStartTime(sf::Time start) {
		*strt = start + sf::milliseconds(timer);
		if (start < *tm_pl) {
			if (type == 1) msc->setPlayingOffset(start);
			else snd->setPlayingOffset(start);
		}
	}

	void setTimePlay(sf::Time start, sf::Time duration) {
		setStartTime(start);
		duration -= sf::milliseconds(timer);
		if (*tm_pl - start > duration) {
			*tm_pl = duration;
		}
	}

	void update() {
		if (isPl) {
			timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTimer).count();
			if (timer >= tm_pl->asMilliseconds()) pause();
		}
	}

	void play(sf::Time start, sf::Time duration) {
		startTimer = std::chrono::high_resolution_clock::now();
		isPl = 1;
		if (type == 1) msc->play();
		else snd->play();
		setTimePlay(start, duration);
	}
};


typedef struct QuestAns {
	int isTextQ;
	std::wstring textQ;
	int isImageQ;
	std::wstring linkImgQ;
	int isMusicQ;
	std::wstring linkMscQ;
	int isTextA;
	std::wstring textA;
	int isImageA;
	std::wstring linkImgA;
} QuestAns;


class MyImage {
public:
	sf::Image* img;
	sf::Texture* txtr;
	sf::Sprite* sprt;
	MyImage(std::wstring file_name) {
		img = new sf::Image;
		txtr = new sf::Texture;
		sprt = new sf::Sprite;
		img->loadFromFile(wstring_to_string(PATH_IMG + file_name));
		txtr->loadFromImage(*img);
		sprt->setTexture(*txtr);
		sprt->setPosition(sf::Vector2f(443, 58));
		float prW = img->getSize().x, prH = img->getSize().y;
		if (650 * prW >= 1200 * prH) {
			sprt->move(0, (650 - 1200 / prW * prH) / 2);
			sprt->setScale(1200 / prW, 1200 / prW);
		}
		else {
			sprt->move((1200 - 650 / prH * prW) / 2, 0);
			sprt->setScale(650 / prH, 650 / prH);
		}
	}

	~MyImage() {
		std::cout << "FIOF" << std::endl;
		delete img;
		delete txtr;
		delete sprt;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(*sprt);
	}
};


#endif // !MYMUSIC_H