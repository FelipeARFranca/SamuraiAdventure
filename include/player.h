#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdio.h>

typedef struct player_inv{
  int weapon;
  int key;
}inventory;

void printSword();
void printPlayer(int nextX, int nextY);
void print_inv_hud(int weapon, int key);
void printHP();

#endif /* __PLAYER_H__ */