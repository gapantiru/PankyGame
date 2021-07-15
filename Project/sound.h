#ifndef SOUND_H
#define SOUND_H

#include<SDL.h>
#include<SDL_mixer.h>


class Sound {
public:

	Sound() :
	success(true),
		number(0)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}

	}

	void playSound(int n) {
		switch (n) {
		case 1:
			Mix_PlayChannel(-1, gHigh, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, gMedium, 0);
			break;
		case 3:
			Mix_PlayChannel(-1, gLow, 0);
			break;
		case 4:
			Mix_PlayChannel(-1, gScratch, 0);
			break;
		}
	}

	void loadMusic() {

		gMusic = Mix_LoadMUS("sound/beat.wav");
		if (gMusic == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}
		gScratch = Mix_LoadWAV("sound/scratch.wav");
		if (gScratch == NULL)
		{
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}

		gHigh = Mix_LoadWAV("sound/high.wav");
		if (gHigh == NULL)
		{
			printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}

		gMedium = Mix_LoadWAV("sound/medium.wav");
		if (gMedium == NULL)
		{
			printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}

		gLow = Mix_LoadWAV("sound/low.wav");
		if (gLow == NULL)
		{
			printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}
		gunShot = Mix_LoadWAV("sound/gunShot.wav");
		if (gLow == NULL)
		{
			printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
			this->success = false;
		}
	}

	void gSuccess(bool succes) {
		this->success = succes;
	}

	void sNumber(int n) {
		this->number = n;
	}

	const inline int getNumber() const { return this->number; }

private:
	Mix_Music *gMusic;

	Mix_Chunk *gunShot;

	Mix_Chunk *gScratch;
	Mix_Chunk *gHigh;
	Mix_Chunk *gMedium;
	Mix_Chunk *gLow;

	bool success;

	int number;
};

#endif
