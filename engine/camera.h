#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"
#include "transform.hpp"

using glm::mat4;
using glm::vec3;

class Camera {

private:
    vec3 position;
    vec3 viewDirection;
    vec3 up;


public:
    Camera();
    Camera(glm::vec3 newPos);

    mat4 getWorldToViewMatrix();

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
