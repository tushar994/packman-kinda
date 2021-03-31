#include "main.h"

#ifndef CHARACTER_H
#define CHARACTER_H


class Character {
public:
    Character() {}
    Character(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object[2];
};
extern Character character1;
extern Character imposter;

#endif // BALL_H
