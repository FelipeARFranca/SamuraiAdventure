#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include "screen.h"

typedef struct map {
  int map_[MAXX][MAXY];
} mapa;

int map_change(int index, int map_limitX, int map_limitY);
void print_MAP();
int map_collision(int x, int y);
void map_clear();
void draw_map();

#endif /* __MAP_H__ */
