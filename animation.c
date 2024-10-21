#include "animation.h"

void treatAuroraAnimation(SDL_Rect* playerPosition, int up, int down, int left, int right, int frameHeight, int frameWidth, int textureWidth, SDL_Rect* playerRect, int force, int* spX, int* spY, aTime *time, MAP* map) {
    const int animationFrameTime = 95;
    int currentTime = SDL_GetTicks();
    int deltaTime = currentTime - time->lastTime;
    time->lastTime = currentTime;

    float speedFactor = deltaTime / 1000.0f;

    float speedX = 0.0f;
    float speedY = 0.0f;

    int val_ud = 0;
    int val_lr = 0;
    if (force != 2) {
        if (down) {
            speedY += 450 * speedFactor;
            val_ud += 2;
        }
        if (up) {
            speedY -= 450 * speedFactor;
            val_ud -= 2;
        }
        if (right) {
            speedX += 450 * speedFactor;
            val_lr += 2;
        }
        if (left) {
            speedX -= 450 * speedFactor;
            val_lr -= 2;
        }
        if ((up || down) && (right || left)) {
            speedY -= val_ud;
            if (val_ud > 0) {
                speedY += sqrt(2);
            }
            else {
                speedY -= sqrt(2);
            }
            speedX -= val_lr;
            if (val_lr > 0) {
                speedX += sqrt(2);
            }
            else {
                speedX -= sqrt(2);
            }
        }
        SDL_Rect oldPoz = *playerPosition;
        playerPosition->x += (int)speedX;
        playerPosition->y += (int)speedY;

        SDL_Rect feetCollisionBox = {
            playerPosition->x,
            playerPosition->y + playerPosition->h / 2,
            playerPosition->w,
            playerPosition->h / 2
        };
        if (!isWalkable(map, feetCollisionBox)) {
            *playerPosition = oldPoz;
            *spX = -1;
        }
    }
    if (*spX != -1) {
        *spX = (int)speedX;
        *spY = (int)speedY;
    }
    else {
        *spX = 0;
        *spY = 0;
    }
    if (currentTime > time->lastAnimationTime + animationFrameTime || force == 1) {
        time->lastAnimationTime = currentTime;
        if (down == 1) {
            playerRect->y = 0;
        }
        if (up == 1) {
            playerRect->y = frameHeight * 3;
        }
        if (right == 1) {
            playerRect->y = frameHeight * 2;
        }
        if (left == 1) {
            playerRect->y = frameHeight;
        }

        if (left || right || up || down) {
            playerRect->x += frameWidth;
            if (playerRect->x + frameWidth >= textureWidth) {
                playerRect->x = 0;
            }
        }
    }
}
void treatTurretAnimation(PLAYER* mainC, int side, int speedX, int speedY) {
    static int frameTime = 60;
    SDL_Rect* sourcePos = &mainC->inv.mainWeapon.sourcePostion;
    SDL_Rect* turrPos = &mainC->inv.mainWeapon.currPosition;
    int currentTime = SDL_GetTicks();
    static int lastFrameTime;
    turrPos->x += speedX;
    turrPos->y += speedY;
    if (currentTime > lastFrameTime + frameTime) {
        lastFrameTime = currentTime;
        if (sourcePos->x + sourcePos->w + 20 >= mainC->inv.mainWeapon.rWidth) {
            sourcePos->x = 0;
        }
        else {
            sourcePos->x += sourcePos->w;
        }

    }
}
