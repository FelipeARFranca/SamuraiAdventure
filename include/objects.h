#ifndef __OBJECTS_H__
#define __OBJECTS_H__

typedef struct game_object{
  int object_index;
  int object_x;
  int object_y;
} object;

void print_object(int index, int x, int y);

#endif 