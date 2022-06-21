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

#endif // !MYMUSIC_H