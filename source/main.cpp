#include "main.h"
#include "timer.h"
// #include "ball.h"
#include "maze.h"
#include "character.h"
#include "generate_maze.h"
#include "button.h"
#include <map>
#define GLT_IMPLEMENTATION
#include "gltext.h"
#include <time.h>

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// Ball ball1;
Maze maze1;
Character character1;
Character imposter;
Button button1;
Button button2;
Button final_button;
Button bomb_objects[3];
Button powerup_objects[3];
int bomb_objects_exist[3] = {1,1,1};
int powerup_objects_exist[3] = {1,1,1};
int object_flag = 1;
int move_imposter = 0;
int move_imposter_time =0;
int points[4000] = {0};
int final_points = 0;
int imposter_alive = 1;
int game_score = 0;
int game_time = 60;
int game_over_flag = 0;
int game_won_flag = 0;
int display_final_button = 0;
time_t game_start_time = time(NULL);
vector <pair < pair <int,int>, pair <int,int> > > edges;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);



void draw() {
    
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    glm::vec3 eye (5,5,-5);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (5, 5, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (1, 0, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    // ball1.draw(VP);
    if(!game_over_flag && !game_won_flag){
        maze1.draw(VP);
        character1.draw(VP);
        if(imposter_alive){
            imposter.draw(VP);
            button1.draw(VP);
        }
        if(object_flag){
            button2.draw(VP);
        }
        if(!object_flag){
            for(int i=0;i<3;i++){
                if(bomb_objects_exist[i]){
                    bomb_objects[i].draw(VP);
                }
                if(powerup_objects_exist[i]){
                    powerup_objects[i].draw(VP);
                }
            }
        }
        if(display_final_button){
            final_button.draw(VP);
        }
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (left) {
        if(!character_collide_with_wall(1)){
            character1.position.y -=0.1;
            // printf("%f,%f\n",character1.position.x,character1.position.y);
            if(character_collide_with_imposter_button(1) && imposter_alive){
                imposter_alive = 0;
                game_score+=2;
            }
            if(character_collide_with_powerup_button(1) && object_flag){
                object_flag=0;
            }
            if(character_collide_with_game_won_button(1) && display_final_button){
                game_won_flag =1;
            }
            if(!object_flag){
                for(int i=0;i<3;i++){
                    // printf("what\n");
                    if(powerup_objects_exist[i]){
                        // printf("what11\n");
                        if(character_collide_with_score_button(1,i)){
                            powerup_objects_exist[i] = 0;
                        }
                    }
                    if(bomb_objects_exist[i]){
                        if(character_collide_with_bomb_button(1,i)){
                            bomb_objects_exist[i] = 0;
                        }
                    }
                }
            }
        }
    }
    if(right){
        if(!character_collide_with_wall(2)){
            character1.position.y += 0.1;
            // printf("%f,%f\n",character1.position.x,character1.position.y);
            if(character_collide_with_imposter_button(1) && imposter_alive){
                imposter_alive = 0;
                game_score+=2;
            }
            if(character_collide_with_powerup_button(1) && object_flag){
                object_flag=0;
            }
            if(character_collide_with_game_won_button(1) && display_final_button){
                game_won_flag =1;
            }
            if(!object_flag){
                for(int i=0;i<3;i++){
                    if(powerup_objects_exist[i]){
                        if(character_collide_with_score_button(1,i)){
                            powerup_objects_exist[i] = 0;
                        }
                    }
                    if(bomb_objects_exist[i]){
                        if(character_collide_with_bomb_button(1,i)){
                            bomb_objects_exist[i] = 0;
                        }
                    }
                }
            }
        }
    }
    if(down){
        if(!character_collide_with_wall(4)){
            character1.position.x -= 0.1;
            if(character_collide_with_imposter_button(2) && imposter_alive){
                imposter_alive = 0;
                game_score+=2;
            }
            if(character_collide_with_powerup_button(2) && object_flag){
                object_flag=0;
            }
            if(character_collide_with_game_won_button(2) && display_final_button){
                game_won_flag =1;
            }
            if(!object_flag){
                for(int i=0;i<3;i++){
                    if(powerup_objects_exist[i]){
                        if(character_collide_with_score_button(2,i)){
                            powerup_objects_exist[i] = 0;
                        }
                    }
                    if(bomb_objects_exist[i]){
                        if(character_collide_with_bomb_button(2,i)){
                            bomb_objects_exist[i] = 0;
                        }
                    }
                }
            }
        }
    }
    if(up){
        if(!character_collide_with_wall(3)){
            character1.position.x += 0.1;
            if(character_collide_with_imposter_button(2) && imposter_alive){
                imposter_alive = 0;
                game_score+=2;
            }
            if(character_collide_with_powerup_button(2) && object_flag){
                object_flag=0;
            }
            if(character_collide_with_game_won_button(2) && display_final_button){
                game_won_flag =1;
            }
            if(!object_flag){
                for(int i=0;i<3;i++){
                    if(powerup_objects_exist[i]){
                        if(character_collide_with_score_button(2,i)){
                            powerup_objects_exist[i] = 0;
                        }
                    }
                    if(bomb_objects_exist[i]){
                        if(character_collide_with_bomb_button(2,i)){
                            bomb_objects_exist[i] = 0;
                        }
                    }
                }
            }
        }
    }

    // printf("%f,",character1.position.x);
    // printf("%f\n",character1.position.y);
}

int character_collide_with_bomb_button(int input, int index){
    float button_y = bomb_objects[index].position.y;
    float button_x = bomb_objects[index].position.x;
    float character_x = character1.position.x;
    float character_y = character1.position.y;
    // printf("player : %f,%f , button : %f,%f\n",character_x,character_y,button_x,button_y);
    if(input==1){
        // along y
        if((character_x+0.3 <= button_x+0.3 && character_x + 0.3 >= button_x-0.3) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x-0.3)){
            if((character_y+0.3 >= button_y-0.3 && character_y + 0.3 <= button_y) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y) ){
                // printf("collided\n");
                game_score -=1;
                return 1;
            }
        }
    }
    if(input==2){
        if((character_y+0.3 <= button_y+0.3 && character_y + 0.3 >= button_y-0.3) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y-0.3)){
            if((character_x+0.3 >= button_x-0.3 && character_x + 0.3 <= button_x) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x) ){
                // printf("collided\n");
                game_score -=1;
                return 1;
            }
        }
    }
    return 0;
}
int character_collide_with_game_won_button(int input){
    float button_y = final_button.position.y;
    float button_x = final_button.position.x;
    float character_x = character1.position.x;
    float character_y = character1.position.y;
    // printf("player : %f,%f , button : %f,%f\n",character_x,character_y,button_x,button_y);
    if(input==1){
        // along y
        if((character_x+0.3 <= button_x+0.3 && character_x + 0.3 >= button_x-0.3) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x-0.3)){
            if((character_y+0.3 >= button_y-0.3 && character_y + 0.3 <= button_y) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    if(input==2){
        if((character_y+0.3 <= button_y+0.3 && character_y + 0.3 >= button_y-0.3) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y-0.3)){
            if((character_x+0.3 >= button_x-0.3 && character_x + 0.3 <= button_x) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    return 0;
}
int character_collide_with_score_button(int input, int index){
    
    float button_y = powerup_objects[index].position.y;
    float button_x = powerup_objects[index].position.x;
    float character_x = character1.position.x;
    float character_y = character1.position.y;
    // printf("player : %f,%f , button : %f,%f\n",character_x,character_y,button_x,button_y);
    if(input==1){
        // along y
        if((character_x+0.3 <= button_x+0.3 && character_x + 0.3 >= button_x-0.3) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x-0.3)){
            if((character_y+0.3 >= button_y-0.3 && character_y + 0.3 <= button_y) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y) ){
                // printf("collided\n");
                game_score +=1;
                return 1;
            }
        }
    }
    if(input==2){
        if((character_y+0.3 <= button_y+0.3 && character_y + 0.3 >= button_y-0.3) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y-0.3)){
            if((character_x+0.3 >= button_x-0.3 && character_x + 0.3 <= button_x) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x) ){
                // printf("collided\n");
                game_score +=1;
                return 1;
            }
        }
    }
    return 0;
}
int character_collide_with_imposter_button(int input){
    float button_y = button1.position.y;
    float button_x = button1.position.x;
    float character_x = character1.position.x;
    float character_y = character1.position.y;
    // printf("player : %f,%f , button : %f,%f\n",character_x,character_y,button_x,button_y);
    if(input==1){
        // along y
        if((character_x+0.3 <= button_x+0.3 && character_x + 0.3 >= button_x-0.3) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x-0.3)){
            if((character_y+0.3 >= button_y-0.3 && character_y + 0.3 <= button_y) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    if(input==2){
        if((character_y+0.3 <= button_y+0.3 && character_y + 0.3 >= button_y-0.3) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y-0.3)){
            if((character_x+0.3 >= button_x-0.3 && character_x + 0.3 <= button_x) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    return 0;
}
int character_collide_with_powerup_button(int input){
    float button_y = button2.position.y;
    float button_x = button2.position.x;
    float character_x = character1.position.x;
    float character_y = character1.position.y;
    // printf("player : %f,%f , button : %f,%f\n",character_x,character_y,button_x,button_y);
    if(input==1){
        // along y
        if((character_x+0.3 <= button_x+0.3 && character_x + 0.3 >= button_x-0.3) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x-0.3)){
            if((character_y+0.3 >= button_y-0.3 && character_y + 0.3 <= button_y) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    if(input==2){
        if((character_y+0.3 <= button_y+0.3 && character_y + 0.3 >= button_y-0.3) || (character_y-0.3 <= button_y+0.3 && character_y - 0.3 >= button_y-0.3)){
            if((character_x+0.3 >= button_x-0.3 && character_x + 0.3 <= button_x) || (character_x-0.4 <= button_x+0.3 && character_x - 0.4 >= button_x) ){
                // printf("collided\n");
                return 1;
            }
        }
    }
    return 0;
}
int character_collide_with_wall(int input){
    float y_init = character1.position.y+5.0;
    float x_init = character1.position.x +5.0;
    float y_final,x_final;
    if(input==1){
        // left
        y_final = y_init - 0.1;
        for(int i=0;i<final_points;i+=6){
            float y1 = points[i+1];
            float y2 = points[i+4];
            float x1 = points[i];
            float x2 = points[i+3];
            float max_x = max(x1,x2);
            float min_x = min(x2,x1);
            // radius and length from maze.cpp
                if((x_init+0.3<=max_x && x_init+0.3>=min_x) || (x_init - 0.4 <= max_x && x_init - 0.4>=min_x) || (max_x<=x_init+0.3 && max_x>=x_init-0.4)){
                    // printf("yeah\n");
                    if(y_init-0.3 >= max(y1,y2) && y_final -0.3 <= max(y1,y2)){
                        // printf("%f,%f : %f,%f\n",x1,y1,x2,y2);
                        // printf("%f,%f\n",x_init,y_init);
                        // printf("yeah1\n");
                        return 1;
                    }        
                }
        }

    }
    else if(input==2){
        y_final = y_init + 0.1;
        for(int i=0;i<final_points;i+=6){
            float y1 = points[i+1];
            float y2 = points[i+4];
            float x1 = points[i];
            float x2 = points[i+3];
            float max_x = max(x1,x2);
            float min_x = min(x2,x1);
            // radius and length from maze.cpp
                if((x_init+0.3<=max_x && x_init+0.3>=min_x) || (x_init - 0.4 <= max_x && x_init - 0.4>=min_x) || (max_x<=x_init+0.3 && max_x>=x_init-0.4)){
                    if(y_init+0.3 <= min(y1,y2) && y_final +0.3 >= min(y1,y2)){
                        return 1;
                    }        
                }
        }
    }
    else if(input==3){
        x_final = x_init + 0.1;
        for(int i=0;i<final_points;i+=6){
            float y1 = points[i+1];
            float y2 = points[i+4];
            float x1 = points[i];
            float x2 = points[i+3];
            float max_y = max(y1,y2);
            float min_y = min(y2,y1);
            // radius and length from maze.cpp
                if((y_init+0.3<=max_y && y_init+0.3>=min_y) || (y_init - 0.3 <= max_y && y_init - 0.3>=min_y) || (max_y<=y_init+0.3 && max_y>=y_init-0.3)){
                    if(x_init+0.3 <= min(x1,x2) && x_final +0.3 >= min(x1,x2)){
                        return 1;
                    }        
                }
        }
    }
    else if(input==4){
        x_final = x_init - 0.1;
        for(int i=0;i<final_points;i+=6){
            float y1 = points[i+1];
            float y2 = points[i+4];
            float x1 = points[i];
            float x2 = points[i+3];
            float max_y = max(y1,y2);
            float min_y = min(y2,y1);
            // radius and length from maze.cpp
                if((y_init+0.3<=max_y && y_init+0.3>=min_y) || (y_init - 0.3 <= max_y && y_init - 0.3>=min_y) || (max_y<=y_init+0.3 && max_y>=y_init-0.3)){
                    if(x_init-0.4 >= max(x1,x2) && x_final -0.4 <= max(x1,x2)){
                        return 1;
                    }        
                }
        }
    }
    return 0;
}
int character_collide_with_imposter(){
    int imposter_x = imposter.position.x;
    int imposter_y = imposter.position.y;
    int player_x = character1.position.x;
    int player_y = character1.position.y;

    if(player_x==imposter_x && player_y==imposter_y){
        return 1;
    }
    else{
        return 0;
    }
}


void tick_elements() {
    // ball1.tick();
    maze1.tick();
    character1.tick();
    if(imposter_alive){
        imposter.tick();
        button1.tick();
    }
    if(object_flag){
        button2.tick();
    }
    if(display_final_button){
        final_button.tick();
    }
    if(!object_flag){
        for(int i=0;i<3;i++){
            if(bomb_objects_exist[i]){
                bomb_objects[i].tick();
            }
            if(powerup_objects_exist[i]){
                powerup_objects[i].tick();
            }
        }
    }
    if(imposter_alive){
        move_imposter_time+=1;
        float imp_x = imposter.position.x + 5.0;
        float imp_y = imposter.position.y + 5.0;
        if(move_imposter_time==50 || move_imposter==0){
            move_imposter = imposter_next_step();
            // printf("imposter is at : %f,%f and going to %d\n",imp_x,imp_y,move_imposter);
            if(move_imposter==1){
                imposter.position.y -=1;
            }
            else if(move_imposter==2){
                imposter.position.y +=1;
            }
            else if(move_imposter==3){
                imposter.position.x +=1;
            }
            else if(move_imposter==4){
                imposter.position.x -=1;
            }
            // printf("affter movement it is at : %f,%f\n",imposter.position.x+5.0,imposter.position.y+5.0);
            move_imposter_time = 0;
        }
        if(character_collide_with_imposter()){
            game_over_flag = 1;
        }
    }
    // camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // ball1       = Ball(0, 0, COLOR_RED);
    maze1 = Maze(0,0,COLOR_RED);
    character1 = Character(-4.5,-4.5,COLOR_RED);
    if(imposter_alive){
        imposter = Character( (int)(5*(((float)rand()/(float)(RAND_MAX)))) + 0.5, 0.5+ (int)(5*(((float)rand()/(float)(RAND_MAX)))), COLOR_GREEN);
        button1 = Button( -5 + (int)(10*(((float)rand()/(float)(RAND_MAX)))) + 0.5, -5 + 0.5+ (int)(10*(((float)rand()/(float)(RAND_MAX)))), COLOR_GREEN, 0);
    }
    button2 = Button( -5 + (int)(10*(((float)rand()/(float)(RAND_MAX)))) + 0.5, -5 + 0.5+ (int)(10*(((float)rand()/(float)(RAND_MAX)))), COLOR_RED, 1);
    for(int i=0;i<3;i++){
        bomb_objects[i] = Button( -5 + (int)(10*(((float)rand()/(float)(RAND_MAX)))) + 0.5, -5 + 0.5+ (int)(10*(((float)rand()/(float)(RAND_MAX)))), COLOR_BLACK, 2);
        powerup_objects[i] = Button( -5 + (int)(10*(((float)rand()/(float)(RAND_MAX)))) + 0.5, -5 + 0.5+ (int)(10*(((float)rand()/(float)(RAND_MAX)))), COLOR_PURPLE, 3);
    }
    final_button = Button(4.5, -4.5, COLOR_BLUE, 1);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    
    srand(time(0));
    int width = 600;
    int height = 600;
    window = initGLFW(width, height);
    initGL(window, width, height);
    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
    // Process timers
    if (t60.processTick())
    {
        // 60 fps
        // OpenGL Draw commands
        draw();
        // Initialize glText
        gltInit();
        int time_remaining = game_time - (int)(time(NULL) - game_start_time);
        if(time_remaining==0 && !game_won_flag){
            game_over_flag = 1;
        }
        // Creating text
        GLTtext *text = gltCreateText();
        char string[100];
        int tasks_left = 0;
        if(imposter_alive){
            tasks_left++;
        }
        if(object_flag){
            tasks_left++;
        }
        if(tasks_left==0){
            display_final_button = 1;
        }
        if(!game_over_flag && !game_won_flag){
            sprintf(string, "time remaining : %d\nscore : %d\ntasks : %d/2\n light: ON\n",time_remaining,game_score,tasks_left);
        }
        else if(game_won_flag){
            sprintf(string,"YOU WON\n");
        }
        else{
            sprintf(string,"GAME OVER");
        }
        gltSetText(text, string);


        // Begin text drawing (this for instance calls glUseProgram)
        gltBeginDraw();

        // Draw any amount of text between begin and end
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        if(!game_over_flag && !game_won_flag){
            gltDrawText2D(text, 1, 19, 3);
        }
        else{
            gltDrawText2D(text, 1, 19, 10);
        }

        // Finish drawing text
        gltEndDraw();

        // Deleting text
        gltDeleteText(text);

        // Destroy glText
        gltTerminate();
        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);
        tick_elements();
        tick_input(window);
    }
    // Poll for Keyboard and mouse events
    glfwPollEvents();
    }
    quit(window);
    
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
