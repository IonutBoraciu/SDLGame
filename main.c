#include "mainMenu.h"
#include "inventory.h"
#include "gameMechanics.h"
#include <string.h>
#include <stdio.h>
#include "life.h"
SDL_Rect camera;

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
                *flag = 1;
            }
            else if (ev.key.keysym.sym == SDLK_ESCAPE) {
                *flag = 2;
            }
            else if (ev.key.keysym.sym == SDLK_1) {
                *flag = 3;
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

void treatAuroraAnimation(SDL_Rect* playerPosition, int up, int down, int left, int right, int frameHeight, int frameWidth, int textureWidth, SDL_Rect* playerRect) {
    static frameTime = 100;

    int currentTime = SDL_GetTicks();
    static int lastFrameTime;

    int speedX = 0;
    int speedY = 0;
    if (down) {
        speedY += 2;
    }
    if (up) {
        speedY -= 2;
    }
    if (right) {
        speedX += 2;
    }
    if (left) {
        speedX -= 2;
    }

    (*playerPosition).x += speedX;
    (*playerPosition).y += speedY;


    if (currentTime > lastFrameTime + frameTime) {
        lastFrameTime = currentTime;
        if (down == 1) {
            (*playerRect).y = 0;
        }
        if (up == 1) {
            (*playerRect).y = frameHeight * 3;
        }
        if (right == 1) {
            (*playerRect).y = frameHeight * 2;
        }
        if (left == 1) {
            (*playerRect).y = frameHeight;

        }
        if (left || right || up || down) {
            (*playerRect).x += frameWidth;
            if ((*playerRect).x + frameWidth >= textureWidth) {
                (*playerRect).x = 0;
            }
        }
    }
}

void treatSmallAlula(PLAYER *mainC) {
    static int frameTime = 0;
    frameTime++;

    if (frameTime >= 25) {
        if (mainC->life.src.x + mainC->life.txtWidth / 4 + 20 <= mainC->life.txtWidth) {
            mainC->life.src.x = mainC->life.src.x + mainC->life.txtWidth / 4;
        }
        else {
            mainC->life.src.x = 0;
        }
        frameTime = 0;
    }
}

void init_stuff(int startWidth,int startHeight,PLAYER *mainC,SDL_Renderer *rd,int *txtW,int *txtH) {
    setSize(&camera, 0, 0, startWidth, startHeight);
    setSize(&mainC->playerPoz, 200, 200, 48, 64);
    for (int i = 0; i < 24; i++)
        mainC->inv.back[i].state = 0;
    for (int i = 0; i < 11; i++)
        mainC->inv.hotbar[i].state = 0;
    mainC->inv.mainWeapon.state = 0;
    mainC->image = LoadTexture("assets/alula.png", rd);
    SDL_QueryTexture(mainC->image, NULL, NULL, txtW, txtH);
    int frameWidth = *txtW / 4;
    int frameHeight = *txtH / 4;

    mainC->sourceSize.x = mainC->sourceSize.y = 0;
    mainC->sourceSize.w = frameWidth;
    mainC->sourceSize.h = frameHeight;

    mainC->life.alula = LoadTexture("assets/alula_faces.png", rd);
    SDL_QueryTexture(mainC->life.alula, NULL, NULL, &mainC->life.txtWidth, &mainC->life.txtHeight);
    setSize(&mainC->life.src, 0, 0, mainC->life.txtWidth / 4, mainC->life.txtHeight);
    setSize(&mainC->life.dest, 0, 0, 100, 100);
    if (startWidth != 1920) {
        resize(&mainC->life.dest, (double)startWidth / 1920, (double)startHeight / 1080);
    }

    
    initLife(mainC, rd,startWidth,startHeight);
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
    OBJECTS obj;
    GAME_MECHANICS gm;
    obj.total = 0;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
         printf("not working\n");
    }

    MY_SOUND sound;
    sound.poz = 0;
    sound.music[sound.poz] = Mix_LoadMUS("gameMusic/title-theme.mp3");
    sound.currentMusic = &sound.music[sound.poz];
    sound.poz++;


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
        SDL_Quit();
        return 0;
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    IMG_Init(imgFlags);

    
    init_stuff(startWidth, startHeight, &mainC,rendererTarget,&textureWidth,&textureHeight);
    frameWidth = textureWidth / 4;
    frameHeight = textureHeight / 4;

    initGM(&gm, camera, startWidth, startHeight, rendererTarget, &mainC);
    updateCamera(mainC, gm.map1Size,&camera);


    obj.objs[obj.total] = LoadTexture("assets/back.png", rendererTarget);
    setSize(&obj.objsSize[obj.total], 2000, 2000, 100, 100);
    obj.total++;

    addItem(&mainC, "assets/smallSound.png", rendererTarget, 'w');
    addItem(&mainC, "assets/mark.jpg", rendererTarget, 'i');
    addItem(&mainC, "assets/mark.jpg", rendererTarget, 'i');
    addItem(&mainC, "assets/mark.jpg", rendererTarget, 'i');

    Mix_VolumeMusic(vSound);
    Mix_PlayMusic(*sound.currentMusic, -1);


    while (isRunning) {
        int flag = -1;
        treatEvents(&down, &up, &right, &left, &isRunning, &flag);
        treatAuroraAnimation(&mainC.playerPoz, up, down, left, right, frameHeight, frameWidth, textureWidth, &mainC.sourceSize);
        updateCamera(mainC, gm.map1Size, &camera);
        SDL_RenderClear(rendererTarget);


        SDL_RenderCopy(rendererTarget, gm.map1, &camera, NULL);

        SDL_Rect renderQuad = { mainC.playerPoz.x - camera.x, mainC.playerPoz.y - camera.y, mainC.playerPoz.w, mainC.playerPoz.h };
        SDL_RenderCopy(rendererTarget, mainC.image, &mainC.sourceSize, &renderQuad);

        renderObjsInView(obj, rendererTarget, camera);

        SDL_RenderCopy(rendererTarget, gm.hotbar, NULL, &gm.hotbarSz);
        updateHotbar(rendererTarget, &mainC);
        treatSmallAlula(&mainC);
        SDL_RenderCopy(rendererTarget, mainC.life.alula, &mainC.life.src, &mainC.life.dest);
        updateLife(&mainC, rendererTarget,0);


        SDL_RenderPresent(rendererTarget);

        if (flag == 1) {
            down = up = right = left = 0;
            inventory(camera, obj, &mainC, rendererTarget, startWidth, startHeight, gm.map1);
        }
        if (flag == 2) {
            down = up = right = left = 0;
            int old_width = startWidth;
            int old_height = startHeight;
            int old_sound = vSound;

            double currentPosMusic = Mix_GetMusicPosition(*sound.currentMusic);
            int check = mainMenu(&startWidth, &startHeight, 1, &vSound, &vEffects);

            Mix_PlayMusic(*sound.currentMusic, -1);
            Mix_SetMusicPosition(currentPosMusic);
            if (check == -1) {
                isRunning = 0;
                break;
            }
            if (old_width != startWidth) {

                initHotbar(&gm.hotbarSz, startWidth, startHeight);
                setSize(&camera, 0, 0, startWidth, startHeight);
                initButtons(&mainC, gm.hotbarSz, startWidth,startHeight);
                printf("%d %d\n", mainC.life.dest.w, mainC.life.dest.h);
                resize(&mainC.life.dest, (double)startWidth / old_width, (double)startHeight / old_height);
                printf("%d %d\n", mainC.life.dest.w, mainC.life.dest.h);
                resize(&mainC.life.destH, (double)startWidth / old_width, (double)startHeight / old_height);

                updateLife(&mainC, rendererTarget, 1);
            }
            if (old_sound != vSound) {
               Mix_VolumeMusic(vSound);
            }
        }

        if (flag == -7) {
            addLife(&mainC, 10);
        }
    }

    IMG_Quit();
    SDL_DestroyTexture(mainC.image);
    SDL_DestroyTexture(gm.map1);
    SDL_DestroyTexture(gm.hotbar);
    SDL_DestroyTexture(mainC.life.alula);
    SDL_DestroyTexture(mainC.life.hearts);
    SDL_DestroyTexture(mainC.life.snapShot);
    for (int i = 0; i < obj.total; i++) {
        SDL_DestroyTexture(obj.objs[i]);
    }
    for (int i = 0; i < 24; i++) {
        if (mainC.inv.back[i].state == 1)
            SDL_DestroyTexture(mainC.inv.back[i].text);
    }
    for (int i = 0; i < 11; i++) {
        if (mainC.inv.hotbar[i].state == 1) {
            SDL_DestroyTexture(mainC.inv.hotbar[i].text);
        }
    }
    if (mainC.inv.mainWeapon.state == 1) {
        SDL_DestroyTexture(mainC.inv.mainWeapon.text);
    }
    for (int i = 0; i < sound.poz; i++) {
        Mix_FreeMusic(sound.music[i]);
    }

    SDL_DestroyRenderer(rendererTarget);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;

}