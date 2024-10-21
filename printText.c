#include "printText.h"
#include "string.h"
#include "stdio.h"
#include "common.h"
#include "life.h"
#include <SDL_thread.h>
void destroy_texture(gpointer texture) {
	SDL_DestroyTexture((SDL_Texture*)texture);
}
void checkLeftMouse(int* clicked) {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0) {
		if (ev.type == SDL_MOUSEBUTTONDOWN) {
			if (ev.button.button == SDL_BUTTON_LEFT) {
				*clicked = 0;
			}
		}
	}
}

void waitLeftMouse() {
	int running = 1;
	while (running) {
		checkLeftMouse(&running);
	}
}

void printLetterOnScreen(D_TEXT alphabet, int sW, int sH, SDL_Renderer* rd, char* p, int i, int checkLeftClick, SDL_Rect* destSize, int type, SDL_Texture* text) {
	destSize->w = (double)alphabet.width[(int)p[i]] * 0.4;
	destSize->w *= (double)sW / 1920;
	SDL_Rect old_sz = *destSize;
	if (type) {
		destSize->h *= 0.8;
	}
	else {
		destSize->y -= (double)(6 * sH / 1080);
	}
	char ign[2] = { p[i],'\0' };
	SDL_SetRenderTarget(rd, text);
	SDL_RenderCopy(rd, g_hash_table_lookup(alphabet.letters, ign), NULL, destSize);
	SDL_SetRenderTarget(rd, NULL);
	*destSize = old_sz;
	destSize->x += destSize->w + 5;
}




void printToSreen(SDL_Renderer* rd, char message[256], SDL_Texture* box, SDL_Rect boxSz, SDL_Texture* face, int screenWidth, int screenHeight, D_TEXT alphabet, MY_SOUND sound, PLAYER mainC) {

	SDL_Rect faceSz = { boxSz.x + 10, boxSz.y + 10, 130,130 };
	SDL_Rect destSize = { boxSz.x,boxSz.y + 20,32,32 };
	int heightOffset = 32;
	int offset = 10;
	int hoffset = 35;
	int spaceSize = 15;
	SDL_Texture* snapShot = SDL_CreateTexture(rd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,screenWidth, screenHeight);
	SDL_Rect screen = { 0,0,screenWidth,screenHeight };
	void* pixels = malloc(screenWidth * screenHeight * 4);
	int pitch = screenWidth * 4;
	SDL_RenderReadPixels(rd, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);
	SDL_UpdateTexture(snapShot, NULL, pixels, pitch);
	free(pixels);

	SDL_RenderCopy(rd, snapShot, NULL, &screen);

	SDL_Texture* text = SDL_CreateTexture(rd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
	SDL_SetRenderTarget(rd, text);
	SDL_RenderClear(rd);
	SDL_SetRenderTarget(rd, NULL);
	SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);

	if (screenHeight != 1080) {

		heightOffset *= (double)screenHeight / 1080;
		offset *= (double)screenWidth / 1920;
		hoffset *= (double)screenHeight / 1080;
		faceSz.w *= (double)screenWidth / 1920;
		faceSz.h *= (double)screenHeight / 1080;
		destSize.h *= (double)screenHeight / 1080;
		spaceSize *= (double)screenWidth / 1920;
	}
	faceSz.y = boxSz.y + boxSz.h / 2 - faceSz.h / 2;
	destSize.x += faceSz.w + 20;
	char* p = strtok(message, " ");

	int channel = Mix_PlayChannel(-1, sound.soundEffects[0], -1);

	SDL_RenderCopy(rd, box, NULL, &boxSz);
	SDL_RenderCopy(rd, face, NULL, &faceSz);
	SDL_RenderCopyF(rd, mainC.life.alula, &mainC.life.src, &mainC.life.dest);
	treatSmallAlula(&mainC);
	SDL_RenderPresent(rd);

	int checkLeftClick = 1;
	int oldTime = SDL_GetTicks();
	int interval = 50;
	int i = 0;
	while (p != NULL) {
		if (!checkLeftClick) {
			interval = 10;
		}
		SDL_RenderClear(rd);
		SDL_RenderCopy(rd, snapShot, NULL, &screen);
		SDL_RenderCopy(rd, box, NULL, &boxSz);
		SDL_RenderCopyF(rd, mainC.life.alula, &mainC.life.src, &mainC.life.dest);
		if(face	!= NULL)
			SDL_RenderCopy(rd, face, NULL, &faceSz);
		SDL_RenderCopy(rd, text, NULL, &screen);
		treatSmallAlula(&mainC);
		int currentTime = SDL_GetTicks();
		float wordSize = 0;
		for (int j = 0; j < strlen(p) && !i; j++) {

			float totalVal = 0;
			totalVal = alphabet.width[(int)p[j]] * 0.4;
			totalVal *= (float)screenWidth / 1920;
			wordSize += totalVal + 5;
		}
		if (wordSize + destSize.x + offset < boxSz.x + boxSz.w) {
			if (checkLeftClick)
				checkLeftMouse(&checkLeftClick);
			if (currentTime - oldTime >= interval) {
				if (islower(p[i])) {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 1, text);

				}
				else if (isupper(p[i])) {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 0, text);
				}
				else {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 0, text);
				}
				i++;
				oldTime = currentTime;
			}

		}
		else if (destSize.y + heightOffset + hoffset < boxSz.y + boxSz.h) {
			if (currentTime - oldTime >= interval) {
				destSize.x = boxSz.x + faceSz.w + 20;
				destSize.y += heightOffset;
				if (checkLeftClick)
					checkLeftMouse(&checkLeftClick);
				if (islower(p[i])) {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 1, text);
				}
				else if (isupper(p[i])) {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 0, text);
				}
				else {
					printLetterOnScreen(alphabet, screenWidth, screenHeight, rd, p, i, checkLeftClick, &destSize, 0, text);

				}
				i++;
				oldTime = currentTime;
			}

		}
		else {
			p = NULL;
		}
		if (p != NULL && i >= strlen(p)) {
			p = strtok(NULL, " ");
			i = 0;
			destSize.x += spaceSize;
		}
		SDL_RenderPresent(rd);
	}


	Mix_HaltChannel(channel);
	int running = 1;
	while (running) {
		SDL_RenderClear(rd);
		SDL_RenderCopy(rd, snapShot, NULL, &screen);
		SDL_RenderCopy(rd, box, NULL, &boxSz);
		SDL_RenderCopyF(rd, mainC.life.alula, &mainC.life.src, &mainC.life.dest);
		if(face != NULL)
			SDL_RenderCopy(rd, face, NULL, &faceSz);
		SDL_RenderCopy(rd, text, NULL, &screen);
		treatSmallAlula(&mainC);
		checkLeftMouse(&running);
		SDL_RenderPresent(rd);
	}
	SDL_DestroyTexture(text);

}

