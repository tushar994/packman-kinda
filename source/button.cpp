#include "button.h"
#include "main.h"

Button::Button(float x, float y, color_t color, int type) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->type = type;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float radius = 0.3;


    static GLfloat vertex_buffer_data[2000];
    static GLfloat vertex_buffer_data2[2000];
    static GLfloat vertex_buffer_data3[2000];

    int vertex_buffer_data_index = 0;
    int vertex_buffer_data_index2 = 0;
    int vertex_buffer_data_index3 = 0;
    float x_init = 5.0, y_init = 5.0;
    for (float i =0; i<6.28; i+=0.1){
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

    if(this->type==2){
        float arr1[] = {5.5,5.05,0, 5.5,4.95,0 ,5.3,4.95,0};
        for(int j=0;j<9;j++){
            vertex_buffer_data2[vertex_buffer_data_index2] = arr1[j];
            vertex_buffer_data_index2++;
        }
        float arr2[] = {5.5,5.05,0, 5.3,4.95,0 ,5.3,5.05,0};
        for(int j=0;j<9;j++){
            vertex_buffer_data2[vertex_buffer_data_index2] = arr2[j];
            vertex_buffer_data_index2++;
        }
    }
    else if(this->type==3){
        float arr1[] = { 5.15,4.95,0,  4.85,4.95,0,   4.85,4.85,0  };
        for(int j=0;j<9;j++){
            vertex_buffer_data3[vertex_buffer_data_index3] = arr1[j];
            vertex_buffer_data_index3++;
        }
        float arr2[] = {5.15,4.95,0,  5.15,4.85,0,   4.85,4.85,0};
        for(int j=0;j<9;j++){
            vertex_buffer_data3[vertex_buffer_data_index3] = arr2[j];
            vertex_buffer_data_index3++;
        }

        float arr3[] = { 5.15,5.05,0,  4.85,5.05,0,   4.85,5.15,0  };
        for(int j=0;j<9;j++){
            vertex_buffer_data3[vertex_buffer_data_index3] = arr3[j];
            vertex_buffer_data_index3++;
        }
        float arr4[] = {5.15,5.05,0,  5.15,5.15,0,   4.85,5.15,0};
        for(int j=0;j<9;j++){
            vertex_buffer_data3[vertex_buffer_data_index3] = arr4[j];
            vertex_buffer_data_index3++;
        }
    }


    this->object[0] = create3DObject(GL_TRIANGLES, vertex_buffer_data_index, vertex_buffer_data, color, GL_FILL);
    if(this->type==2 ){
        this->object[1] = create3DObject(GL_TRIANGLES, vertex_buffer_data_index2, vertex_buffer_data2, COLOR_ORANGE, GL_FILL);
    }
    if(this->type==3){
        this->object[2] = create3DObject(GL_TRIANGLES, vertex_buffer_data_index3, vertex_buffer_data3, COLOR_ORANGE, GL_FILL);
    }
}

void Button::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object[0]);
    if(this->type==2 ){
        draw3DObject(this->object[1]);
    }
    if(this->type==3){
        draw3DObject(this->object[2]);
    }
}

void Button::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Button::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

