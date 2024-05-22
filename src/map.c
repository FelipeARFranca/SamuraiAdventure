#include "map.h"
#include "screen.h"
#include "player.h"
#include "objects.h"

extern mapa game_map[8];
extern inventory player_inventory;
extern object katana;
extern object magatama;

extern int map_index;

extern int BlueOni_spawn;
extern int BlueOni_location;
extern int BlueOni_x;
extern int BlueOni_y;

extern int RedOni_spawn;
extern int RedOni_location;
extern int RedOni_x;
extern int RedOni_y;

extern int BossOni_spawn;
extern int BossOni_location;
extern int BossOni_x;
extern int BossOni_y;


int map_change(int index, int map_limitX, int map_limitY){
  if (map_limitY == 0){
    if (index == 1){
      return 0;
    }
    if (index == 4){
      return 5;
    }
    if (index == 5){
      return 6;
    }
  }
  else if (map_limitY == 24){
    if (index == 0){
      return 1;
    }
    if (index == 4){
      return 0;
    }
    if (index == 5){
      return 4;
    }
    if (index == 6){
      return 5;
    }
  }
  else if (map_limitX > 80){
    if (index == 1){
      return 2;
    }
    else if (index == 3){
      return 1;
    }
    else if (index == 7){
      return 3;
    }
  }
  else if (map_limitX < 0){
    if (index == 2){
      return 1;
    }
    else if (index == 1){
      return 3;
    }
    else if (index == 3){
      return 7;
    }
  }
  else if (index == 0 && map_limitX == 40 && map_limitY == 15){
    return 4;
  }

}

int map_collision(int x, int y) {
  if (game_map[map_index].map_[x][y] == '1' || game_map[map_index].map_[x][y] == '2' || game_map[map_index].map_[x][y] == '4') {
    return 1;
  }
  else if (game_map[map_index].map_[x][y] == '3' && player_inventory.key == 0){
    return 1;
  }
  else if (game_map[map_index].map_[x][y] == '5' && (player_inventory.weapon == 1 || player_inventory.key == 0)){
    
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
      else if (game_map[map_index].map_[i][j] == '5'){
        screenGotoxy(i, j);
        printf(" ");
      }
      else if (game_map[map_index].map_[i][j] == '0') {
        screenGotoxy(i, j);
        printf(" ");
      }
    }
  }

  if(BlueOni_spawn == 0 && map_index == BlueOni_location) {
    screenGotoxy(BlueOni_x, BlueOni_y);
    printf(" ");
  }

  if(RedOni_spawn == 0 && map_index == RedOni_location) {
    screenGotoxy(RedOni_x, RedOni_y);
    printf(" ");
  }

  if(BossOni_spawn == 0 && map_index == BossOni_location) {
    screenGotoxy(BossOni_x, BossOni_y);
    printf(" ");
  }
}

