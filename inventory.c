#include "inventory.h"
#include <stdio.h>
int width;
int height;

int oldWidth;
int oldHeight;
SDL_Renderer* renderTarget;

SDL_Texture* createSnapShot(SDL_Rect camera, OBJECTS obj, PLAYER mainC, SDL_Texture* map) {
    SDL_Texture* snapShot = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(renderTarget, snapShot);
    SDL_RenderCopy(renderTarget, map, &camera, NULL);
    SDL_Rect renderQuad = { mainC.playerPoz.x - camera.x, mainC.playerPoz.y - camera.y, mainC.playerPoz.w, mainC.playerPoz.h };
    SDL_RenderCopy(renderTarget, mainC.image, &mainC.sourceSize, &renderQuad);
    renderObjsInView(obj, renderTarget, camera);
    SDL_SetRenderTarget(renderTarget, NULL);
    return snapShot;
}

void initSlots(SDL_Rect slots[3][8], SDL_Rect hotbar[11], SDL_Rect* mainWeapon) {
    setSize(&slots[0][0], 455, 343, 72, 77);
    float offset = 5;
    float offset2 = 0;
    float offset3 = 15;

    if (width != 1920) {
        resize(&slots[0][0], (float)width / 1920, (float)height / 1080);
        offset *= (float)width / 1920;
        offset3 *= (float)width / 1920;
        if (width > height * 2) {
            offset3 = 18;
        }

    }

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 8; i++) {
            if (i != 0) {
                setSize(&slots[j][i], slots[j][i - 1].x + slots[j][i - 1].w + offset, slots[0][0].y + offset2, slots[j][i - 1].w, slots[j][i - 1].h);
            }
            else {
                setSize(&slots[j][i], slots[0][0].x, slots[0][0].y + offset2, slots[0][0].w, slots[0][0].h);
            }
        }
        offset2 += offset3 + slots[j][0].h;
    }

    setSize(&hotbar[0], 455, 760, 72, 77);
    if (width != 1920) {
        resize(&hotbar[0], (float)width/1920,(float)height/1080);
    }
    for (int i = 1; i < 11; i++)
        setSize(&hotbar[i], hotbar[i - 1].x + hotbar[i - 1].w + offset, hotbar[i - 1].y, hotbar[i - 1].w, hotbar[i - 1].h);

    setSize(mainWeapon, 1370, 745, 106, 106);
    if (width != 1920) {
        resize(mainWeapon,(float)width/1920,(float)height/1080);
    }
}

void initBack(PLAYER mainC, SDL_Texture* snapShot, SDL_Texture* inventoryMenu, SDL_Rect inventSize, SDL_Rect slots[3][8], SDL_Rect hotbar[11], SDL_Rect mainWeapon) {
    SDL_RenderClear(renderTarget);
    SDL_RenderCopy(renderTarget, snapShot, NULL, &inventSize);
    SDL_RenderCopy(renderTarget, inventoryMenu, NULL, &inventSize);
    for (int i = 0; i < 24; i++) {
        if (mainC.inv.back[i].state) {
            int poz_i;
            int poz_j;
            poz_i = i / 8;
            if (poz_i > 2) {
                poz_i = 2;
                poz_j = 7;
            }
            else {
                poz_j = i % 8;
            }
            if (mainC.inv.back[i].text == NULL) {
                printf("HOPAA\n");
            }
            SDL_RenderCopy(renderTarget, mainC.inv.back[i].text, NULL, &slots[poz_i][poz_j]);
        }
    }
    for (int i = 0; i < 11; i++) {
        if (mainC.inv.hotbar[i].state)
            SDL_RenderCopy(renderTarget, mainC.inv.hotbar[i].text, NULL, &hotbar[i]);
    }
    if (mainC.inv.mainWeapon.state) {
        SDL_RenderCopy(renderTarget, mainC.inv.mainWeapon.text, NULL, &mainWeapon);
    }

}

