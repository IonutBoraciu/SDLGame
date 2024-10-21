#pragma once
#include "common.h"
typedef struct enemy {
	SDL_Texture* character;
	SDL_Rect sourceSize;
	SDL_Rect destSize;
	SDL_Rect spawnPoint;
	int width, height;
	int rangeX, rangeY;
	DIR dir;
	aTime time;
} ENEMY;

void addEnemy(char location[100], SDL_Rect spawnPoint, ENEMY* enemies, SDL_Renderer* rd, int ratioW, int ratioH, int* size, int rangeX, int rangeY);
void attack(ENEMY* enemies, int n, PLAYER mainC, SDL_Rect camera);