void print_MAP() {

  if (map_index == 0){
    screenSetColor(YELLOW, DARKGRAY);
  }
  else if (map_index == 1){
    screenSetColor(BLUE, DARKGRAY);
  }
  else if (map_index == 2){
    screenSetColor(RED, DARKGRAY);
  }
  else if (map_index == 3){
    screenSetColor(CYAN, DARKGRAY);
  }
  else if (map_index == 4){
    screenSetColor(GREEN, DARKGRAY);
  }
  else if (map_index == 5){
    screenSetColor(MAGENTA, DARKGRAY);
  }
  else if (map_index == 6){
    screenSetColor(WHITE, DARKGRAY);
  }
  else if (map_index == 7){
    screenSetColor(WHITE, DARKGRAY);
  }

  for (int i = 0; i < MAXX; i++) {
    for (int j = 0; j < MAXY; j++) {
      if (game_map[map_index].map_[i][j] == '1' || game_map[map_index].map_[i][j] == '5' ) { 
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

void draw_map() {
  char spechar = '1';

  //GOLDEN CASTLE 0 -------------------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < MAXX; i++) {
    game_map[0].map_[i][1] = spechar;
    game_map[0].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
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

  
  
  

  for (int j = 32; j < 48; j++) {
    game_map[0].map_[j][23] = '0';
  }

  //MAP 1---------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < MAXX; i++) {
    game_map[1].map_[i][1] = spechar;
    game_map[1].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[1].map_[0][j] = spechar;
    game_map[1].map_[80][j] = spechar;
  }


  for (int j = 32; j < 48; j++) {
    game_map[1].map_[j][1] = '0';
  }
  
  for (int j = 9; j <= 15; j++){
    game_map[1].map_[80][j] = '0';
    game_map[1].map_[0][j] = '0';
  }

  //MAP 2-----------------------------------------------------------------------------------------------------------------

  for (int i = 0; i < MAXX; i++) {
    game_map[2].map_[i][1] = spechar;
    game_map[2].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[2].map_[0][j] = spechar;
    game_map[2].map_[80][j] = spechar;
  }

  for (int j = 9; j <= 15; j++){
    game_map[2].map_[0][j] = '0';
  }
  
  for (int j = 5; j <= 9; j++){
    game_map[2].map_[10][j] = '1';
    game_map[2].map_[11][j] = '1';
    game_map[2].map_[69][j] = '1';
    game_map[2].map_[70][j] = '1';
  }

  for (int j = 15; j <= 19; j++){
    game_map[2].map_[10][j] = '1';
    game_map[2].map_[11][j] = '1';
    game_map[2].map_[69][j] = '1';
    game_map[2].map_[70][j] = '1';
  }

  for (int j = 9; j <= 15; j++){
    game_map[2].map_[15][j] = '1';
    game_map[2].map_[16][j] = '1';
    game_map[2].map_[64][j] = '1';
    game_map[2].map_[65][j] = '1';
  }

  for (int j = 12; j <= 14; j++){
    game_map[2].map_[j][9] = '1';
    game_map[2].map_[j][15] = '1';
  }

  for (int j = 66; j <= 68; j++){
    game_map[2].map_[j][9] = '1';
    game_map[2].map_[j][15] = '1';
  }

  for (int j = 6; j <= 18; j++){
    game_map[2].map_[24][j] = '1';
    game_map[2].map_[25][j] = '1';
    game_map[2].map_[55][j] = '1';
    game_map[2].map_[56][j] = '1';
  }

  for (int j = 2; j <= 5; j++){
    game_map[2].map_[33][j] = '1';
    game_map[2].map_[34][j] = '1';
    game_map[2].map_[46][j] = '1';
    game_map[2].map_[47][j] = '1';
  }

  for (int j = 19; j <= 22  ; j++){
    game_map[2].map_[33][j] = '1';
    game_map[2].map_[34][j] = '1';
    game_map[2].map_[46][j] = '1';
    game_map[2].map_[47][j] = '1';
  }

  //MAP 3-----------------------------------------------------------------------------------------------------------------

  for (int i = 0; i < MAXX; i++) {
    game_map[3].map_[i][1] = spechar;
    game_map[3].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[3].map_[0][j] = spechar;
    game_map[3].map_[80][j] = spechar;
  }

  for (int j = 9; j <= 15; j++){
    game_map[3].map_[80][j] = '0';
    game_map[3].map_[0][j] = '5';
  }

  //MAP 4-----------------------------------------------------------------------------------------------------------------

  for (int i = 0; i < MAXX; i++) {
    game_map[4].map_[i][1] = spechar;
    game_map[4].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[4].map_[0][j] = spechar;
    game_map[4].map_[80][j] = spechar;
  }

  for(int j = 39; j <= 41; j++){
    game_map[4].map_[j][23] = '0';
  }

  for (int j = 32; j < 48; j++) {
    game_map[4].map_[j][1] = '0';
  }

  char mazeline4[]  = "1111111111111111111100011111111111111111111000111111111111111111111111111111";
  char mazeline5[]  = "0000000000001100000000011000000000000000000000110001100000000000110000000000000";
  char mazeline6[]  = "0001111111111111111111111000111111111111111111110001100011111111111111111111111";
  char mazeline7[]  = "0000000000000000000000000000110000000000000000000001100011000000000000000000";
  char mazeline8[]  = "1111111111111111111000011111110001111111111110001111100011111111111111111111";
  char mazeline9[]  = "0001100000000000000000000000000001100000000110000000000000000000000000000011";
  char mazeline10[] = "0001111111111111111111111111111111100011000111111111111111111111111111100011";
  char mazeline11[] = "0001100000000000001100000000000001100011000111000000000000011000000000000011";
  char mazeline12[] = "0001100000000000001100011111110001100011000111000111111100011000000000000011";
  char mazeline13[] = "0001111111111110001100011000000001100011000111000110001100011000111111111111";
  char mazeline14[] = "0000000000000110001100011000110001100011000111000110001100011000110";
  char mazeline15[] = "1111111111000110001100011000110001100011111111000110001100011000110011111111111";
  char mazeline16[] = "0000000000000110001100011000110001100000000011000110001100011000110";
  char mazeline17[] = "0001111111111110001100011000110001111111100011000110001100011000111111111111";
  char mazeline18[] = "0001100000000000001100011000110000000000000011000110001100011000000000000011";
  char mazeline19[] = "0001100011111111111100011000111111111111111111111110001100011111111111100011";
  char mazeline20[] = "0001100000000000000000000000000000000000000000000000000000000000000000000011";
  char mazeline21[] = "0001111111111111110000000000000000000000000000000000000000000111111111111111";

  f = 0;

  for (e = 1; e < 77; e++){
    game_map[4].map_[e][4] = mazeline4[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 80; e++){
    game_map[4].map_[e][5] = mazeline5[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 80; e++){
    game_map[4].map_[e][6] = mazeline6[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][7] = mazeline7[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][8] = mazeline8[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][9] = mazeline9[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][10] = mazeline10[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][11] = mazeline11[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][12] = mazeline12[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][13] = mazeline13[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 68; e++){
    game_map[4].map_[e][14] = mazeline14[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 80; e++){
    game_map[4].map_[e][15] = mazeline15[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 68; e++){
    game_map[4].map_[e][16] = mazeline16[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][17] = mazeline17[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][18] = mazeline18[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][19] = mazeline19[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][20] = mazeline20[f];
    f++;
  }
  f = 0;
  for (e = 1; e < 77; e++){
    game_map[4].map_[e][21] = mazeline21[f];
    f++;
  }
  f = 0;

  

  //MAP 5-----------------------------------------------------------------------------------------------------------------

   for (int i = 0; i < MAXX; i++) {
    game_map[5].map_[i][1] = spechar;
    game_map[5].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[5].map_[0][j] = spechar;
    game_map[5].map_[80][j] = spechar;
  }

  for (int j = 32; j < 48; j++) {
    game_map[5].map_[j][1] = '0';
    game_map[5].map_[j][23] = '0';
  }

  //MAP 6------------------------------------------------------------------------------------------------------------------

  for (int i = 0; i < MAXX; i++) {
    game_map[6].map_[i][1] = spechar;
    game_map[6].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[6].map_[0][j] = spechar;
    game_map[6].map_[80][j] = spechar;
  }

  for (int j = 32; j < 48; j++) {
    game_map[6].map_[j][23] = '0';
  }

  //MAP 7------------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < MAXX; i++) {
    game_map[7].map_[i][1] = spechar;
    game_map[7].map_[i][23] = spechar;
  }

  for (int j = 1; j < MAXY-2; j++) {
    game_map[7].map_[0][j] = spechar;
    game_map[7].map_[80][j] = spechar;
  }

  for (int j = 9; j <= 15; j++){
    game_map[7].map_[80][j] = '5';
  }

  //----------------------------------------------------------------------------------------------------------------------

  player_inventory.weapon = 0;
  player_inventory.key = 0;

  katana.object_index = 1;
  katana.object_x = 20;
  katana.object_y = 18;

  magatama.object_index = 2;
  magatama.object_x = 70;
  magatama.object_y = 12;
}