void inventory(SDL_Rect camera, OBJECTS obj, PLAYER* mainC, SDL_Renderer* render, int widthCurr, int heightCurr, SDL_Texture* map) {

    SDL_Rect slots[3][8];
    SDL_Rect hotbar[11];
    SDL_Rect mainWeapon;
    initSlots(slots, hotbar, &mainWeapon);
    oldWidth = widthCurr;
    oldHeight = heightCurr;

    width = widthCurr;
    height = heightCurr;
    renderTarget = render;
    SDL_Texture* snapShot = createSnapShot(camera, obj, *mainC, map);
    SDL_Texture* inventoryMenu = LoadTexture("assets/inventory.png", renderTarget);
    SDL_Rect inventSize = { 0,0,width,height };

    initBack(*mainC, snapShot, inventoryMenu, inventSize, slots, hotbar, mainWeapon);
    SDL_RenderPresent(renderTarget);



    int isRunning = 1;
    int inArm = 0;
    ITEMS* aux = NULL;
    while (isRunning) {
        SDL_Event ev;
        if (inArm) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            SDL_RenderClear(renderTarget);
            initBack(*mainC, snapShot, inventoryMenu, inventSize, slots, hotbar, mainWeapon);
            SDL_Rect followMouse = { x,y,slots[0][0].w,slots[0][0].h };
            SDL_RenderCopy(renderTarget, aux->text, NULL, &followMouse);
            SDL_RenderPresent(renderTarget);
        }
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                isRunning = 0;
            }
            else if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_i && !inArm) {
                    isRunning = 0;
                }
            }
            else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    for (int j = 0; j < 3; j++) {
                        for (int i = 0; i < 8; i++) {
                            if (ev.button.x >= slots[j][i].x && ev.button.x <= slots[j][i].x + slots[j][i].w) {
                                if (ev.button.y >= slots[j][i].y && ev.button.y <= slots[j][i].y + slots[j][i].h) {
                                    if (mainC->inv.back[j * 8 + i].state != 0 && inArm == 0) {
                                        inArm = 1;
                                        aux = &mainC->inv.back[j * 8 + i];
                                        mainC->inv.back[j * 8 + i].state = 0;
                                    }
                                    else if (mainC->inv.back[j * 8 + i].state == 0 && inArm == 1) {
                                        inArm = 0;
                                        mainC->inv.back[j * 8 + i].state = 1;
                                        if (aux->text != mainC->inv.back[j * 8 + i].text) {
                                            mainC->inv.back[j * 8 + i].text = LoadTexture(aux->text_location,renderTarget);
                                            mainC->inv.back[j * 8 + i].type = aux->type;
                                            strcpy(mainC->inv.back[j * 8 + i].text_location, aux->text_location);
                                            SDL_DestroyTexture(aux->text);
                                            aux = NULL;
                                        }
                                        initBack(*mainC, snapShot, inventoryMenu, inventSize, slots, hotbar, mainWeapon);
                                        SDL_RenderPresent(renderTarget);
                                    }
                                }
                            }
                        }
                    }
                    for (int i = 0; i < 11; i++) {
                        if (ev.button.x >= hotbar[i].x && ev.button.x <= hotbar[i].x + hotbar[i].w) {
                            if (ev.button.y >= hotbar[i].y && ev.button.y <= hotbar[i].y + hotbar[i].h) {
                                if (inArm == 1) {
                                    if (mainC->inv.hotbar[i].state == 0) {
                                        inArm = 0;
                                        mainC->inv.hotbar[i].state = 1;
                                        if (aux->text != mainC->inv.hotbar[i].text) {
                                            mainC->inv.hotbar[i].text = LoadTexture(aux->text_location, renderTarget);
                                            mainC->inv.hotbar[i].type = aux->type;
                                            strcpy(mainC->inv.hotbar[i].text_location, aux->text_location);
                                            SDL_DestroyTexture(aux->text);
                                            aux = NULL;
                                        }
                                        initBack(*mainC, snapShot, inventoryMenu, inventSize, slots, hotbar, mainWeapon);
                                        SDL_RenderPresent(renderTarget);
                                    }
                                }
                                else if (mainC->inv.hotbar[i].state != 0) {
                                    inArm = 1;
                                    aux = &mainC->inv.hotbar[i];
                                    mainC->inv.hotbar[i].state = 0;
                                }
                            }
                        }
                    }
                    if (ev.button.x >= mainWeapon.x && ev.button.x <= mainWeapon.x + mainWeapon.w) {
                        if (ev.button.x >= mainWeapon.y && ev.button.y <= mainWeapon.y + mainWeapon.h) {
                            if (inArm == 1) {
                                if (!mainC->inv.mainWeapon.state && aux->type == 'w') {
                                    inArm = 0;
                                    mainC->inv.mainWeapon.state = 1;
                                    if (aux->text != mainC->inv.mainWeapon.text) {
                                        mainC->inv.mainWeapon.text = LoadTexture(aux->text_location, renderTarget);
                                        mainC->inv.mainWeapon.type = 'w';
                                        strcpy(mainC->inv.mainWeapon.text_location, aux->text_location);
                                        SDL_DestroyTexture(aux->text);
                                        aux = NULL;
                                    }
                                    initBack(*mainC, snapShot, inventoryMenu, inventSize, slots, hotbar, mainWeapon);
                                    SDL_RenderPresent(renderTarget);
                                }
                            }
                            else if (mainC->inv.mainWeapon.state) {
                                inArm = 1;
                                aux = &mainC->inv.mainWeapon;
                                mainC->inv.mainWeapon.state = 0;
                            }
                        }
                    }
                }

            }


        }
    }


    SDL_DestroyTexture(snapShot);
    SDL_DestroyTexture(inventoryMenu);

}
void addItem(PLAYER* mainC, char location[50], SDL_Renderer* render, char type) {

    int free = 1;

    if (type == 'w') {
        if (mainC->inv.mainWeapon.state == 0) {
            mainC->inv.mainWeapon.state = 1;
            mainC->inv.mainWeapon.text = LoadTexture(location, render);
            mainC->inv.mainWeapon.type = type;
            strcpy_s(mainC->inv.mainWeapon.text_location, 100, location);
            free = 0;
        }
    }

    for (int i = 0; i < 11 && free; i++) {
        if (mainC->inv.hotbar[i].state == 0) {
            mainC->inv.hotbar[i].state = 1;
            mainC->inv.hotbar[i].text = LoadTexture(location, render);
            mainC->inv.hotbar[i].type = type;
            strcpy_s(mainC->inv.hotbar[i].text_location, 100, location);
            free = 0;
        }
    }

    for (int i = 0; i < 24 && free; i++) {
        if (mainC->inv.back[i].state == 0) {
            mainC->inv.back[i].state = 1;
            mainC->inv.back[i].text = LoadTexture(location, render);
            mainC->inv.back[i].type = type;
            strcpy_s(mainC->inv.back[i].text_location, 100, location);
            free = 0;
        }
    }
}