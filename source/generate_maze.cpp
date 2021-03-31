#include "generate_maze.h"
#include "main.h"
#include "character.h"

using namespace std;

int flag_print_for = 0;
struct Node* generate_node(int x, int y, int value){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->x = x;
    new_node->y = y;
    new_node->value = value;
    new_node->parent = NULL;
    new_node->size = 1;
    return new_node;
}

struct Node* find_parent(struct Node* node){
    while(node->parent!=NULL){
        node = node->parent;
    }
    return node;
}


int generate_maze(int* final_points_arr, int n){
    struct Node* boxes[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            boxes[i][j] = generate_node(i,j,i*n+j);
        }
    }
    vector < pair<int,int> > vertical_walls;
    for(int i=0;i<n;i++){
        for(int j=0;j<n-1;j++){
            vertical_walls.push_back(make_pair(i,j));
        }
    }
    // for(int i=0;i<vertical_walls.size();i++){
    //     cout<<vertical_walls[i].first<<" "<<vertical_walls[i].second<<"\n";
    // }
    vector < pair<int,int> > horizontal_walls;
    for(int i=0;i <n-1;i++){
        for(int j=0;j<n;j++){
            horizontal_walls.push_back(make_pair(i,j));
        }
    }
    srand(time(NULL));
    int flag = 0;
    while(1){
        // cout<<"bruh\n";
        // we remove vertical wall
        flag++;
        flag = flag%2;
        if(flag){
            int wall_to_remove = rand() % (vertical_walls.size());
            int to_remove_value_x = vertical_walls[wall_to_remove].first;
            int to_remove_value_y = vertical_walls[wall_to_remove].second;
            struct Node*  left_cell = boxes[to_remove_value_x][to_remove_value_y];
            struct Node* right_cell = boxes[to_remove_value_x][to_remove_value_y+1];
            struct Node* left_cell_parent = find_parent(left_cell);
            struct Node* right_cell_parent = find_parent(right_cell);
            if(right_cell_parent->value != left_cell_parent->value){
                // this is essentially removin the wall
                vertical_walls[wall_to_remove].first = vertical_walls[vertical_walls.size()-1].first;
                vertical_walls[wall_to_remove].second =  vertical_walls[vertical_walls.size()-1].second;
                vertical_walls.pop_back();
                // edges.push_back(make_pair(make_pair(to_remove_value_x,to_remove_value_y) , make_pair(to_remove_value_x,to_remove_value_y+1)));
                // edges.push_back(make_pair(make_pair(to_remove_value_x,to_remove_value_y+1) , make_pair(to_remove_value_x,to_remove_value_y)));
                edges.push_back(make_pair(make_pair(to_remove_value_y,to_remove_value_x) , make_pair(to_remove_value_y+1, to_remove_value_x)));
                edges.push_back(make_pair(make_pair(to_remove_value_y+1,to_remove_value_x) , make_pair(to_remove_value_y,to_remove_value_x)));
                // conneccted the two msts
                left_cell_parent->parent = right_cell_parent;
                right_cell_parent->size += left_cell_parent->size;
                if(right_cell_parent->size >= n*n){
                    break;
                }
            }
        }
        // we remove horizontal wall
        else{
            int wall_to_remove = rand() % (horizontal_walls.size());
            int to_remove_value_x = horizontal_walls[wall_to_remove].first;
            int to_remove_value_y = horizontal_walls[wall_to_remove].second;
            struct Node*  left_cell = boxes[to_remove_value_x][to_remove_value_y];
            struct Node* right_cell = boxes[to_remove_value_x+1][to_remove_value_y];
            struct Node* left_cell_parent = find_parent(left_cell);
            struct Node* right_cell_parent = find_parent(right_cell);
            if(right_cell_parent->value != left_cell_parent->value){
                // this is essentially removin the wall
                horizontal_walls[wall_to_remove].first = horizontal_walls[horizontal_walls.size()-1].first;
                horizontal_walls[wall_to_remove].second =  horizontal_walls[horizontal_walls.size()-1].second;
                horizontal_walls.pop_back();
                // edges.push_back(make_pair(make_pair(to_remove_value_x,to_remove_value_y) , make_pair(to_remove_value_x+1,to_remove_value_y)));
                // edges.push_back(make_pair(make_pair(to_remove_value_x+1,to_remove_value_y) , make_pair(to_remove_value_x,to_remove_value_y)));
                edges.push_back(make_pair(make_pair(to_remove_value_y,to_remove_value_x) , make_pair(to_remove_value_y,to_remove_value_x+1)));
                edges.push_back(make_pair(make_pair(to_remove_value_y,to_remove_value_x+1) , make_pair(to_remove_value_y,to_remove_value_x)));
                // conneccted the two msts
                left_cell_parent->parent = right_cell_parent;
                right_cell_parent->size += left_cell_parent->size;
                if(right_cell_parent->size >= n*n){
                    break;
                }
            }
        }
    }
    int final_points_arr_index = 0;
    // for(int i=0;i<vertical_walls.size();i++){
    //     cout<<vertical_walls[i].first<<" "<<vertical_walls[i].second<<"\n";
    // }
    // cout<<"other\n";
    // for(int i=0;i<horizontal_walls.size();i++){
    //     cout<<horizontal_walls[i].first<<" "<<horizontal_walls[i].second<<"\n";
    // }
    for(int i=0;i<vertical_walls.size();i++){
        final_points_arr[final_points_arr_index] = vertical_walls[i].second +1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = vertical_walls[i].first;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = 0;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = vertical_walls[i].second +1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = vertical_walls[i].first + 1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = 0;
        final_points_arr_index++;
    }
    for(int i=0;i<horizontal_walls.size();i++){
        final_points_arr[final_points_arr_index] = horizontal_walls[i].second;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = horizontal_walls[i].first+1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = 0;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = horizontal_walls[i].second +1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = horizontal_walls[i].first + 1;
        final_points_arr_index++;
        final_points_arr[final_points_arr_index] = 0;
        final_points_arr_index++;
    }
    int boundary[24] = { 0,0,0, n,0,0, 0,0,0, 0,n,0, 0,n,0, n,n,0, n,0,0, n,n,0 };
    for(int i=0;i<24;i++){
        final_points_arr[final_points_arr_index] = boundary[i];
        final_points_arr_index++;
    }

    return final_points_arr_index;
}



