#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <stdio.h>

extern int hp;
extern int map_index;
extern int playerDamageBlink;
extern int x;
extern int y;

extern int swordactivetime;
extern int swordstun;
extern int swordX;
extern int swordY;

void enemyMoviment(int *enemy_spawn, int *enemyX, int *enemyY, int *enemy_damageBlink, int *enemy_hp, int enemy_speed, int *enemy_tickCount, int *enemy_stun, int *enemy_damaged, int enemy_damage);

int enemyCollision(int x, int y, int enemylocation, int spawn, int enemyX, int enemyY);

void printEnemy(int enemyX, int enemyY, int *enemyPrevX, int *enemyPrevY, int *enemyDamageBlink, int enemyDamaged, int color);

#endif /* __ENEMY_H__ */