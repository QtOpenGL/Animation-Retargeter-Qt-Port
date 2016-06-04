#ifndef QUATERNION_H
#define QUATERNION_H

#include<iostream>
#include<math.h>
#include<string>
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

using std::string;
using std::to_string;
using glm::vec3;
using glm::vec4;
using glm::mat4;


/*===============================
 * Quaternion
 *===============================*/

class quaternion{

private:
    float x, y, z, w;

public:
    quaternion(){ x = y = z = w = 0;}
    quaternion(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; };
    quaternion(float x, float y, float z) { this->x = x; this->y = y; this->z = z; this->w = 0; };
    quaternion( vec3 axis, float angle );

    void normalize();
    void computeW();
    quaternion operator*(const quaternion &op);
    void operator=(const quaternion &op);
    vec3 rotateVec(const vec3 &vector);
    vec4 rotateVec(const vec4 &vector);
    mat4 toMatrix();

    //setters and getters
    inline void setX(float newX) {this->x = newX; };
    inline void setY(float newY) {this->y = newY; };
    inline void setZ(float newZ) {this->z = newZ; };
    inline void setW(float newW) {this->w = newW; };

    inline float getX() const {return this->x; };
    inline float getY() const {return this->y; };
    inline float getZ() const {return this->z; };
    inline float getW() const {return this->w; };
    float getMagnitude() const { return sqrt( (x * x) + (y * y) + (z * z) + (w * w) ); };
    quaternion getConjugate() const { return quaternion(-x, -y, -z, w); };
    void becomeIdentity(){ w = 1, x = 0; y = 0; z = 0; }


    string toString(){ return "( " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + " )"; }

};


#endif // QUATERNION_H