void destroy_texture_2(gpointer texture) {
	SDL_DestroyTexture((SDL_Texture*)texture);
}

void initLetters(SDL_Renderer* rd, D_TEXT* alphabet) {

	char location[100] = "assets/letters/a.png";
	(*alphabet).letters = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, destroy_texture_2);
	for (int i = 0; i < 26; i++) {
		SDL_Texture* txt = LoadTexture(location, rd);
		char text[2];
		text[0] = (char)i + 97;
		text[1] = '\0';
		char* key = g_strdup(text);
		g_hash_table_insert((*alphabet).letters, key, txt);
		SDL_QueryTexture(txt, NULL, NULL, &alphabet->width[97 + i], NULL);
		location[15] = (char)(i + 98);
	}

	strcpy(location, "assets/letters/b_A.png");
	for (int i = 0; i < 26; i++) {
		SDL_Texture* txt = LoadTexture(location, rd);
		char text[2];
		text[0] = (char)i + 65;
		text[1] = '\0';
		char* key = g_strdup(text);
		g_hash_table_insert((*alphabet).letters, key, txt);
		SDL_QueryTexture(txt, NULL, NULL, &alphabet->width[65 + i], NULL);
		location[17] = (char)(i + 66);
	}
	char extension[5] = ".png";
	strcpy(location, "assets/letters/");
	const char specialCharacters[] = { '?', '!', ',', '.', '(', ')', '[', ']',':',';','\'' };
	int nSc = sizeof(specialCharacters) / sizeof(char);
	for (int i = 0; i < nSc; i++) {
		char completeLoc[100];
		completeLoc[0] = '\0';

		strcat(completeLoc, location);
		char position[5];
		sprintf(position, "%d", (int)specialCharacters[i]);
		strcat(completeLoc, position);
		strcat(completeLoc, extension);
		char text[2] = { specialCharacters[i],'\0' };
		char* key = g_strdup(text);
		SDL_Texture* txt = LoadTexture(completeLoc, rd);
		g_hash_table_insert((*alphabet).letters, key, txt);
		SDL_QueryTexture(txt, NULL, NULL, &alphabet->width[(int)specialCharacters[i]], NULL);

	}
}

