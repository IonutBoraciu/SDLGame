#include "enemy.h"
#include "stdio.h"
#include "math.h"
void addEnemy(char location[100], SDL_Rect spawnPoint, ENEMY* enemies, SDL_Renderer* rd, int ratioW, int ratioH,int *size,int rangeX,int rangeY) {
	static int pos;
	enemies[pos].character = LoadTexture(location, rd);
	SDL_QueryTexture(enemies[pos].character, NULL, NULL, &enemies[pos].width, &enemies[pos].height);
	setSize(&enemies[pos].sourceSize, 0, 0, enemies[pos].width / ratioW, enemies[pos].height / ratioH);
	enemies[pos].destSize = enemies[pos].spawnPoint = spawnPoint;
	enemies[pos].rangeX = rangeX;
	enemies[pos].rangeY = rangeY;
	enemies[pos].time.lastAnimationTime = 0;
	enemies[pos].time.lastTime = 0;
	enemies[pos].dir.left = enemies[pos].dir.right = 0;
	enemies[pos].dir.up = enemies[pos].dir.down = 0;
	pos++;
	*size = pos;

}

void static moveEnemy(ENEMY* enemy, SDL_Rect focus,int posX,int posY,DIR *dir) {
	int speedX = 1.5;
	int speedY = 1.5;
	int signX = 1;
	int signY = 1;
	int distance = focus.w + 20;
	int check = 0;
	if (focus.x + speedX * 2 + distance <= enemy->destSize.x && posX) {
		dir->left = 1;
		signX *= -1;
		check += 1;
	}
	if (focus.y + speedY * 2 <= enemy->destSize.y && posY) {
		dir->up = 1;
		signY *= -1;
		check += 1;
	}
	if (focus.y - speedY * 2 >= enemy->destSize.y && !posY) {
		dir->down = 1;
		check += 1;
	}
	if (focus.x - speedX * 2 + distance >= enemy->destSize.x && !posX) {
		dir->right = 1;
		check += 1;
	}
	if (check == 2) {
		enemy->destSize.x = enemy->destSize.x + (signX * sqrt(speedX));
		enemy->destSize.y = enemy->destSize.y + (signY * sqrt(speedY));
	}
	else if (dir->left) {
		enemy->destSize.x = enemy->destSize.x - speedX;
	}
	else if (dir->up) {
		enemy->destSize.y = enemy->destSize.y - speedY;
	}
	else if (dir->down) {
		enemy->destSize.y = enemy->destSize.y + speedY;
	}
	else if (dir->right) {
		enemy->destSize.x = enemy->destSize.x + speedX;
	}
}

int static checkOutofRange(ENEMY enemy, PLAYER mainC) {
	if (mainC.playerPoz.x < enemy.destSize.x) {
		if (enemy.destSize.x < enemy.spawnPoint.x - enemy.rangeX) {
			return 1;
		}
	}
	else {
		if (enemy.destSize.x > enemy.spawnPoint.x + enemy.rangeX) {
			return 1;
		}
	}
	return 0;
}

int static attackOne(ENEMY* enemy, DIR* movement, SDL_Rect focus) {
	if (focus.x < enemy->destSize.x && focus.x + enemy->rangeX >= enemy->spawnPoint.x) {
		if (focus.y < enemy->destSize.y) {
			if (focus.y + enemy->rangeY >= enemy->spawnPoint.y) {
				moveEnemy(enemy, focus,1,1,movement);
			}
		}
		else {
			if (focus.y - enemy->rangeY <= enemy->spawnPoint.y) {
				moveEnemy(enemy, focus, 1, 0,movement);
			}
		}
	}
	else if (focus.x - enemy->rangeX <= enemy->spawnPoint.x && focus.x >= enemy->destSize.x) {
		if (focus.y < enemy->destSize.y) {
			if (focus.y + enemy->rangeY >= enemy->spawnPoint.y) {
				moveEnemy(enemy, focus, 0, 1,movement);
			}
		}
		else {
			if (focus.y - enemy->rangeY <= enemy->spawnPoint.y) {
				moveEnemy(enemy, focus, 0, 0,movement);
			}
		}
	}
	if (movement->down + movement->up + movement->left + movement->right == 0) {
		return 0;
	}
	return 1;

}

void static moveSpawnPoint(ENEMY* enemy,DIR *dir) {
	int check = 0;
	int signX = 1;
	int signY = 1;
	int speedX = 1.5;
	int speedY = 1.5;
	if (enemy->destSize.x + 10 < enemy->spawnPoint.x) {
		dir->right = 1;
		check += 1;

	}
	else if(enemy->destSize.x >= enemy->spawnPoint.x + 10){
		dir->left = 1;
		check += 1;
		signX *= -1;
	}
	if (enemy->destSize.y < enemy->spawnPoint.y) {
		dir->down = 1;
		check += 1;

	}
	else {
		dir->up = 1;
		check += 1;
		signY *= -1;
	}
	if (check == 2) {
		enemy->destSize.x = enemy->destSize.x + signX * sqrt(speedX);
		enemy->destSize.y = enemy->destSize.y + signY * sqrt(speedY);
		
	}
	else if (dir->left) {
		enemy->destSize.x -= speedX;
	}
	else if (dir->right) {
		enemy->destSize.x += speedX;
	}
	else if (dir->up) {
		enemy->destSize.y -= speedY;
	}
	else if (dir->down) {
		enemy->destSize.y += speedY;
	}

}

void attack(ENEMY* enemies, int n,PLAYER mainC,SDL_Rect camera) {
	for (int i = 0; i < n; i++) {
		enemies[i].dir.left = enemies[i].dir.right = enemies[i].dir.down = enemies[i].dir.up = 0;
		if (SDL_HasIntersection(&camera, &enemies[i].destSize)) {
			int state = attackOne(&enemies[i],&enemies[i].dir, mainC.playerPoz);
			if (state == 0 && (enemies[i].destSize.x != enemies[i].spawnPoint.x || enemies[i].destSize.y != enemies[i].spawnPoint.y)) {
				int diff_x = abs(enemies[i].destSize.x - enemies[i].spawnPoint.x);
				int diff_y = abs(enemies[i].destSize.y - enemies[i].spawnPoint.y);
				if (diff_x < 15 && diff_y < 15) {
					enemies[i].destSize = enemies[i].spawnPoint;
				}
				else {
					moveSpawnPoint(&enemies[i], &enemies[i].dir);
				}

			}
		}
	}
}

