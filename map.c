#include "map.h"
#include "stdio.h"
int wallBorders[26] = {21,22,23,37,38,39,49,50,51,65,66,67,97,98,99,100,102,118,145,146,148,149,161,162,164,165};

void renderObject(SDL_Renderer* renderer, MAP* map,SDL_Rect camera) {
	for (int i = 0; i < map->totalObjs; i++) {
		int gid = map->idObjs[i];
		SDL_Rect dest = map->objects[i];
		for (int j = 0; j < 11; j++) {
			if (gid >= map->firstgids[j] && (j == 10 || gid < map->firstgids[j + 1])) {
				int tileIndex = gid - map->firstgids[j];
				int tilesetCols = map->widthText[j] / 32;
				SDL_Rect srcRect = {
					(tileIndex % tilesetCols) * 32,
					(tileIndex / tilesetCols) * 32,
					32,
					32
				};
				SDL_Rect dest = map->objects[i];
				if (SDL_HasIntersection(&camera, &dest)) {
					dest.x -= camera.x;
					dest.y -= camera.y + 30;
					SDL_RenderCopy(renderer, map->tilesets[j], &srcRect, &dest);
				}

			}
		}
	}
}

void renderMap(SDL_Renderer* renderer, MAP* map, int numLayers, int width, int height, int tileWidth, int tileHeight, int start,SDL_Rect camera) {

	for (int layer = start; layer < numLayers; layer++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				long long int tileID = map->map[layer][y][x];
				long long int oldTileID = tileID;
				if (tileID == 0) {
					continue;
				}
				for (int i = 0; i < 11; i++) {
					tileID = oldTileID;
					int flag = 0;
					float rotate = 0;
					int checkHF = 0;
					int checkVF = 0;
					if (tileID & HF) {
						tileID &= ~(1 << 31);
						flag = flag | SDL_FLIP_HORIZONTAL;
						checkHF = 1;
					}
					if (tileID & VF) {
						tileID &= ~(1 << 30);
						flag = flag | SDL_FLIP_VERTICAL;
						checkVF = 1;
					}

					if (tileID & AD) {
						tileID &= ~(1 << 29);
						flag = flag | SDL_FLIP_HORIZONTAL;
						rotate = 90.0f;
						if (checkHF) {
							flag = 0;
						}
					}
					if (tileID & ROT) {
						tileID &= ~(1 << 28);
						rotate = 90.0f;
					}

					if (tileID >= map->firstgids[i] && (i == 11 - 1 || tileID < map->firstgids[i + 1])) {
						SDL_Rect dest = { x * tileWidth,y * tileHeight,tileWidth,tileHeight };
						if(SDL_HasIntersection(&dest,&camera))
							renderTile(renderer, map->tilesets[i], tileID, map->firstgids[i], tileWidth, tileHeight, x * tileWidth - camera.x, y * tileHeight - camera.y, map->widthText[i], flag, rotate);
						break;
					}
				}
			}
		}
	}
}
void renderTile(SDL_Renderer* renderer, SDL_Texture* tileset, int tileID, int firstgid, int tileWidth, int tileHeight, int posX, int posY, int widthTSX, int flag, float rotate) {

	int oldID = tileID;

	int tileIndex = tileID - firstgid;
	int tilesetCols = widthTSX / tileWidth;

	SDL_Rect srcRect = {
		(tileIndex % tilesetCols) * tileWidth,
		(tileIndex / tilesetCols) * tileHeight,
		tileWidth,
		tileHeight
	};
	SDL_Rect destRect = { posX, posY, tileWidth, tileHeight };
	if (flag == 0 && rotate < 2) {
		SDL_RenderCopy(renderer, tileset, &srcRect, &destRect);
	}
	else {
		SDL_RenderCopyEx(renderer, tileset, &srcRect, &destRect, rotate, NULL, (SDL_RendererFlip)flag);
	}
}

