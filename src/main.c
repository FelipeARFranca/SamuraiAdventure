#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>

// mapa

typedef struct map {
  int map_[MAXX][25];
} mapa;

//inventário do jogador

typedef struct player_inv{
  int weapon;
  int key;
}inventory;

//objetos no mapa
typedef struct game_object{
  int object_index;
  int object_x;
  int object_y;
} object;

mapa game_map[2];
int map_index = 0;

inventory player_inventory;

object katana;
object magatama;

// player

int hp = 7;
int x = 20, y = 20;
char viewside;
int playerDamageBlink = 0;

int swordX, swordY;
int swordactivetime = 0;
int swordstun = 0;

// inimigo
int BlueOni_spawn = 0;

int BlueOni_hp = 2;
int BlueOni_speed = 4, BlueOni_tickCount = 0;
int BlueOni_x = 10, BlueOni_y = 10; // x e y iniciais do inimigo
int BlueOni_prevX, BlueOni_prevY;

int BlueOni_stun = 0;
int BlueOni_damageBlink = 0;
int BlueOni_damaged = 0;

int enemyCollision(int x, int y) {
    if(map_index == 1 && BlueOni_spawn == 0 && BlueOni_x == x && BlueOni_y == y) {
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

void print_object(int index, int x, int y){
  screenGotoxy(x, y);
  if (index == 1){
    screenSetColor(WHITE, DARKGRAY);
    printf("刀");
  }
  else if (index == 2){
    screenSetColor(LIGHTGREEN, DARKGRAY);
    printf("9");
  }
}

void print_inv_hud(int weapon, int key){
  screenGotoxy(59, 24);
  printf("Weapon = %d | Key = %d", weapon, key);
}

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

void enemyMoviment() { 
  if(BlueOni_stun > 0) {
    BlueOni_stun--;
    return;
  }

  if(BlueOni_tickCount < BlueOni_speed) {
      BlueOni_tickCount++;
  } else {
      BlueOni_tickCount = 0;
      BlueOni_damaged = 0;

      //colisão com a espada do player
      if(swordactivetime > 0 & swordstun == 0) {
        if(collision(BlueOni_x, BlueOni_y, swordX, swordY) == 1) {
          swordstun = 40;
          screenGotoxy(swordX, swordY);
          printf(" ");
          
          BlueOni_damageBlink = 2;
          BlueOni_hp--;

          if(BlueOni_hp == 0) {
            BlueOni_spawn = 1;
            screenGotoxy(BlueOni_prevX, BlueOni_prevY);
            printf(" ");
            return;
          }
          BlueOni_stun = 30;
          BlueOni_damaged = 1;
          
        }
      }
      

      if(BlueOni_x > x && BlueOni_x-2 != x && BlueOni_x-2 != swordX) {
          BlueOni_x -= 2;
      } else if(BlueOni_x < x && BlueOni_x+2 != x && BlueOni_x+2 != swordX) {
          BlueOni_x += 2;
      } 
      
      if(BlueOni_y > y && BlueOni_y-1 != y && BlueOni_y-1 != swordY) {
          BlueOni_y -= 1;
      } else if(BlueOni_y < y && BlueOni_y+1 != y && BlueOni_y+1 != swordY) {
          BlueOni_y += 1;
      } 

      // dano no player
      if(collision(BlueOni_x, BlueOni_y, x, y) == 1) {
        hp--;
        BlueOni_stun = 20;
        playerDamageBlink = 2;
      }
  }
}
  

void printEnemy() {
    screenGotoxy(BlueOni_x, BlueOni_y);
    printf(" ");

    enemyMoviment();

    screenGotoxy(BlueOni_x, BlueOni_y);

    if(BlueOni_damageBlink == 0) {
      if(BlueOni_damaged == 0) screenSetColor(BLUE, DARKGRAY); 
      else screenSetColor(YELLOW, DARKGRAY);
  
      printf("鬼");
    } else {
      printf(" ");
      BlueOni_damageBlink--;
    }
}

void print_MAP() {
  screenSetColor(YELLOW, DARKGRAY);
  for (int i = 0; i < MAXX; i++) {
    for (int j = 0; j < MAXY; j++) {
      if (game_map[map_index].map_[i][j] == '1') { 
        screenGotoxy(i, j);
        printf("█");
      } 
      else if (game_map[map_index].map_[i][j] == '2'){
        screenGotoxy(i, j);
        printf("▓");
      }
      else if (game_map[map_index].map_[i][j] == '3'){
        screenGotoxy(i, j);
        printf("▒");
      }
      else if (game_map[map_index].map_[i][j] == '4'){
        screenGotoxy(i, j);
        printf("░");
      }
      else if (game_map[map_index].map_[i][j] == '0') {
        screenGotoxy(i, j);
        printf(" ");
      }
    }
  }
}

int map_collision(int x, int y) {
  if (game_map[map_index].map_[x][y] == '1' || game_map[map_index].map_[x][y] == '2' || game_map[map_index].map_[x][y] == '4') {
    return 1;
  }
  else if (game_map[map_index].map_[x][y] == '3' && player_inventory.key == 0){
    return 1;
  }
  else {
    return 0;
  }
}

void map_clear(){
  for (int i = 0; i < MAXX; i++) {
    for (int j = 0; j < MAXY; j++) {
      if (game_map[map_index].map_[i][j] == '1') { 
        screenGotoxy(i, j);
        printf(" ");
      } 
      else if (game_map[map_index].map_[i][j] == '2'){
        screenGotoxy(i, j);
        printf(" ");
      }
      else if (game_map[map_index].map_[i][j] == '3'){
        screenGotoxy(i, j);
        printf(" ");
      }
      else if (game_map[map_index].map_[i][j] == '4'){
        screenGotoxy(i, j);
        printf(" ");
      }
      else if (game_map[map_index].map_[i][j] == '0') {
        screenGotoxy(i, j);
        printf(" ");
      }
    }
  }

  if(BlueOni_spawn == 0 && map_index == 1) {
    screenGotoxy(BlueOni_x, BlueOni_y);
    printf(" ");
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

void printxy() {
  screenSetColor(BLUE, DARKGRAY);
  screenGotoxy(2, 24);
  printf("Player HP: %d X: %d Y: %d S: %c T: %d ", hp, x, y, viewside, swordstun);
  screenGotoxy(2, 25);
  printf("Blue Oni HP: %d X: %d Y: %d   ", BlueOni_hp, BlueOni_x, BlueOni_y);
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
  printPlayer(x, y);
  screenUpdate();

  char spechar = '1';

  for (int i = 0; i < MAXX; i++) {
    game_map[0].map_[i][1] = spechar;
    game_map[0].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-1; j++) {
    game_map[0].map_[0][j] = spechar;
    game_map[0].map_[80][j] = spechar;
  }
  
  char charline2[] = "122222222111222222222222222111222222221";
  char charline3[] = "11111111114111111111111111111141111111111";
  char charline4[] = "110114444444444114444444444444444411444444444411011";
  char charline5[] = "1144444441141144444444444444444444411411444444411";
  char charline6[] = "111111121144444444444444444444444441121111111";
  char charline7[] = "12222222111111111111111111111111122222221";
  char charline8[] = "12222222221222222222222222222212222222221";
  char charline9[] = "11111111111222222222222222222211111111111";
  char charline10[] = "11111111111111111111111";
  char charline11[] = "110114444444444444444444444411011";
  char charline12[] = "1144444444444444444444444444411";
  char charline13[] = "111111111111111111111111111";
  char charline14[] = "12222222211111222222221";
  char charline15[] = "12222222213331222222221";
  char charline16[] = "11111111113331111111111";

  int e;
  int f = 0;
  int line_start[] = {21, 20, 15, 16, 18, 20, 20, 20, 29, 24, 25, 27, 29, 29, 29};
  int line_end[] =   {59, 60, 65, 64, 62, 60, 60, 60, 51, 56, 55, 53, 51, 51, 51};

  
  for (e = 21; e <= 59; e++){
    game_map[0].map_[e][2] = charline2[f];
    f++;
  }
  f = 0;
  for (e = 20; e <= 60; e++){
    game_map[0].map_[e][3] = charline3[f];
    f++;
  }
  f = 0;
  for (e = 15; e <= 65; e++){
    game_map[0].map_[e][4] = charline4[f];
    f++;
  }
  f = 0;
  for (e = 16; e <= 64; e++){
    game_map[0].map_[e][5] = charline5[f];
    f++;
  }
  f = 0;
  for (e = 18; e <= 62; e++){
    game_map[0].map_[e][6] = charline6[f];
    f++;
  }
  f = 0;
  for (e = 20; e <= 60; e++){
    game_map[0].map_[e][7] = charline7[f];
    f++;
  }
  f = 0;
  for (e = 20; e <= 60; e++){
    game_map[0].map_[e][8] = charline8[f];
    f++;
  }
  f = 0;
  for (e = 20; e <= 60; e++){
    game_map[0].map_[e][9] = charline9[f];
    f++;
  }
  f = 0;
  for (e = 29; e <= 51; e++){
    game_map[0].map_[e][10] = charline10[f];
    f++;
  }
  f = 0;
  for (e = 24; e <= 56; e++){
    game_map[0].map_[e][11] = charline11[f];
    f++;
  }
  f = 0;
  for (e = 25; e <= 55; e++){
    game_map[0].map_[e][12] = charline12[f];
    f++;
  }
  f = 0;
  for (e = 27; e <= 53; e++){
    game_map[0].map_[e][13] = charline13[f];
    f++;
  }
  f = 0;
  for (e = 29; e <= 51; e++){
    game_map[0].map_[e][14] = charline14[f];
    f++;
  }
  f = 0;
  for (e = 29; e <= 51; e++){
    game_map[0].map_[e][15] = charline15[f];
    f++;
  }
  f = 0;
  for (e = 29; e <= 51; e++){
    game_map[0].map_[e][16] = charline16[f];
    f++;
  }
  f = 0;

  
  
  

  for (int j = 30; j < 48; j++) {
    game_map[0].map_[j][23] = '0';
  }

  //---------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < MAXX; i++) {
    game_map[1].map_[i][1] = spechar;
    game_map[1].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-1; j++) {
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

  player_inventory.weapon = 0;
  player_inventory.key = 0;

  katana.object_index = 1;
  katana.object_x = 20;
  katana.object_y = 18;

  magatama.object_index = 2;
  magatama.object_x = 70;
  magatama.object_y = 12;

  int newX = 40;
  int newY = 19;

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
          map_clear();
          map_index = 0;
          newY = 23;
        } else {
          if (map_index == 0 && x == katana.object_x && y - 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 1 && x == magatama.object_x && y - 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          newY = y - 1;
        }
        viewside = 'U';
        ch = 0;
      } else if (ch == 115 /*&& y + 1 <= MAXY - 1*/ && map_collision(x, y + 1) == 0 && enemyCollision(x, y + 1) == 0){
        if(y + 1 == 24 && map_index == 0) {
          map_clear();
          map_index = 1;
          newY = 1;
        } else {
          if (map_index == 0 && x == katana.object_x && y + 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 1 && x == magatama.object_x && y + 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          newY = y + 1;
        }
        viewside = 'D';
        ch = 0;
      } else if (ch == 97 /*&& x - 2 >= MINX + 1*/ && map_collision(x - 2, y) == 0 && enemyCollision(x - 2, y) == 0) {
        if (map_index == 0 && x - 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
        if (map_index == 1 && x - 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
        newX = x - 2;
        viewside = 'L';
        ch = 0;
      } else if (ch == 100 /*&& x + 2 < MAXX - 1*/ && map_collision(x + 2, y) == 0 && enemyCollision(x + 2, y) == 0) {
        if (map_index == 0 && x + 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
        if (map_index == 1 && x + 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
        newX = x + 2;
        viewside = 'R';
        ch = 0;
      }
      

      if(swordactivetime > 0 && swordstun == 0) {
          printSword();
          swordactivetime--;
          if(swordactivetime == 0) {
            screenGotoxy(swordX, swordY);
            printf(" ");

            swordX = 0;
            swordY = 0;
          }
      } else if(swordstun > 0) {
        swordstun--;
        if(swordstun == 0) {
          swordX = 0;
          swordY = 0;
        }
      }

      if (player_inventory.weapon != 0 && swordstun == 0){
        if(ch == 122 ) {
          if(swordactivetime == 0) {
            swordactivetime = 15;
            printSword();
            ch = 0;
          }
        }
      }

      if (ch == 44) {
        map_index = 0;
      }
      if (ch == 46) {
        map_index = 1;
      }

      print_MAP();
      if (map_index == 0 && player_inventory.weapon != 1){
        print_object(katana.object_index, katana.object_x, katana.object_y);
      }
      else{
        screenGotoxy(katana.object_x, katana.object_y);
        printf(" ");
      }
      if (map_index == 1 && player_inventory.key != 1){
        print_object(magatama.object_index, magatama.object_x, magatama.object_y);
      }
      else{
        screenGotoxy(magatama.object_x, magatama.object_y);
        printf(" ");
      }

      printPlayer(newX, newY);
      printxy();
      print_inv_hud(player_inventory.weapon, player_inventory.key);

      if(BlueOni_spawn == 0 && map_index == 1) printEnemy();
      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}