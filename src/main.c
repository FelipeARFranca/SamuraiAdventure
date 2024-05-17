#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

// mapa

typedef struct map {
  int map_[MAXX][25];
} mapa;

mapa game_map[2];
int map_index = 0;

// player

int hp = 7;
int x = 20, y = 20;
char viewside;
int playerDamageBlink = 0;

int swordX, swordY;
int swordactivetime = 0;

// inimigo
int enemyspawn = 0;

int enemyHP = 2;
int enemySpeed = 4, enemyTickCount = 0;
int enemyX = 10, enemyY = 10; // x e y iniciais do inimigo
char enemyViewside;
int enemyPrevX, enemyPrevY;

int enemystun = 0;
int enemyDamageBlink = 0;
int enemyDamaged = 0;

int enemyCollision(int x, int y) {
    if(enemyX == x && enemyY == y) {
        return 1;
    } else {
        return 0;
    }
}

int collision(int originx, int originy, int targetx, int targety) {
  if((originx+2 == targetx || originx-2 == targetx || originx == targetx) && (originy+1 == targety || originy-1 == targety || originy == targety)) {
    return 1;
  } else {
    return 0;
  }
}

void printSword() {
  screenSetColor(CYAN, DARKGRAY);
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

void enemyMoviment() { 
  if(enemystun > 0) {
    enemystun--;
    return;
  }

  if(enemyTickCount < enemySpeed) {
      enemyTickCount++;
  } else {
      enemyTickCount = 0;
      enemyDamaged = 0;

      //colisão com a espada do player
      if(swordactivetime > 0) {
        if(collision(enemyX, enemyY, swordX, swordY) == 1) {
          enemyDamageBlink = 2;
          enemyHP--;
          if(enemyHP == 0) {
            enemyspawn = 1;
            printSword();
            return;
          }
          enemystun = 30;
          enemyDamaged = 1;
        }
      }

      if(enemyX > x && enemyX-2 != x && enemyX-2 != swordX ) {
          enemyX -= 2;
          enemyViewside = 'L';
      } else if(enemyX < x && enemyX+2 != x && enemyX+2 != swordX) {
          enemyX += 2;
          enemyViewside = 'R';
      } 

      if(enemyY > y && enemyY-1 != y && enemyY-1 != swordY) {
          enemyY -= 1;
          enemyViewside = 'U';
      } else if(enemyY < y && enemyY+1 != y && enemyY+1 != swordY) {
          enemyY += 1;
          enemyViewside = 'D';
      } 

      // dano no player
      if(collision(enemyX, enemyY, x, y) == 1) {
        hp--;
        enemystun = 20;
        playerDamageBlink = 2;
      }
  }
}
  

void printEnemy() {
    screenGotoxy(enemyPrevX, enemyPrevY);
    printf(" ");

    enemyMoviment();

    screenGotoxy(enemyX, enemyY);

    if(enemyDamageBlink == 0) {
      if(enemyDamaged == 0) screenSetColor(RED, DARKGRAY); 
      else screenSetColor(YELLOW, DARKGRAY);
  
      printf("鬼");
    } else {
      printf(" ");
      enemyDamageBlink--;
    }

    enemyPrevX = enemyX;
    enemyPrevY = enemyY;  
}

void print_MAP() {
  screenSetColor(CYAN, DARKGRAY);
  for (int i = 0; i < MAXX; i++) {
    for (int j = 0; j < MAXY; j++) {
      if (game_map[map_index].map_[i][j] == '1') {
        screenGotoxy(i, j);
        printf("█");
      } else if (game_map[map_index].map_[i][j] == '0') {
        screenGotoxy(i, j);
        printf(" ");
      }
    }
  }
}

int map_collision(int x, int y) {
  if (game_map[map_index].map_[x][y] == '1') {
    return 1;
  } else {
    return 0;
  }
}

void printPlayer(int nextX, int nextY) {
  screenSetColor(CYAN, DARKGRAY);
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

void printxy() {
  screenSetColor(BLUE, DARKGRAY);
  screenGotoxy(2, 0);
  printf(" Player X: %d Y: %d S: %c  ", x, y, viewside);
  screenGotoxy(2, 2);
  printf(" Enemy  X: %d Y: %d S: %c  ", enemyX, enemyY, enemyViewside);
  screenGotoxy(2, 3);
  printf(" HP: %d Enemy HP: %d", hp, enemyHP);
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(34, 22);
  printf("Key code: %d ", ch);
}

int main() {
  static int ch = 0;

  screenInit(0);
  keyboardInit();
  timerInit(50);
  print_MAP();
  printEnemy();
  printPlayer(x, y);
  screenUpdate();

  char spechar = '1';

  for (int i = 0; i < MAXX; i++) {
    game_map[0].map_[i][1] = spechar;
    game_map[0].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY; j++) {
    game_map[0].map_[0][j] = spechar;
    game_map[0].map_[80][j] = spechar;
  }

  game_map[0].map_[20][10] = '1';
  game_map[0].map_[20][11] = '1';
  game_map[0].map_[21][10] = '1';
  game_map[0].map_[21][11] = '1';

  for (int j = 30; j < 48; j++) {
    game_map[0].map_[j][23] = '0';
  }

  //----------------
  for (int i = 0; i < MAXX; i++) {
    game_map[1].map_[i][1] = spechar;
    game_map[1].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY; j++) {
    game_map[1].map_[0][j] = spechar;
    game_map[1].map_[80][j] = spechar;
  }

  game_map[1].map_[20][10] = '1';
  game_map[1].map_[20][11] = '1';
  game_map[1].map_[21][10] = '1';
  game_map[1].map_[21][11] = '1';

  for (int j = 30; j < 48; j++) {
    game_map[1].map_[j][1] = '0';
  }

  int newX = 40;
  int newY = 12;

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();
      printKey(ch);
    }

    // Update game state (move elements, verify collision, etc)
    if (timerTimeOver() == 1) {
      if (ch == 119 /*&& y - 1 >= MINY + 1*/ && map_collision(x, y - 1) == 0 && enemyCollision(x, y - 1) == 0){
        if(y - 1 == 0 && map_index == 1) {
          map_index = 0;
          newY = 24;
        } else {
          newY = y - 1;
        }
        viewside = 'U';
        ch = 0;
      } else if (ch == 115 /*&& y + 1 <= MAXY - 1*/ && map_collision(x, y + 1) == 0 && enemyCollision(x, y + 1) == 0){
        if(y + 1 == 24 && map_index == 0) {
          map_index = 1;
          newY = 1;
        } else {
          newY = y + 1;
        }
        viewside = 'D';
        ch = 0;
      } else if (ch == 97 /*&& x - 2 >= MINX + 1*/ && map_collision(x - 2, y) == 0 && enemyCollision(x - 2, y) == 0) {
        newX = x - 2;
        viewside = 'L';
        ch = 0;
      } else if (ch == 100 /*&& x + 2 < MAXX - 1*/ && map_collision(x + 2, y) == 0 && enemyCollision(x + 2, y) == 0) {
        newX = x + 2;
        viewside = 'R';
        ch = 0;
      }

      if(swordactivetime > 0) {
          printSword();
          swordactivetime--;
          if(swordactivetime == 0) {
            screenGotoxy(swordX, swordY);
            printf(" ");
          }
      }

      if(ch == 122) {
        if(!(swordactivetime > 0)) {
          swordactivetime = 100;
          printSword();
        }
      }

      if (ch == 44) {
        map_index = 0;
      }
      if (ch == 46) {
        map_index = 1;
      }

      print_MAP();
      printPlayer(newX, newY);
      printxy();

      if(enemyspawn == 0) printEnemy();
      else {
        screenGotoxy(enemyPrevX, enemyPrevY);
        printf(" ");
      }
      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}