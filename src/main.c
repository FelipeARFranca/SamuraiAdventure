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
char viewside = 'U';
int x = 20, y = 20;

//sword
int swordPrevX, swordPrevY;
int swordactivetime = 0;

// inimigo

int enemyHP = 2;
char enemyViewside;
int enemystun = 0;
int enemyX = 10, enemyY = 10;
int enemyPrevX, enemyPrevY;
int enemyTickCount = 0, enemySpeed = 4;

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

void enemyMoviment() { 

    if(enemystun > 0) {
      enemystun--;
      return;
    }

    if(enemyTickCount < enemySpeed) {
        enemyTickCount++;
    } else {
        enemyTickCount = 0;

        if(enemyX > x && enemyX-2 != x) {
            enemyX -= 2;
            enemyViewside = 'L';
        } else if(enemyX < x && enemyX+2 != x) {
            enemyX += 2;
            enemyViewside = 'R';
        } 

        if(enemyY > y && enemyY-1 != y) {
            enemyY -= 1;
            enemyViewside = 'U';
        } else if(enemyY < y && enemyY+1 != y) {
            enemyY += 1;
            enemyViewside = 'D';
        } 

        // dano
        if(collision(enemyX, enemyY, x, y) == 1) {
          hp--;
          enemystun = 30;
        }
        
    }
}  

void printEnemy() {
    screenGotoxy(enemyPrevX, enemyPrevY);
    printf(" ");

    enemyMoviment();

    screenGotoxy(enemyX, enemyY);
    screenSetColor(RED, DARKGRAY);
    printf("鬼");

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

void printSword() {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(swordPrevX, swordPrevY);
  printf(" ");

  if(viewside == 'U') {
    screenGotoxy(x, y-1);
    printf("⟘");
    swordPrevX = x;
    swordPrevY = y - 1;

  } else if(viewside == 'D') {
    screenGotoxy(x, y+1);
    printf("⟙");
    swordPrevX = x;
    swordPrevY = y + 1;

  } else if(viewside == 'L') {
    screenGotoxy(x-2, y);
    printf("⟞");
    swordPrevX = x - 2;
    swordPrevY = y;

  } else if(viewside == 'R') {
    screenGotoxy(x+2, y);
    printf("⟝");
    swordPrevX = x + 2;
    swordPrevY = y;
  }
}

void printPlayer(int nextX, int nextY) {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" ");
  x = nextX;
  y = nextY;
  screenGotoxy(x, y);
  printf("侍");
}

void printxy() {
  screenSetColor(BLUE, DARKGRAY);
  screenGotoxy(2, 0);
  printf(" Player X: %d Y: %d S: %c  ", x, y, viewside);
  screenGotoxy(2, 2);
  printf(" Enemy  X: %d Y: %d S: %c  ", enemyX, enemyY, enemyViewside);
  screenGotoxy(2, 3);
  printf(" HP: %d ", hp);
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(35, 22);
  printf("Key code :");

  screenGotoxy(34, 23);
  printf("            ");

  if (ch == 27)
    screenGotoxy(36, 23);
  else
    screenGotoxy(39, 23);

  printf("%d ", ch);
  while (keyhit()) {
    printf("%d ", readch());
  }
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

      if(ch = 90) {
        if(swordactivetime > 0) {
          printSword();
          swordactivetime--;
        } else {
          swordactivetime = 20;
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
      printEnemy();
      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}