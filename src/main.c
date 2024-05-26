#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#include "enemy.h"
#include "map.h"
#include "player.h"
#include "objects.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//mapa
mapa game_map[8];
int map_index = 6;

//inventario
inventory player_inventory;

object katana;
object magatama;
object seitou;

int play_time = 0;
int gameover = 0;
int gamewin = 0;

//lista de vencedores
struct vencedores {
  char nome[20];
  int tempo;
  struct vencedores *next;
};

struct vencedores *head = NULL;

// player
int player_spawn = 0;

int hp = 6;
int x = 21, y = 20;
char viewside;
int playerDamageBlink = 0;

int swordX, swordY;
int swordactivetime = 0;
int swordstun = 0;
int swordDamage = 1;

// inimigo BlueOni
int BlueOni_spawn = 0;
int BlueOni_location = 1;

int BlueOni_hp = 3;
int BlueOni_speed = 3, BlueOni_tickCount = 0;
int BlueOni_x = 10, BlueOni_y = 10; // x e y iniciais do inimigo
int BlueOni_prevX, BlueOni_prevY;
int BlueOni_damage = 1;

int BlueOni_stun = 0;   
int BlueOni_damageBlink = 0;
int BlueOni_damaged = 0;

// inimigo RedOni
int RedOni_spawn = 0;
int RedOni_location = 2;

int RedOni_hp = 4;
int RedOni_speed = 5, RedOni_tickCount = 0;
int RedOni_x = 20, RedOni_y = 20;
int RedOni_prevX, RedOni_prevY;
int RedOni_damage = 2;

int RedOni_stun = 0;
int RedOni_damageBlink = 0;
int RedOni_damaged = 0;

int BossOni_spawn = 0;
int BossOni_location = 5;

int BossOni_hp = 10;
int BossOni_speed = 3, BossOni_tickCount = 0;
int BossOni_x = 40, BossOni_y = 5; 
int BossOni_prevX, BossOni_prevY;
int BossOni_damage = 2;

int BossOni_stun = 50;
int BossOni_damageBlink = 0;
int BossOni_damaged = 0;


void printxy() {
  screenSetColor(RED, DARKGRAY);
  screenGotoxy(0, 24);
  printf("      ");
  printf("LIFE:");
  for(int i = 0; i < hp+2; i++) printf("  ");
  screenGotoxy(12, 24);
  for(int i = 0; i < hp; i++) printf("❤ ");
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(34, 22);
  printf("Key code: %d ", ch);
}

void add_jogador(struct vencedores **head, char *nome, int time) {
  struct vencedores *n = *head;
  struct vencedores *novo = (struct vencedores*) malloc(sizeof(struct vencedores));
  struct vencedores *anterior = NULL;

  strcpy(novo->nome,nome);
  novo->tempo = time;
  novo->next = NULL;

  if(*head == NULL) {
    *head = novo;
    return;
  }

  if((*head)->tempo > novo->tempo) {
    novo->next = *head;
    *head = novo;
    return;
  }

  while(n != NULL && n->tempo <= novo->tempo) {
    anterior = n;
    n = n->next;
  }

  if (anterior != NULL) {
    anterior->next = novo;
  }

  novo->next = n;
  return;
}

void loadwinnerlist() {
  FILE *list;
  char nome[20];
  int tempo;

  list = fopen("winners.txt","r");
  
  while(fscanf(list,"%s %d", nome, &tempo) == 2) {
    add_jogador(&head, nome, tempo);
  }

  fclose(list);
}

void printwinnerlist(struct vencedores *head) {
    struct vencedores *n = head;
    int i = 1;
    while (n != NULL && i <= 10) {
        printf("%d. %s: %d ticks\n",i, n->nome, n->tempo);
        n = n->next;
        i++;
    }
    printf("\n");
}

void writewinnerlist() {
  FILE *list;

  list = fopen("winners.txt","w");

  struct vencedores *n = head;

  while (n != NULL) {
      fprintf(list,"%s %d\n", n->nome, n->tempo);
      n = n->next;
  }

  fclose(list);
}

