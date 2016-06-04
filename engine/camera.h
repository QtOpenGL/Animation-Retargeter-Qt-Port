#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"
#include "transform.hpp"

class Camera {

private:
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec3 up;
    int mousex;
    int mousey;
    glm::vec2 oldMousePosition;



public:
    Camera();
    Camera(glm::vec3 newPos);

    glm::mat4 getWorldToViewMatrix();

    void moveForward();
    void moveBackward();
    void moveUp();
    void moveDown();
    void lookLeft();
    void lookRight();
    void strafeLeft();
    void strafeRight();
    void lookUp();
    void lookDown();

};

#endif // CAMERA_H