int imposter_next_step(){
    float y_init = character1.position.y+5.0;
    float x_init = character1.position.x +5.0;
    int y_char = (int)y_init;
    int x_char = (int)x_init;
    y_init = imposter.position.y +5.0;
    x_init = imposter.position.x +5.0;
    int y_imp = (int)y_init;
    int x_imp = (int)x_init;

    int distance[10][10];
    int parent[10][10];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            distance[i][j] = 100000;
            parent[i][j] = 0;
        }
    }
    distance[x_char][y_char] = 0;

    for(int i=0;i<100;i++){
        for(int j=0;j<edges.size();j++){
            int u_x = edges[j].first.first;
            int u_y = edges[j].first.second;
            int v_x = edges[j].second.first;
            int v_y = edges[j].second.second;
            if(flag_print_for==0){
                // printf("points : %d,%d;%d,%d\n",u_x,u_y,v_x,v_y);
            }
            if(distance[u_x][u_y]!= 100000 && distance[u_x][u_y] + 1 < distance[v_x][v_y]){
                distance[v_x][v_y]  = distance[u_x][u_y] + 1;
                if(u_x==v_x){
                    if(v_y-1==u_y){
                        parent[v_x][v_y] = 1;
                    }
                    else{
                        parent[v_x][v_y] = 2;
                    }
                }
                else{
                    if(v_x-1==u_x){
                        parent[v_x][v_y] = 4;
                    }
                    else{
                        parent[v_x][v_y] = 3;
                    }
                }
            }
        }
        flag_print_for = 1;
    }
    return(parent[x_imp][y_imp]);
}