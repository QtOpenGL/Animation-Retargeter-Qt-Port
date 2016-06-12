#ifndef CAMERA_H
#define CAMERA_H

//#include "glm.hpp"
//#include "transform.hpp"
#include <QVector3D>
#include <QMatrix4x4>
#include <QMatrix3x3>

class Camera {

private:
    QVector3D position;
    QVector3D viewDirection;
    QVector3D up;
    QVector3D oldMousePosition;


public:
    Camera();
    Camera(QVector3D newPos);

    QMatrix4x4 getWorldToViewMatrix();

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
