#include "quaternion.h"


/*===============================
 * Quaternion
 ================================*/

/*
 * Quaternion
 * constructor given an axis and angle.
 * @param axis - vector for the axis of orientation.
 * @param angle - the angle of orientation in degrees.
 */

quaternion::quaternion(vec3 axis, float angle){

    //degrees to radians.
    float theta = (M_PI / 180.0f) * angle;

    setW(cos(theta/2));
    setX(axis.x * sin(theta/2));
    setY(axis.y * sin(theta/2));
    setZ(axis.z * sin(theta/2));

}

/*
 * Quaternion Multiplication
 * multiplies the operand quaternion by this quaternion.
 * @param op - a quaternion.
 * @return quaternion - a concatenation of the two quaternions.
 */
quaternion quaternion::operator*(const quaternion &op){
    quaternion result;

    result.setW( (getW() * op.getW()) - (getX() * op.getX()) - (getY() * op.getY()) - (getZ() * op.getZ()) );
    result.setX( (getW() * op.getX()) + (getX() * op.getW()) + (getY() * op.getZ()) - (getZ() * op.getY()) );
    result.setY( (getW() * op.getY()) + (getY() * op.getW()) + (getZ() * op.getX()) - (getX() * op.getZ()) );
    result.setZ( (getW() * op.getZ()) + (getZ() * op.getW()) + (getX() * op.getY()) - (getY() * op.getX()) );

    return result;
}

/*
 * computeW()
 * computes the w component of a unit quaternion.
 */
void quaternion::computeW() {

    /*not using getMagnitude() because you may want to recalculate w */
    float newW = 1.0f - ((getX() * getX()) + (getY() * getY()) + (getZ() * getZ()));

    if(newW < 0)
        setW(0);
    else
        setW(-sqrt(newW));

}

/*
 * Normalize()
 * normalizes the quaternion, creating a unit quaternion.
 */
void quaternion::normalize(){
    float mag = getMagnitude();

    if(mag > 0.0f){
        setX(getX() / mag);
        setY(getY() / mag);
        setZ(getZ() / mag);
        setW(getW() / mag);
    }
}

/*
 * rotateVec(vec3)
 * rotates a 3-D vector by the axis-angle rotation encoded in the quaternion.
 * @param vec - the vector to be rotated.
 * @return vec3 - the rotated vector.
 */
vec3 quaternion::rotateVec(const vec3 &vec){

    quaternion point(vec.x, vec.y, vec.z);

    quaternion inv = getConjugate(); //for unit quaternions the conjugate and inverse a equivalent.

    inv.normalize();

    quaternion result = *this * point * inv;

    return vec3(result.getX(), result.getY(), result.getZ());
}


/*
 * rotateVec(vec4)
 * rotates a 4-D vector by the axis-angle rotation encoded in the quaternion.
 * @param vec - the vector to be rotated.
 * @return vec4 - the rotated vector.
 */
vec4 quaternion::rotateVec(const vec4 &vec){

    quaternion point(vec.x, vec.y, vec.z);

    quaternion inv = getConjugate(); //for unit quaternions the conjugate and inverse a equivalent.

    quaternion result = *this * point * inv;

    return vec4(result.x, result.y, result.z, 0);
}


mat4 quaternion::toMatrix(){
    float matrix[16];

    matrix[0] = 1 - (2*y*y) - (2*z*z);
    matrix[1] = (2*x*y) - (2*w*z);
    matrix[2] = (2*x*z) + (2*w*y);
    matrix[3] = 0;
    //----------------------
    matrix[4] = (2*x*y) + (2*w*z);
    matrix[5] = 1 - (2*x*x) - (2*z*z);
    matrix[6] = (2*y*z) - (2*w*x);
    matrix[7] = 0;
    //----------------------
    matrix[8] = (2*x*z) - (2*w*y);
    matrix[9] = (2*y*z) + (2*w*x);
    matrix[10] = 1 - (2*x*x) - (2*y*y);
    matrix[11] = 0;
    //---------------------
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;

    //column wise.
    return glm::make_mat4(matrix);
}


void quaternion::operator=(const quaternion &op){

    setX(op.getX());
    setY(op.getY());
    setZ(op.getZ());
    setW(op.getW());

}
