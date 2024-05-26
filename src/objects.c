#include "objects.h"
#include "screen.h"

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
  else if (index == 3){
    screenSetColor(YELLOW, DARKGRAY);
    printf("刀");
  }
}