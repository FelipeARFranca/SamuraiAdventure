#include "enemy.h"
#include "screen.h"

extern int hp;
extern int map_index;
extern int playerDamageBlink;
extern int x;
extern int y;

extern int swordactivetime;
extern int swordstun;
extern int swordX;
extern int swordY;

int collision(int originx, int originy, int targetx, int targety) {
  if((originx+2 == targetx || originx-2 == targetx || originx == targetx) && (originy+1 == targety || originy-1 == targety || originy == targety)) {
    return 1;
  } else {
    return 0;
  }
}

void enemyMoviment(int *enemy_spawn, int *enemyX, int *enemyY, int *enemy_damageBlink, int *enemy_hp, int enemy_speed, int *enemy_tickCount, int *enemy_stun, int *enemy_damaged, int enemy_damage) { 
  if(*enemy_stun > 0) {
    *enemy_stun -= 1;  
    return;
  }

  if(*enemy_tickCount < enemy_speed) {
      *enemy_tickCount += 1;   
  } else {
      *enemy_tickCount = 0;
      *enemy_damaged = 0;

      //colisão com a espada do player
      if(swordactivetime > 0 & swordstun == 0) {
        if(collision(*enemyX, *enemyY, swordX, swordY) == 1) {
          swordstun = 40;
          swordactivetime = 0;
          screenGotoxy(swordX, swordY);
          printf(" ");
          
          *enemy_damageBlink = 2;
          *enemy_hp -= 1;

          if(*enemy_hp == 0) {
            *enemy_spawn = 1;
            screenGotoxy(*enemyX, *enemyY);
            printf(" ");
            return;
          }
          *enemy_stun = 30;
          *enemy_damaged = 1;
          
        }
      }
      

      if(*enemyX > x && *enemyX-2 != x && *enemyX-2 != swordX) {
          *enemyX -= 2;
      } else if(*enemyX < x && *enemyX+2 != x && *enemyX+2 != swordX) {
          *enemyX += 2;
      } 
      
      if(*enemyY > y && *enemyY-1 != y && *enemyY-1 != swordY) {
          *enemyY -= 1;
      } else if(*enemyY < y && *enemyY+1 != y && *enemyY+1 != swordY) {
          *enemyY += 1;
      } 

      // dano no player
      if(collision(*enemyX, *enemyY, x, y) == 1) {
        hp -= enemy_damage;
        *enemy_stun = 20;
        playerDamageBlink = 2;
      }
  }
}

int enemyCollision(int x, int y, int enemylocation, int spawn, int enemyX, int enemyY) {
    if(map_index == enemylocation && spawn == 0 && enemyX == x && enemyY == y) {
        return 1;
    } else {
        return 0;
    }
}

void printEnemy(int enemyX, int enemyY, int *enemyPrevX, int *enemyPrevY, int *enemyDamageBlink, int enemyDamaged, int color) {
    screenGotoxy(*enemyPrevX, *enemyPrevY);
    printf(" ");


    screenGotoxy(enemyX, enemyY);

    if(*enemyDamageBlink == 0) {    
      if(enemyDamaged != 0) screenSetColor(YELLOW, DARKGRAY); 
      else if(color == 1)screenSetColor(BLUE, DARKGRAY);
      else if(color == 2)screenSetColor(RED, DARKGRAY);

      printf("鬼");
    } else {
      printf(" ");
      *enemyDamageBlink -= 1;
    }
    *enemyPrevX = enemyX;
    *enemyPrevY = enemyY;

}