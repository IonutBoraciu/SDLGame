#include "gameMechanics.h"
#include "common.h"
void updateCamera(PLAYER mainC, SDL_Rect border,SDL_Rect *camera) {
    int old_cam_x = camera->x;
    int old_cam_y = camera->y;
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
    int offset_buttons = 8;
    setSize(&mainC->inv.hotbar_sizes[0], 588, 995, 60, 65);
    if (width != 1920) {
        resize(&mainC->inv.hotbar_sizes[0], (float)width / 1920, (float)height / 1080);
        offset_buttons *= (float)width / 1920;
    }
    for (int i = 1; i < 11; i++) {
        mainC->inv.hotbar_sizes[i] = mainC->inv.hotbar_sizes[i - 1];
        mainC->inv.hotbar_sizes[i].x += mainC->inv.hotbar_sizes[i - 1].w + offset_buttons;
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

