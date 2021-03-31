#include "main.h"

#ifndef GENERATE_MAZE_H
#define GENERATE_MAZE_H
#include <stdlib.h>
#include<time.h>


struct Node {
    int x;
    int y;
    struct Node* parent;
    int value;
    int size;
};
struct Node* find_parent(struct Node* node);
struct Node* generate_node(int x, int y, int value);
int generate_maze(int* final_points_arr, int n);
int imposter_next_step();

#endif // BALL_H