void parseMap(const char* filename, MAP* map, int numLayers, int width, int height) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Failed to open file: %s\n", filename);
		return;
	}
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* fileContent = malloc(fileSize + 1);
	fread(fileContent, 1, fileSize, file);
	fclose(file);
	fileContent[fileSize] = '\0';

	cJSON* json = cJSON_Parse(fileContent);
	if (!json) {
		printf("Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
		free(fileContent);
		return;
	}

	cJSON* layers = cJSON_GetObjectItemCaseSensitive(json, "layers");
	if (!cJSON_IsArray(layers)) {
		printf("No layers found\n");
		cJSON_Delete(json);
		free(fileContent);
		return;
	}

	for (int i = 0; i < numLayers; i++) {
		cJSON* layer = cJSON_GetArrayItem(layers, i);
		cJSON* nameLayer = cJSON_GetObjectItem(layer, "name");
		cJSON* type = cJSON_GetObjectItem(layer, "type");
		if (type && strcmp(type->valuestring, "tilelayer") == 0) {

			cJSON* data = cJSON_GetObjectItemCaseSensitive(layer, "data");
			if (!cJSON_IsArray(data)) {
				printf("Layer %d has no data\n", i);
				continue;
			}
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < width; k++) {
					int tileIndex = j * width + k;
					cJSON* tile = cJSON_GetArrayItem(data, tileIndex);
					if (tile) {

						map->map[i][j][k] = tile->valuedouble;
					}
					else {
						map->map[i][j][k] = 0;
					}
				}
			}
		}
		else {
			if (strcmp(nameLayer->valuestring, "border") == 0) {
				cJSON* objects = cJSON_GetObjectItem(layer, "objects");
				int objNr = cJSON_GetArraySize(objects);
				for (int i = 0; i < objNr; i++) {
					cJSON* object = cJSON_GetArrayItem(objects, i);
					int objX = cJSON_GetObjectItem(object, "x")->valuedouble;
					int objY = cJSON_GetObjectItem(object, "y")->valuedouble;
					cJSON* polygon = cJSON_GetObjectItem(object, "polygon");
					if (polygon && cJSON_IsArray(polygon)) {
						int polCount = cJSON_GetArraySize(polygon);
						cJSON* point = cJSON_GetArrayItem(polygon, 0);
						int minX, maxX, minY, maxY;
						minX = maxX = cJSON_GetObjectItem(point, "x")->valuedouble + objX;
						minY = maxY = cJSON_GetObjectItem(point, "y")->valuedouble + objY;

						for (int i = 1; i < polCount; i++) {
							point = cJSON_GetArrayItem(polygon, i);
							int x = cJSON_GetObjectItem(point, "x")->valuedouble + objX;
							int y = cJSON_GetObjectItem(point, "y")->valuedouble + objY;
							if (x < minX)
								minX = x;
							if (x > maxX)
								maxX = x;
							if (y < minY)
								minY = y;
							if (y > maxY)
								maxY = y;
						}
						map->borders[map->totalBord] = (SDL_Rect){minX,minY,maxX - minX,maxY - minY};
						map->totalBord++;
					}
				}
			}
			else {
				cJSON* objects = cJSON_GetObjectItem(layer, "objects");
				int nrOf = cJSON_GetArraySize(objects);
				for (int i = 0; i < nrOf && objects; i++) {
					cJSON* object = cJSON_GetArrayItem(objects, i);
					if (object) {
						int x = cJSON_GetObjectItem(object, "x")->valuedouble;
						int y = cJSON_GetObjectItem(object, "y")->valuedouble;
						int gid = cJSON_GetObjectItem(object, "gid")->valuedouble;
						//printf("%d %d %d\n", x, y, gid);
						map->idObjs[map->totalObjs] = gid;
						map->objects[map->totalObjs] = (SDL_Rect){x,y,32,32};
						map->totalObjs++;
					}
				}
			}
		}
	}

	// Clean up
	cJSON_Delete(json);
	free(fileContent);
}

