#include "gameMechanics.h"
#include "common.h"
#include "life.h"
void updateCamera(PLAYER mainC, SDL_Rect border,SDL_Rect *camera) {
    camera->x = (mainC.playerPoz.x + mainC.playerPoz.w / 2) - (camera->w / 2);
    camera->y = (mainC.playerPoz.y + mainC.playerPoz.h / 2) - (camera->h / 2);

    if (camera->x < 0)
        camera->x = 0;
    if (camera->y < 0) {
        camera->y = 0;
    }
    if (camera->x > border.w - camera->w) {
        camera->x = border.w - camera->w;
    }
    if (camera->y > border.h - camera->h) {
        camera->y = border.h - camera->h;
    }
}

void updateHotbar(SDL_Renderer* render, PLAYER* mainC) {
    for (int i = 0; i < 11; i++) {
        if (mainC->inv.hotbar[i].state) {
            SDL_RenderCopy(render, mainC->inv.hotbar[i].text, NULL, &mainC->inv.hotbar_sizes[i]);
        }
    }
}

void initButtons(PLAYER* mainC, SDL_Rect hotbar_sz, int width,int height) {
    float offset_buttons = 8;
    setSize(&mainC->inv.hotbar_sizes[0], 588, 995, 60, 65);
    if (width != 1920) {
        resize(&mainC->inv.hotbar_sizes[0], (double)width / 1920, (double)height / 1080);
        offset_buttons *= (double)width / 1920;
        offset_buttons += 1;
    }
    for (int i = 1; i < 11; i++) {
        mainC->inv.hotbar_sizes[i] = mainC->inv.hotbar_sizes[i - 1];
        mainC->inv.hotbar_sizes[i].x += mainC->inv.hotbar_sizes[i - 1].w + offset_buttons;
        if (i % 2 == 0 && width != 1920) {
            mainC->inv.hotbar_sizes[i].x -= 1;
        }
    }
}

void initHotbar(SDL_Rect* hotbar_sz, int startWidth, int startHeight) {
    hotbar_sz->w = startWidth / 2.5;
    hotbar_sz->h = startHeight / 12;
    hotbar_sz->x = startWidth / 2 - hotbar_sz->w / 2;
    hotbar_sz->y = startHeight - hotbar_sz->h - 10;
}

void initMap(GAME_MECHANICS* gm, SDL_Rect camera, SDL_Renderer* rT) {
    gm->map1 = LoadTexture("assets/mapAlpha.png", rT);
    setSize(&gm->map1Size, 0, 0, 4000, 4000);
    SDL_RenderCopy(rT, gm->map1, &camera, &gm->map1Size);
}
void initHotbarG(GAME_MECHANICS* gm, int sW, int sH, SDL_Renderer* rT, PLAYER* mainC) {
    gm->hotbar = LoadTexture("assets/hotbar.png", rT);
    initHotbar(&gm->hotbarSz, sW, sH);
    initButtons(mainC, gm->hotbarSz, sW,sH);
}

void initGM(GAME_MECHANICS *gm, SDL_Rect camera, int sW, int sH, SDL_Renderer *rT, PLAYER *mainC) {
    initMap(gm, camera, rT);
    initHotbarG(gm, sW, sH, rT, mainC);
}

void initMySound(MY_SOUND* sound) {
    sound->poz = 0;
    sound->pozEffects = 0;
    sound->music[sound->poz] = Mix_LoadMUS("gameMusic/title-theme.mp3");
    sound->soundEffects[sound->pozEffects] = Mix_LoadWAV("soundEffects/text.mp3");
    sound->currentMusic = &sound->music[sound->poz];
    sound->poz++;
    sound->pozEffects++;
}

void init_stuff(int startWidth, int startHeight, PLAYER* mainC, SDL_Renderer* rd, int* txtW, int* txtH,SDL_Rect *camera,MY_SOUND *sound) {
    setSize(camera, 0, 0, startWidth, startHeight);
    setSize(&mainC->playerPoz, 200, 300, 32, 56);
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
    setSizeF(&mainC->life.dest, 0, 0, 100, 100);
    if (startWidth != 1920) {
        resizeF(&mainC->life.dest, (double)startWidth / 1920, (double)startHeight / 1080);
    }
    initLife(mainC, rd, startWidth, startHeight);
    initMySound(sound);
    for (int i = 0; i < 24; i++) {
        if (!i) {
            mainC->inv.mainWeapon.text = NULL;
        }
        if (i < 11) {
            mainC->inv.hotbar[i].text = NULL;
        }
        mainC->inv.back[i].text = NULL;
    }
}

void destroyPlayerData(PLAYER mainC) {
    SDL_DestroyTexture(mainC.image);
    SDL_DestroyTexture(mainC.life.alula);
    SDL_DestroyTexture(mainC.life.hearts);
    SDL_DestroyTexture(mainC.life.snapShot);
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
}

void destroyGM(GAME_MECHANICS gm) {
    SDL_DestroyTexture(gm.map1);
    SDL_DestroyTexture(gm.hotbar);
    SDL_DestroyTexture(gm.alulaFace);
    SDL_DestroyTexture(gm.messageBox);
}

void destroySound(MY_SOUND sound) {
    Mix_FreeChunk(sound.soundEffects[0]);
    for (int i = 0; i < sound.poz; i++) {
        Mix_FreeMusic(sound.music[i]);
    }
}

void freeData(PLAYER mainC, GAME_MECHANICS gm, MY_SOUND sound, OBJECTS obj, D_TEXT alphabet,SDL_Renderer* rd, SDL_Window *wd) {
    destroyPlayerData(mainC);
    destroyGM(gm);
    destroySound(sound);
    for (int i = 0; i < obj.total; i++) {
        SDL_DestroyTexture(obj.objs[i]);
    }
    IMG_Quit();
    g_hash_table_destroy(alphabet.letters);

    SDL_DestroyRenderer(rd);
    SDL_DestroyWindow(wd);
    Mix_CloseAudio();
    SDL_Quit();
}
