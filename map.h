#pragma once
#include "common.h"
#define HF 0x80000000
#define VF 0x40000000
#define AD 0x20000000
#define ROT 0x10000000
typedef struct map {
	SDL_Texture* tilesets[15];
	SDL_Rect *borders;
	SDL_Rect *objects;
	long long int*** map;
	int firstgids[15];
	int widthText[15];
	int *idObjs;
	int totalBord;
	int totalObjs;
}MAP;

void renderMap(SDL_Renderer* renderer, MAP* map, int numLayers, int width, int height, int tileWidth, int tileHeight, int start, SDL_Rect camera);
void renderTile(SDL_Renderer* renderer, SDL_Texture* tileset, int tileID, int firstgid, int tileWidth, int tileHeight, int posX, int posY, int widthTSX, int flag, float rotate);
void parseMap(const char* filename, MAP* map, int numLayers, int width, int height);
void initTileSets(MAP* map, SDL_Renderer* rd);
int isWalkable(MAP* map, SDL_Rect playerPoz);
void renderObject(SDL_Renderer* renderer, MAP* map, SDL_Rect camera);
