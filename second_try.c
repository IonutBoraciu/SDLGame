#include "mainMenu.h"

void treatEvents (int *down,int *up,int *right, int *left, int *isRunning) {
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
            (*playerPosition).y += 2;
        }
        if (up) {
           (*playerPosition).y -= 2;
        }
        if (right) {
            (*playerPosition).x += 2;
        }
        if (left) {
            (*playerPosition).x -= 2;
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

int main() {
    // VERY IMPORTANT DO NOT TOUCH.
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    const int FPS = 60;
    int frameTime = 0;
    int textureWidth,textureHeight;
    int frameWidth,frameHeight;
    SDL_Window* window = NULL;
    SDL_Texture *currentImage = NULL;
    SDL_Renderer *rendererTarget = NULL;
    SDL_Rect playerRect;
    SDL_Rect playerPosition;
    int startWidth = 1920, startHeight = 1080;


    window = SDL_CreateWindow("MyFirstGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,startWidth,startHeight,0);
    rendererTarget = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    initRender(rendererTarget);
    initWindow(window);
    int checkExit = mainMenu(&startWidth,&startHeight);
    if(checkExit == -1){ 
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(rendererTarget);
        return 0;
    }

    playerPosition.x = playerPosition.y = 0;
    playerPosition.w = 48;
    playerPosition.h = 64;



    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;


    currentImage = LoadTexture("assets/alula.png",rendererTarget);

    SDL_QueryTexture(currentImage,NULL,NULL,&textureWidth,&textureHeight);
    frameWidth = textureWidth/4;
    frameHeight = textureHeight/4;

    playerRect.x = playerRect.y = 0;
    playerRect.w = frameWidth;
    playerRect.h = frameHeight;

    SDL_SetRenderDrawColor(rendererTarget,0xFF,0,0,0xFF);

    int isRunning = 1;
    int down = 0, up = 0, right = 0, left = 0;
    while(isRunning) {

        treatEvents(&down,&up,&right,&left,&isRunning);
        treatAuroraAnimation(&frameTime,&playerPosition,up,down,left,right,frameHeight,frameWidth,textureWidth,&playerRect);
        
        SDL_RenderClear(rendererTarget);
        SDL_RenderCopy(rendererTarget,currentImage,&playerRect,&playerPosition);
        SDL_RenderPresent(rendererTarget);

    }
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(currentImage);
    SDL_DestroyRenderer(rendererTarget);

    return 0;

}