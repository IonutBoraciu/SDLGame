#include "life.h"


void updateLife(PLAYER *mainC, SDL_Renderer *rd,int flag) {
	static float lastLife = -1;
	int fullHearts = mainC->life.life_value/10;
	int offset = mainC->life.destH.w + 10;
	float restHeart = mainC->life.life_value - ((int)mainC->life.life_value / 10) * 10;
	int totalHearts = fullHearts;

	if (restHeart) {
		totalHearts++;
	}
	int totalWidth = totalHearts * (mainC->life.destH.w + 10) - 10;
	int totalHeight = mainC->life.destH.h;
	if (mainC->life.life_value != lastLife || flag) {
		if (mainC->life.snapShot) {
			SDL_DestroyTexture(mainC->life.snapShot);
			mainC->life.snapShot = NULL;
		}
		SDL_Rect size = { 0,0,mainC->life.destH.w,mainC->life.destH.h };
		mainC->life.snapShot = SDL_CreateTexture(rd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, totalWidth, totalHeight);
		SDL_SetTextureBlendMode(mainC->life.snapShot, SDL_BLENDMODE_BLEND); // for transparent background
		SDL_SetRenderTarget(rd, mainC->life.snapShot);
		SDL_RenderClear(rd);

		while (fullHearts) {
			fullHearts--;
			SDL_RenderCopy(rd, mainC->life.hearts, &mainC->life.srcH[0], &size);
			size.x += offset;
		}

		if (restHeart == 7.5) {
			SDL_RenderCopy(rd, mainC->life.hearts, &mainC->life.srcH[1], &size);
		}
		else if (restHeart == 5.0) {
			SDL_RenderCopy(rd, mainC->life.hearts, &mainC->life.srcH[2], &size);
		}
		else if (restHeart == 2.5) {
			SDL_RenderCopy(rd, mainC->life.hearts, &mainC->life.srcH[3], &size);
		}
		SDL_SetRenderTarget(rd, NULL);
		lastLife = mainC->life.life_value;
	}
	SDL_Rect destLife = { mainC->life.destH.x,mainC->life.destH.y,totalWidth,totalHeight };
	if (mainC->life.snapShot) {
		SDL_RenderCopy(rd, mainC->life.snapShot, NULL, &destLife);

	}
}

void initLife(PLAYER* mainC, SDL_Renderer* rd,int w,int h) {

	mainC->life.life_value = 52.5;
	mainC->life.hearts = LoadTexture("assets/heart.png", rd);
	int widthH, heightH;
	SDL_QueryTexture(mainC->life.hearts, NULL, NULL, &widthH, &heightH);
	setSize(&mainC->life.destH, mainC->life.dest.w + mainC->life.dest.x + 10, mainC->life.dest.w / 3, 50, 50);
	if (w != 1920) {
		mainC->life.destH.w *= (double)w / 1920;
		mainC->life.destH.h *= (double)h / 1080;
	}
	int offset = 0;
	for (int i = 0; i < 4; i++) {
		mainC->life.srcH[i].x = offset;
		mainC->life.srcH[i].y = 0;
		mainC->life.srcH[i].w = widthH / 4;
		mainC->life.srcH[i].h = heightH;
		offset += widthH / 4 + 5;
	}
	mainC->life.snapShot = NULL;
	mainC->life.max_life = 100;
}

void addLife(PLAYER* mainC, float life) {
	mainC->life.life_value += life;
	if (mainC->life.life_value > mainC->life.max_life) {
		mainC->life.life_value = mainC->life.max_life;
	}
}

void removeLife(PLAYER* mainC, float life) {
	mainC->life.life_value -= life;
	if (mainC->life.life_value < 0) {
		mainC->life.life_value = 0;
	}
}

void treatSmallAlula(PLAYER* mainC) {
	static int frameTime = 150;

	int currentTime = SDL_GetTicks();
	static int lastFrameTime;

	if (currentTime > lastFrameTime + frameTime)  {
		lastFrameTime = currentTime;
		if (mainC->life.src.x + mainC->life.txtWidth / 4 + 20 <= mainC->life.txtWidth) {
			mainC->life.src.x = mainC->life.src.x + mainC->life.txtWidth / 4;
		}
		else {
			mainC->life.src.x = 0;
		}
	}
}
