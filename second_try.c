#include "mainMenu.h"
#include "inventory.h"

SDL_Rect camera;

void initRect(SDL_Rect *obs,int x,int y, int w,int h) {
    obs->x = x;
    obs->y = y;
    obs->h = h;
    obs->w = w;
}

void updateCamera(PLAYER mainC,SDL_Rect border) {
    int old_cam_x = camera.x;
    int old_cam_y = camera.y;
    camera.x = (mainC.playerPoz.x + mainC.playerPoz.w / 2) - (camera.w / 2);
    camera.y = (mainC.playerPoz.y + mainC.playerPoz.h / 2) - (camera.h / 2);

    if(camera.x < 0)
        camera.x = 0;
    if(camera.y < 0) {
        camera.y = 0;
    }
    if(camera.x > border.w - camera.w) {
        camera.x = old_cam_x;
    }
    if(camera.y > border.h - camera.h) {
        camera.y = old_cam_y;
    }
}

void treatEvents (int *down,int *up,int *right, int *left, int *isRunning, int *flag) {
    SDL_Event ev;
    while(SDL_PollEvent(&ev)!=0) {
        if(ev.type == SDL_QUIT) {
            *isRunning = 0;
        } else if (ev.type == SDL_KEYDOWN){
            if(ev.key.keysym.sym == SDLK_s) {
                *down = 1;
            } else if(ev.key.keysym.sym == SDLK_w) {
                *up = 1;
            } else if(ev.key.keysym.sym == SDLK_d) {
                *right = 1;
            } else if(ev.key.keysym.sym == SDLK_a) {
                *left = 1;
            } else if(ev.key.keysym.sym == SDLK_i) {
                *flag = 1;
            }
        } else if(ev.type == SDL_KEYUP) {
            if(ev.key.keysym.sym == SDLK_s) {
                *down = 0;
            } else if(ev.key.keysym.sym == SDLK_w) {
                *up = 0;
            } else if(ev.key.keysym.sym == SDLK_d) {
                *right = 0;
            } else if(ev.key.keysym.sym == SDLK_a) {
                *left = 0;
            }
        }
    }
}

void treatAuroraAnimation(int *frameTime, SDL_Rect *playerPosition,int up,int down,int left,int right,int frameHeight,int frameWidth,int textureWidth,SDL_Rect *playerRect) {
    (*frameTime)++;
        if (down) {
            (*playerPosition).y += 3;
        }
        if (up) {
           (*playerPosition).y -= 3;
        }
        if (right) {
            (*playerPosition).x += 3;
        }
        if (left) {
            (*playerPosition).x -= 3;
        }
        if (*frameTime == 5) {
            *frameTime = 0;
            if (down == 1) {

                (*playerRect).y = 0;
            }
            if (up == 1) {
                (*playerRect).y = frameHeight*3;
            }
            if (right == 1) {
                (*playerRect).y = frameHeight*2;
            }
            if (left == 1) {
                (*playerRect).y = frameHeight;
                
            }
            if(left || right || up || down) {
                (*playerRect).x += frameWidth;
                if ((*playerRect).x + frameWidth >= textureWidth) {
                    (*playerRect).x = 0;
                }
            }
        }
}
void addItem(PLAYER *mainC, char location[50],SDL_Renderer *render) {
    for(int i = 0; i < 24; i++) {
        if(mainC->inv.back[i].state == 0) {
            mainC->inv.back[i].state = 1;
            mainC->inv.back[i].text = LoadTexture(location,render);
            break;
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    const int FPS = 60;
    int frameTime = 0;
    int textureWidth,textureHeight;
    int frameWidth,frameHeight;
    SDL_Window* window = NULL;
    SDL_Renderer *rendererTarget = NULL;
    PLAYER mainC;
    OBJECTS obj;
    obj.total = 0;

    int startWidth = 1920, startHeight = 1080;


    window = SDL_CreateWindow("Star Hunter",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,startWidth,startHeight,0);
    rendererTarget = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    initRender(rendererTarget);
    initWindow(window);
    int checkExit = mainMenu(&startWidth,&startHeight);
    if(checkExit == -1) { 
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(rendererTarget);
        SDL_Quit();
        return 0;
    }

    initRect(&camera,0,0,startWidth,startHeight);

    initRect(&mainC.playerPoz,0,0,48,64);
    for(int i = 0; i < 24; i++)
        mainC.inv.back[i].state = 0;
    for(int i = 0; i < 11; i++)
        mainC.inv.hotbar[i].state = 0;

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    mainC.image = LoadTexture("assets/alula.png",rendererTarget);
    SDL_Texture *map1 = LoadTexture("mapAlpha.png",rendererTarget);
    SDL_Rect map1Size;

    initRect(&map1Size,0,0,4000,4000);
    SDL_RenderCopy(rendererTarget,map1,&camera,&map1Size);
    updateCamera(mainC,map1Size);


    SDL_QueryTexture(mainC.image,NULL,NULL,&textureWidth,&textureHeight);
    frameWidth = textureWidth/4;
    frameHeight = textureHeight/4;

    mainC.sourceSize.x = mainC.sourceSize.y = 0;
    mainC.sourceSize.w = frameWidth;
    mainC.sourceSize.h = frameHeight;

    obj.objs[obj.total] = LoadTexture("assets/back.png",rendererTarget);
    initRect(&obj.objsSize[obj.total],2000,2000,100,100);
    obj.total++;



    int isRunning = 1;
    int down = 0, up = 0, right = 0, left = 0;
    addItem(&mainC,"assets/smallSound.png",rendererTarget);
    addItem(&mainC,"assets/mark.jpg",rendererTarget);
    while (isRunning) {
        int flag = -1;
        treatEvents(&down,&up,&right,&left,&isRunning,&flag);
        treatAuroraAnimation(&frameTime,&mainC.playerPoz,up,down,left,right,frameHeight,frameWidth,textureWidth,&mainC.sourceSize);
        updateCamera(mainC,map1Size);


        SDL_RenderClear(rendererTarget);
        SDL_RenderCopy(rendererTarget,map1,&camera,NULL);

        SDL_Rect renderQuad = { mainC.playerPoz.x - camera.x, mainC.playerPoz.y - camera.y, mainC.playerPoz.w, mainC.playerPoz.h };
        SDL_RenderCopy(rendererTarget, mainC.image, &mainC.sourceSize, &renderQuad);

        renderObjsInView(obj,rendererTarget,camera);

        SDL_RenderPresent(rendererTarget);

        if(flag == 1) {
            down = up = right = left = 0;
            inventory(camera,obj,&mainC,rendererTarget,startWidth,startHeight,map1);
        }

    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mainC.image);
    SDL_DestroyTexture(map1);
    for(int i = 0; i < obj.total; i++) {
        SDL_DestroyTexture(obj.objs[i]);
    }
    for(int i = 0; i < 24; i++) {
        if(mainC.inv.back[i].state ==  1)
        SDL_DestroyTexture(mainC.inv.back[i].text);
    }
    SDL_DestroyRenderer(rendererTarget);
    SDL_Quit();

    return 0;

}