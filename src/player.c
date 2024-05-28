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
extern int hp;

void printSword() {
  if (player_inventory.weapon == 1){
    screenSetColor(WHITE, DARKGRAY);
  }
  else if (player_inventory.weapon > 1){
    screenSetColor(YELLOW, DARKGRAY);
  }
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

void printHP() {
  screenSetColor(RED, DARKGRAY);
  screenGotoxy(0, 24);
  printf("      ");
  printf("LIFE:");
  for(int i = 0; i < hp+2; i++) printf("  ");
  screenGotoxy(12, 24);
  for(int i = 0; i < hp; i++) printf("❤ ");
}

void print_inv_hud(int weapon, int key){
  screenSetColor(WHITE, DARKGRAY);
  screenGotoxy(58, 24);
  printf("INVENTORY");
  screenGotoxy(58, 25);
  printf("Weapon:");
  if (weapon == 0){
    screenGotoxy(65, 25);
    printf(" ");
  }
  else if (weapon == 1){
    screenGotoxy(65, 25);
    printf("『 刀 』");
  }
  else if (weapon > 1){
    screenGotoxy(65, 25);
    screenSetColor(YELLOW, DARKGRAY);
    printf("『 天照の聖刀 』");
  }

  screenGotoxy(58, 26);
  printf("Key:");
  if(key == 0){
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(63, 26);
    printf(" ");
  }
  else{
    screenSetColor(LIGHTGREEN, DARKGRAY);
    screenGotoxy(63, 26);
    printf("『 9 』");
  }
}
