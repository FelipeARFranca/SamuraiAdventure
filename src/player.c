#include "player.h"
#include "screen.h"
#include "map.h"

extern inventory player_inventory;
extern int x;
extern int y;
extern char viewside;
extern int swordX;
extern int swordY;
extern int playerDamageBlink;

void printSword() {
  screenSetColor(WHITE, DARKGRAY);
  screenGotoxy(swordX, swordY);
  printf(" ");

  if(viewside == 'U') {
    screenGotoxy(x, y-1);
    printf("⟘");

    swordX = x;
    swordY = y - 1;

  } else if(viewside == 'D') {
    screenGotoxy(x, y+1);
    printf("⟙");
    swordX = x;
    swordY = y + 1;

  } else if(viewside == 'L') {
    screenGotoxy(x-2, y);
    printf("⟞");
    swordX = x - 2;
    swordY = y;

  } else if(viewside == 'R') {
    screenGotoxy(x+2, y);
    printf("⟝");
    swordX = x + 2;
    swordY = y;
  }
}

void printPlayer(int nextX, int nextY) {
  screenSetColor(WHITE, DARKGRAY);
  screenGotoxy(x, y);
  printf(" ");
  x = nextX;
  y = nextY;
  screenGotoxy(x, y);
   if(playerDamageBlink == 0) printf("侍");
    else {
      printf(" ");
      playerDamageBlink--;
    }
}

void print_inv_hud(int weapon, int key){
  screenGotoxy(59, 24);
  printf("Weapon = %d | Key = %d", weapon, key);
}