#include "character.h"
#include "main.h"

Character::Character(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float radius = 0.3;
    float length = 0.2;
    float glass_length = 0.1;
    float glass_breadth  = 0.2;


    static GLfloat vertex_buffer_data[1000];

    int vertex_buffer_data_index = 0;
    float x_init = 5, y_init = 5;
    for (float i =0; i<=3.14; i+=0.1){
        float x1 = sin(i)*radius + x_init;
        float x2 = sin(i+0.1)*radius + x_init;
        float y1 = cos(i)*radius + y_init;
        float y2 = cos(i+0.1)*radius + y_init;
        float arr[] = {x1,y1,0,x2,y2,0,x_init,y_init,0};
        for(int j=0;j<9;j++){
            vertex_buffer_data[vertex_buffer_data_index] = arr[j];
            vertex_buffer_data_index++;
        }
    }
    float tri1[] = {x_init , y_init- radius, 0, x_init , y_init+ radius, 0 , x_init - length, y_init - radius, 0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri1[i];
        vertex_buffer_data_index++;
    }
    float tri2[] = {x_init - length , y_init- radius, 0, x_init - length , y_init+ radius, 0 , x_init, y_init + radius, 0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri2[i];
        vertex_buffer_data_index++;
    }

    // float tri3[] = {4.7,4.8,0,  4.6,4.8,0,  4.6,4.6,0};
    // float tri3[] = {4.8,4.7,0,  4.8,4.6,0,  4.6,4.6,0};
    float tri3[] = {4.8,4.9,0,  4.8,4.7,0,  4.6,4.7,0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri3[i];
        vertex_buffer_data_index++;
    }
    float tri4[] = {4.8,4.9,0,  4.6,4.9,0,  4.6,4.7,0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri4[i];
        vertex_buffer_data_index++;
    }

    float tri5[] = {4.8,5.1,0,  4.8,5.3,0,  4.6,5.3,0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri5[i];
        vertex_buffer_data_index++;
    }
    float tri6[] = {4.8,5.1,0,  4.6,5.1,0,  4.6,5.3,0};
    for(int i=0;i<9;i++){
        vertex_buffer_data[vertex_buffer_data_index] = tri6[i];
        vertex_buffer_data_index++;
    }


    this->object[0] = create3DObject(GL_TRIANGLES, vertex_buffer_data_index, vertex_buffer_data, color, GL_FILL);
    static GLfloat glasses[] = {
        x_init, y_init - glass_breadth, 0,
        x_init, y_init + glass_breadth, 0,
        x_init - glass_length, y_init - glass_breadth, 0,

        x_init - glass_length, y_init - glass_breadth, 0,
        x_init - glass_length, y_init + glass_breadth, 0,
        x_init, y_init + glass_breadth, 0,
    };
    this->object[1] = create3DObject(GL_TRIANGLES, 2*3, glasses, COLOR_GREY, GL_FILL);
}

void Character::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object[0]);
    draw3DObject(this->object[1]);
}

void Character::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Character::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

