#include "mainMenu.h"
#include "inventory.h"
#include "gameMechanics.h"
#include <string.h>
#include <stdio.h>
#include "life.h"
#include "printText.h"
#include <stdlib.h>
#include "animation.h"
#include "enemy.h"
#include "map.h"
SDL_Rect camera;
void increaseHealth(int count, ...) {
	va_list args;
	va_start(args, count);

	PLAYER* point = va_arg(args,PLAYER *);
	addLife(point, 20);
}

void increaseSpeed(int count, ...) {
	va_list args;
	va_start(args, count);
	PLAYER* point = va_arg(args, PLAYER*);
	point->speed += 100;
}

void treatEvents(int* down, int* up, int* right, int* left, int* isRunning, int* flag) {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0) {
		if (ev.type == SDL_QUIT) {
			*isRunning = 0;
		}
		else if (ev.type == SDL_KEYDOWN) {
			if (ev.key.keysym.sym == SDLK_s) {
				*down = 1;
			}
			else if (ev.key.keysym.sym == SDLK_w) {
				*up = 1;
			}
			else if (ev.key.keysym.sym == SDLK_d) {
				*right = 1;
			}
			else if (ev.key.keysym.sym == SDLK_a) {
				*left = 1;
			}
			else if (ev.key.keysym.sym == SDLK_i) {
				*flag = -2;
			}
			else if (ev.key.keysym.sym == SDLK_ESCAPE) {
				*flag = -3;
			}
			else if (ev.key.keysym.sym >= SDLK_1 && ev.key.keysym.sym <= SDLK_9) {
				*flag = ev.key.keysym.sym - 48;
			}
			else if (ev.key.keysym.sym == SDLK_0) {
				*flag = 10;
			}
			else if (ev.key.keysym.sym == SDLK_MINUS) {
				*flag = 11;
			}
			else if (ev.key.keysym.sym == SDLK_o) {
				*flag = -7;
			}
		}
		else if (ev.type == SDL_KEYUP) {
			if (ev.key.keysym.sym == SDLK_s) {
				*down = 0;
			}
			else if (ev.key.keysym.sym == SDLK_w) {
				*up = 0;
			}
			else if (ev.key.keysym.sym == SDLK_d) {
				*right = 0;
			}
			else if (ev.key.keysym.sym == SDLK_a) {
				*left = 0;
			}
		}
		else if (ev.type == SDL_MOUSEBUTTONDOWN) {
			if (ev.button.button == SDL_BUTTON_LEFT) {
				printf("%d %d\n", ev.button.x, ev.button.y);
			}
		}
	}
}