void initTileSets(MAP* map,SDL_Renderer* rd) {
	map->map = NULL;
	map->map = malloc(15 * sizeof(long long int**));
	for (int i = 0; i < 15 && map->map; i++) {
		map->map[i] = malloc(100 * sizeof(long long int*));
		for (int j = 0; j < 100 && map->map[i]; j++) {
			map->map[i][j] = malloc(100 * sizeof(long long int));
		}
	}
	map->borders = malloc(100 * sizeof(SDL_Rect));
	map->objects = malloc(200 * sizeof(SDL_Rect));
	map->idObjs = malloc(200 * sizeof(int));
	map->totalBord = 0;
	map->totalObjs = 0;
	map->tilesets[0] = LoadTexture("assets/stoneWall.png", rd);
	map->widthText[0] = 512;
	map->firstgids[0] = 1;
	map->firstgids[1] = 257;
	map->tilesets[1] = LoadTexture("assets/grassTile.png", rd);
	map->widthText[1] = 256;
	map->firstgids[2] = 321;
	map->tilesets[2] = LoadTexture("assets/shadow.png", rd);
	map->widthText[2] = 512;
	map->firstgids[3] = 577;
	map->tilesets[3] = LoadTexture("assets/struct.png", rd);
	map->widthText[3] = 512;
	map->firstgids[4] = 833;
	map->tilesets[4] = LoadTexture("assets/props.png", rd);
	map->widthText[4] = 512;
	map->firstgids[5] = 1089;
	map->tilesets[5] = LoadTexture("assets/plant.png", rd);
	map->widthText[5] = 512;
	map->firstgids[6] = 1345;
	map->tilesets[6] = LoadTexture("assets/stoneGround.png", rd);
	map->widthText[6] = 256;
	map->firstgids[7] = 1409;
	map->tilesets[7] = LoadTexture("assets/shadowPlant.png", rd);
	map->widthText[7] = 512;
	map->firstgids[8] = 1665;
	map->tilesets[8] = LoadTexture("assets/tileset-grassland-water.png", rd);
	map->widthText[8] = 364;
	map->tilesets[9] = LoadTexture("assets/bridges.png", rd);
	map->widthText[9] = 832;
	map->firstgids[9] = 1731;
	map->tilesets[10] = LoadTexture("assets/snow.png", rd);
	map->widthText[10] = 1024;
	map->firstgids[10] = 2303;

}

int checkBox(long long int mapID, MAP* map) {
	int localVal_0 = mapID - map->firstgids[0];
	int localVal_4 = mapID - map->firstgids[4];
	int localVal_5 = mapID - map->firstgids[5];
	if (mapID > map->firstgids[4] && mapID < map->firstgids[5]) {
		if (localVal_4 != 56 && localVal_4 != 8) {
			return 0;
		}
	}
	if (mapID > map->firstgids[0] && mapID < map->firstgids[1]) {
		int nrBorders = sizeof(wallBorders) / sizeof(int);

		if (localVal_0 >= wallBorders[0] && localVal_0 <= wallBorders[nrBorders - 1]) {
			for (int j = 0; j < nrBorders; j++) {
				if (localVal_0 == wallBorders[j]) {
					return 0;
				}
			}
		}
	}
	if (mapID > map->firstgids[5] && mapID < map->firstgids[6]) {
		if (localVal_5 == 66 || localVal_5 == 70 || localVal_5 == 74) {
			return 0;
		}
		if (localVal_5 == 97 || localVal_5 == 99 || localVal_5 == 101 || localVal_5 == 107) {
			return 0;
		}
	}
	return 1;
}

int isWalkable(MAP* map,SDL_Rect playerPoz) {

	int isWalkable = 1;
	playerPoz.w -= 5;
	int objectOffset = map->firstgids[4];
	for (int i = 0; i < map->totalBord; i++) {
		if (SDL_HasIntersection(&playerPoz, &map->borders[i]))
			return 0;
	}
	for (int i = 0; i < 9; i++) {
		if (i == 5 || i == 6 || i == 8)
			continue;
		int x = (int)ceil(playerPoz.x / 32.0);
		int y = (int)floor(playerPoz.y / 32.0);

		int xz = (int)floor(playerPoz.x / 32.0);
		int yz = (int)ceil(playerPoz.y / 32.0);

		SDL_Rect box = { x * 32, y * 32,32 ,32 };
		SDL_Rect boxZ = { xz * 32, y * 32,32,32 };
		SDL_Rect boxZZ = { x * 32, yz * 32,32,32 };
		SDL_Rect boxZZZ = { xz * 32, yz * 32, 32,32 };
		if ((SDL_HasIntersection(&box, &playerPoz))) {
			int check = checkBox(map->map[i][y][x], map);
			if (!check) return 0;
		}
		if (SDL_HasIntersection(&boxZ, &playerPoz)) {
			int check = checkBox(map->map[i][y][xz], map);
			if (!check) return 0;
		}
		if (SDL_HasIntersection(&boxZZ, &playerPoz)) {
			int check = checkBox(map->map[i][yz][x], map);
			if (!check) return 0;
		}
		if (SDL_HasIntersection(&boxZZZ, &playerPoz)) {
			int check = checkBox(map->map[i][yz][xz], map);
			if (!check) return 0;
		}

	}
	return isWalkable;

}