int main() {
  static int ch = 0;

  screenInit(0);
  keyboardInit();
  timerInit(50);
  print_MAP();
  printPlayer(x, y);
  screenUpdate();

  draw_map(); 

  int newX = 40;
  int newY = 19;

  while (gameover == 0 && gamewin == 0) // enter
  {
    // Handle user input
    if(swordactivetime == 0) {
      if (keyhit()) {
        ch = readch();
      }
    }

    // Update game state (move elements, verify collision, etc)
    if (timerTimeOver() == 1 && gameover == 0 && gamewin == 0) {
      play_time++;

      //W
      if (ch == 119 /*&& y - 1 >= MINY + 1*/ && map_collision(x, y - 1) == 0 && enemyCollision(x, y - 1) == 0 && Boss_collision(BossOni_x, BossOni_y,x, y - 1) == 0){
        if (map_index == 0 && x == 40 && y == 16){
          map_clear();
          map_index = map_change(map_index, x, y - 1);
          newY = 23;
        }
        else if(y == 1) {
          map_clear();
          map_index = map_change(map_index, x, y - 1);
          newY = 23;  
        } else {
          if (map_index == 0 && x == katana.object_x && y - 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 2 && x == magatama.object_x && y - 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          if (map_index == 7 && x == seitou.object_x && y - 1 == seitou.object_y && player_inventory.weapon == 0){
            player_inventory.weapon += 2;
          }
          newY = y - 1;
        }
        viewside = 'U';
        ch = 0;

      //S
      } else if (ch == 115 /*&& y + 1 <= MAXY - 1*/ && map_collision(x, y + 1) == 0 && enemyCollision(x, y + 1) == 0 && Boss_collision(BossOni_x, BossOni_y,x, y + 1) == 0){
        if(y == 23) {
          map_clear();
          map_index = map_change(map_index, x, y + 1);
          if (map_index == 0){
            newY = 16;
          }else {
            newY = 1;
          }
        } else {
          if (map_index == 0 && x == katana.object_x && y + 1 == katana.object_y && player_inventory.weapon == 0){
            player_inventory.weapon++;
          }
          if (map_index == 2 && x == magatama.object_x && y + 1 == magatama.object_y && player_inventory.key == 0){
            player_inventory.key++;
          }
          if (map_index == 7 && x == seitou.object_x && y + 1 == seitou.object_y && player_inventory.weapon == 0){
            player_inventory.weapon += 2;
          }
          newY = y + 1;
        }
        viewside = 'D';
        ch = 0;

      //A
      } else if (ch == 97 /*&& x - 2 >= MINX + 1*/ && map_collision(x - 2, y) == 0 && enemyCollision(x - 2, y) == 0 && Boss_collision(BossOni_x, BossOni_y,x - 2,y) == 0) {
        if (x == 0){
          map_clear();
          map_index = map_change(map_index, x - 2, y);
          newX = 80;
        } else {
          if (map_index == 0 && x - 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
              player_inventory.weapon++;
            }
          if (map_index == 2 && x - 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
              player_inventory.key++;
            }
          if (map_index == 7 && x - 2 == seitou.object_x && y == seitou.object_y && player_inventory.weapon == 0){
              player_inventory.weapon += 2;
            }
          newX = x - 2;
        }
        viewside = 'L';
        ch = 0;

      //D
      } else if (ch == 100 /*&& x + 2 < MAXX - 1*/ && map_collision(x + 2, y) == 0 && enemyCollision(x + 2, y) == 0 && Boss_collision(BossOni_x, BossOni_y,x + 2 ,y) == 0) {
        if (x == 80){
          map_clear();
          map_index = map_change(map_index, x + 2, y);
          newX = 0;
        } else {
          if (map_index == 0 && x + 2 == katana.object_x && y == katana.object_y && player_inventory.weapon == 0){
              player_inventory.weapon++;
            }
          if (map_index == 2 && x + 2 == magatama.object_x && y == magatama.object_y && player_inventory.key == 0){
              player_inventory.key++; 
            }
          if (map_index == 7 && x + 2 == seitou.object_x && y == seitou.object_y && player_inventory.weapon == 0){
              player_inventory.weapon += 2;
            }
          newX = x + 2;
        }
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
            swordstun = 10;
          }
      } else if(swordstun > 0) {
        swordstun--;
        if(swordstun == 0) {
          swordX = 0;
          swordY = 0;
        }
      }

      if (player_inventory.weapon != 0 && swordstun == 0){
        if(ch == 32 ) {
          if(swordactivetime == 0) {
            swordactivetime = 15;
            printSword();
            ch = 0;
          }
        }
      }

      if(player_inventory.weapon > 1){
        swordDamage = 4;
      }

      if(BossOni_hp > 0){
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][1] = '1';
        }
      }
      else{
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][1] = '0';
        }
      }

      if (map_index == 5 && newY < 23){
        for (int j = 32; j < 48; j++) {
          game_map[5].map_[j][23] = '1';
        }
      }

      if (map_index == 3 && x == 22 && y == 12){
        if (player_inventory.weapon == 1 || player_inventory.key == 0){
          screenGotoxy(38, 6);
          screenSetColor(WHITE, DARKGRAY);
          printf("。。。");
        }
        else{
          screenGotoxy(38, 6);
          screenSetColor(WHITE, DARKGRAY);
          printf("左の方へ");
        }
      }
      else{
        screenGotoxy(32, 6);
        printf("                       ");
      }


      if(map_index == 6) {
        screenGotoxy(20,20);
        printf("Θ");
        if(x == 20 && y == 20) gamewin = 1;
      }

      if (ch == 44) {
        map_index = 0;
      }
      if (ch == 46) {
        map_clear();
        map_index = 5;
      }

      print_MAP();

      if (map_index == 3){
        screenGotoxy(20, 12);
        if (player_inventory.weapon == 1 || player_inventory.key == 0){
          printf("?");
        }
        else {
          printf("!");
        }
      }
      else{
        screenGotoxy(20, 12);
        printf("  ");
      }

      if (map_index == 0 && player_inventory.weapon != 1){
        print_object(katana.object_index, katana.object_x, katana.object_y);
      }
      else{
        screenGotoxy(katana.object_x, katana.object_y);
        printf(" ");
      }
      if (map_index == 2 && player_inventory.key != 1){
        print_object(magatama.object_index, magatama.object_x, magatama.object_y);
      }
      else{
        screenGotoxy(magatama.object_x, magatama.object_y);
        printf(" ");
      }
      if (map_index == 7 && player_inventory.weapon < 2){
        print_object(seitou.object_index, seitou.object_x, seitou.object_y);
      }
      else{
        screenGotoxy(seitou.object_x, seitou.object_y);
        printf(" ");
      }

      if(BlueOni_spawn == 0 && map_index == BlueOni_location) {
        enemyMoviment(&BlueOni_spawn, &BlueOni_x, &BlueOni_y, &BlueOni_damageBlink, &BlueOni_hp, BlueOni_speed, &BlueOni_tickCount, &BlueOni_stun, &BlueOni_damaged, BlueOni_damage);
        printEnemy(BlueOni_x, BlueOni_y, &BlueOni_prevX, &BlueOni_prevY, &BlueOni_damageBlink, BlueOni_damaged, BlueOni_damage);
      }

      if(RedOni_spawn == 0 && map_index == RedOni_location) {
        enemyMoviment(&RedOni_spawn, &RedOni_x, &RedOni_y, &RedOni_damageBlink, &RedOni_hp, RedOni_speed, &RedOni_tickCount, &RedOni_stun, &RedOni_damaged, RedOni_damage);
        printEnemy(RedOni_x, RedOni_y, &RedOni_prevX, &RedOni_prevY, &RedOni_damageBlink, RedOni_damaged, RedOni_damage);
      }

      if (BossOni_spawn == 0 && map_index == BossOni_location) {
          BossMoviment(&BossOni_spawn, &BossOni_x, &BossOni_y, &BossOni_damageBlink, &BossOni_hp, BossOni_speed, &BossOni_tickCount, &BossOni_stun, &BossOni_damaged, BossOni_damage);
          printBoss(BossOni_x, BossOni_y, &BossOni_prevX, &BossOni_prevY, &BossOni_damageBlink, BossOni_damaged);
      }

      if(ch == 10) {

        break;
      }

      if(hp <= 0) {
        gameover = 1;
      }

      printPlayer(newX, newY);
      printxy();
      print_inv_hud(player_inventory.weapon, player_inventory.key);

      screenUpdate();
    }
  }
  screenDestroy();
  keyboardDestroy();
  timerDestroy();

  char final;

  if(gameover == 1) {
      printf(" ██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ ██████\n");
      printf("██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██   ██\n");
      printf("██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   ██████\n");
      printf("██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██   ██\n");
      printf(" ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██   ██\n\n");
      printf("Aperte ENTER para fechar o jogo");
      scanf("%c", &final);
      printf("\e[1;1H\e[2J");
  }

  if(gamewin == 1) {
    char nome_player[20];
    char c;
    int i = 0;
    loadwinnerlist();



    printf("Você venceu! Escreva seu nome para a Lista de Vencedores:");

    while ((c = getchar()) != '\n' && i < 19) {
      if(c == '\n' && c == '\0') break;
      nome_player[i] = c;
      i++;
    }

    printf("%s, tempo de jogo: %d\n\n", nome_player, play_time);
    add_jogador(&head, nome_player, play_time);

    printf("Lista dos 10 Melhores:\n");
    printwinnerlist(head);
    printf("\n\n");
    printf("Aperte ENTER para fechar o jogo\n");
    scanf("%c", &final);

    writewinnerlist();
    printf("\e[1;1H\e[2J");

  }
  return 0;
}