int main() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	int textureWidth, textureHeight;
	int frameWidth, frameHeight;
	int isRunning = 1;
	int down = 0, up = 0, right = 0, left = 0;
	SDL_Window* window = NULL;
	SDL_Renderer* rendererTarget = NULL;
	PLAYER mainC;
	mainC.speed = 450;
	OBJECTS obj;
	GAME_MECHANICS gm;
	MY_SOUND sound;
	D_TEXT alphabet;
	ENEMY enemies[100];
	MAP mp;
	obj.total = 0;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("not working\n");
	}

	int vSound = 128 / 2;
	int vEffects = 128 / 2;

	int startWidth = 1920, startHeight = 1080;
	window = SDL_CreateWindow("Star Hunter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, startWidth, startHeight, 0);
	rendererTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	initFullSreen();

	initRender(rendererTarget);
	initWindow(window);
	int checkExit = mainMenu(&startWidth, &startHeight, 0, &vSound, &vEffects);
	if (checkExit == -1) {
		SDL_DestroyRenderer(rendererTarget);
		SDL_DestroyWindow(window);
		Mix_FreeMusic(sound.music[0]);
		Mix_CloseAudio();
		SDL_Quit();
		return 0;
	}
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	IMG_Init(imgFlags);

	init_stuff(startWidth, startHeight, &mainC, rendererTarget, &textureWidth, &textureHeight,&camera,&sound);
	frameWidth = textureWidth / 4;
	frameHeight = textureHeight / 4;

	initGM(&gm, camera, startWidth, startHeight, rendererTarget, &mainC);
	updateCamera(mainC, gm.map1Size, &camera);

	SDL_RenderClear(rendererTarget);
	SDL_Texture* loading = LoadTexture("assets/loading.png", rendererTarget);
	SDL_RenderCopy(rendererTarget, loading, NULL,NULL );
	SDL_RenderPresent(rendererTarget);

	obj.objs[obj.total] = LoadTexture("assets/back.png", rendererTarget);
	setSize(&obj.objsSize[obj.total], 2000, 2000, 100, 100);
	obj.total++;
	addItem(&mainC, "assets/turretFrame.png assets/turret_r.png assets/turret_l.png",rendererTarget,'w',24, NULL);
	addItem(&mainC, "assets/health.png", rendererTarget, 'c',-1, increaseHealth);
	addItem(&mainC, "assets/speed.png", rendererTarget, 'c',-1,increaseSpeed);
	addItem(&mainC, "assets/page.png", rendererTarget, 't',-1, NULL);
	SDL_Rect spawnPoint;
	int nOfEn = 0;
	setSize(&spawnPoint, 1000, 1000, 48, 64);
//	addEnemy("assets/robot.png", spawnPoint, enemies, rendererTarget, 4, 4,&nOfEn,500,500);
	mainC.time.lastAnimationTime = 0;
	mainC.time.lastTime = 0;

	Mix_VolumeMusic(vSound);
	for (int i = 0; i < sound.pozEffects; i++) {
		Mix_VolumeChunk(sound.soundEffects[i], vEffects);
	}
	Mix_PlayMusic(*sound.currentMusic, -1);

	gm.messageBox = LoadTexture("assets/textBox2.png", rendererTarget);
	SDL_Rect sBox;
	sBox.w = startWidth / 2;
	sBox.h = startHeight / 6;
	sBox.x = startWidth - 3 * sBox.w / 2;
	sBox.y = startHeight - sBox.h - (startHeight - (*mainC.inv.hotbar_sizes).y) - 30;
	initLetters(rendererTarget, &alphabet);

	gm.alulaFace = LoadTexture("assets/alulaFace.png", rendererTarget);
	int leftRight = 1;
	SDL_Texture* tilesets[10];
	int firstgids[10];
	int widthText[10];
	initTileSets(&mp, rendererTarget);
	parseMap("assets/map.json",&mp,11,100,100);
	createColissionMap(&mp);

	while (isRunning) {
		int flag = -1;
		int old_down = down;
		int old_up = up;
		int old_right = right;
		int old_left = left;
		int speedX = 0, speedY = 0;
		treatEvents(&down, &up, &right, &left, &isRunning, &flag);
		if (old_down != down || old_up != up || old_left != left || old_right != right) {
			treatAuroraAnimation(&mainC.playerPoz, up, down, left, right, frameHeight, frameWidth, textureWidth, &mainC.sourceSize, 1,&speedX,&speedY,&mainC.time,&mp,mainC.speed);
		}
		else {
			treatAuroraAnimation(&mainC.playerPoz, up, down, left, right, frameHeight, frameWidth, textureWidth, &mainC.sourceSize, 0, &speedX, &speedY,&mainC.time,&mp,mainC.speed);
		}
		treatTurretAnimation(&mainC, 0, speedX, speedY);
		updateCamera(mainC, gm.map1Size, &camera);
		SDL_RenderClear(rendererTarget);

		if (left)
			leftRight = 0;
		else if (right)
			leftRight = 1;

		renderMap(rendererTarget, &mp, 5, 100, 100, 32, 32,0,camera);
		renderMap(rendererTarget, &mp, 8, 100, 100, 32, 32, 7, camera);
		renderObject(rendererTarget, &mp, camera);
		SDL_Rect renderQuad = { mainC.playerPoz.x - camera.x, mainC.playerPoz.y - camera.y, mainC.playerPoz.w, mainC.playerPoz.h};
		SDL_RenderCopy(rendererTarget, mainC.image, &mainC.sourceSize, &renderQuad);
		renderMap(rendererTarget, &mp, 7, 100, 100, 32, 32, 5,camera);
		//renderMap(rendererTarget, &mp, 11, 100, 100, 32, 32, 7, camera);

		if (mainC.inv.mainWeapon.state) {
			SDL_Rect renderTurret = { mainC.inv.mainWeapon.currPosition.x - camera.x, mainC.inv.mainWeapon.currPosition.y - camera.y, mainC.inv.mainWeapon.currPosition.w,mainC.inv.mainWeapon.currPosition.h };
			if(leftRight)
				SDL_RenderCopy(rendererTarget, mainC.inv.mainWeapon.weapon, &mainC.inv.mainWeapon.sourcePostion, &renderTurret);
			else
				SDL_RenderCopy(rendererTarget, mainC.inv.mainWeapon.weaponLeft, &mainC.inv.mainWeapon.sourcePostion, &renderTurret);
		}

		//attack(enemies, nOfEn, mainC, camera);
		for (int i = 0; i < nOfEn; i++) {
			SDL_Rect poz = enemies[i].destSize;
			if (SDL_HasIntersection(&poz, &camera)) {
				SDL_Rect cameraRect = { poz.x - camera.x, poz.y - camera.y, poz.w, poz.h };
				DIR copy = enemies[i].dir;
				if (copy.up + copy.down + copy.left + copy.right >= 1) {
					treatAuroraAnimation(&enemies[i].destSize, copy.up, copy.down, copy.left, copy.right, 64, 48, enemies[i].width, &enemies[i].sourceSize, 2, &speedX, &speedY, &enemies[i].time,&mp,500);
				}
				if (enemies[i].destSize.x == enemies[i].spawnPoint.x && enemies[i].destSize.y == enemies[i].spawnPoint.y) {
					enemies[i].sourceSize.x = 0;
					enemies[i].sourceSize.y = 0;
				}
				SDL_RenderCopy(rendererTarget, enemies[i].character, &enemies[i].sourceSize, &cameraRect);
			}
		}

		renderObjsInView(obj, rendererTarget, camera);

		SDL_RenderCopy(rendererTarget, gm.hotbar, NULL, &gm.hotbarSz);
		updateHotbar(rendererTarget, &mainC);
		updateLife(&mainC, rendererTarget, 0);

		if (flag == -2) {
			down = up = right = left = 0;
			inventory(camera, obj, &mainC, rendererTarget, startWidth, startHeight, gm.map1,&mp,leftRight);
		}

		if (flag == -3) {
			down = up = right = left = 0;
			int old_width = startWidth;
			int old_height = startHeight;
			int old_sound = vSound;
			int old_effects = vEffects;

			double currentPosMusic = Mix_GetMusicPosition(*sound.currentMusic);
			int check = mainMenu(&startWidth, &startHeight, 1, &vSound, &vEffects);
			SDL_RenderClear(rendererTarget);

			Mix_PlayMusic(*sound.currentMusic, -1);
			Mix_SetMusicPosition(currentPosMusic);
			if (check == -1) {
				isRunning = 0;
				break;
			}
			if (old_width != startWidth) {
				initHotbar(&gm.hotbarSz, startWidth, startHeight);
				setSize(&camera, 0, 0, startWidth, startHeight);
				initButtons(&mainC, gm.hotbarSz, startWidth, startHeight);
				resizeF(&mainC.life.dest, (double)startWidth / old_width, (double)startHeight / old_height);
				resize(&mainC.life.destH, (double)startWidth / old_width, (double)startHeight / old_height);
				sBox.w = startWidth / 2;
				sBox.h = startHeight / 6;
				sBox.x = startWidth - 3 * sBox.w / 2;
				sBox.y = startHeight - sBox.h - (startHeight - (*mainC.inv.hotbar_sizes).y) - 30;
			}
			updateLife(&mainC, rendererTarget, 1);
			if (old_sound != vSound) {
				Mix_VolumeMusic(vSound);
			}
			if (old_effects != vEffects) {
				for (int i = 0; i < sound.pozEffects; i++) {
					Mix_VolumeChunk(sound.soundEffects[i], vEffects);
				}
			}
		}

		if (flag == -7) {
			char message[256] = "Hello, the feature actually works!";
			printToSreen(rendererTarget, message, gm.messageBox, sBox, gm.alulaFace, startWidth, startHeight, alphabet,sound,mainC);
			down = up = left = right = 0;
		}
		if (flag >= 1 && flag <= 11) {
			if (mainC.inv.hotbar[flag - 1].state) {
				if (mainC.inv.hotbar[flag - 1].f != NULL) {
					mainC.inv.hotbar[flag - 1].f(1, &mainC);
					if (mainC.inv.hotbar[flag - 1].type == 'c') {
						mainC.inv.hotbar[flag - 1].state = 0;
						SDL_DestroyTexture(mainC.inv.hotbar[flag - 1].text);
					}
				}
				else if (mainC.inv.hotbar[flag - 1].type == 't') {
					char message[256] = "Welcome to my first game made in C, only using the SDL library! Hope you will enjoy my demo!";
					printToSreen(rendererTarget, message, gm.messageBox, sBox, gm.alulaFace, startWidth, startHeight, alphabet, sound, mainC);
					down = up = left = right = 0;
				}
			}
		}
		SDL_RenderCopyF(rendererTarget, mainC.life.alula, &mainC.life.src, &mainC.life.dest);
		treatSmallAlula(&mainC);
		SDL_RenderPresent(rendererTarget);
	}
	for (int i = 0; i < nOfEn; i++) {
		SDL_DestroyTexture(enemies[i].character);
	}
	for (int i = 0; i < 9; i++) {
		SDL_DestroyTexture(mp.tilesets[i]);
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 100; j++) {
			free(mp.map[i][j]);
		}
	}
	for (int i = 0; i < 8; i++) {
		free(mp.map[i]);
	}
	free(mp.map);
	free(mp.borders);
	free(mp.idObjs);
	free(mp.objects);
	SDL_DestroyTexture(loading);
	freeData(mainC, gm, sound,obj,alphabet,rendererTarget,window);
	return 0;

}