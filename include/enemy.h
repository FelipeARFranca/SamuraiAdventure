#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <stdio.h>

void enemyMoviment(int *enemy_spawn, int *enemyX, int *enemyY, int *enemy_damageBlink, int *enemy_hp, int enemy_speed, int *enemy_tickCount, int *enemy_stun, int *enemy_damaged, int enemy_damage);

int enemyCollision(int x, int y);

void printEnemy(int enemyX, int enemyY, int *enemyPrevX, int *enemyPrevY, int *enemyDamageBlink, int enemyDamaged, int color);

void BossMoviment(int *boss_spawn, int *bossX, int *bossY, int *boss_damageBlink, int *boss_hp, int boss_speed, int *boss_tickCount, int *boss_stun, int *boss_damaged, int boss_damage);

int Boss_collision(int originx, int originy, int x, int y);

void printBoss(int bossX, int bossY, int *bossPrevX, int *bossPrevY, int *bossDamageBlink, int bossDamaged);

#endif /* __ENEMY_H__ */
