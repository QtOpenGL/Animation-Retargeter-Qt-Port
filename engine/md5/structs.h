#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <string>
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "utils/quaternion.h"

using glm::vec3;
using glm::mat4;
using std::vector;
using std::string;
using std::to_string;



/* Data Structures used for the loading of animations*/

/*
 * Vert
 */
class vert{
private:
    int vertIndex, countWeight, startWeight;

public:
    vert( int vIndex, int cWeight, int sWeight );
    int getIndex() const { return vertIndex; };
    int getWeightCount() const { return countWeight; };
    int getStartWeight() const { return startWeight; };

};


/*
 * Weight
 */
class weight {
private:
    int weightIndex, joint;
    float bias;
    vec3 pos;

public:
    weight(int wIndex, int njoint, float nbias, vec3 npos);

    int getIndex() const { return weightIndex; };
    int getJoint() const { return joint; };
    float getBias() const { return bias; };
    vec3 getPos() const { return pos; };

};

/*
 * Joint
 */
class joint{
private:
    string name;
    int parent, flags, startIndex, height;
    vec3 localPos;
    quaternion localOrient;
    vec3 objectPos;
    quaternion objectOrient;
    joint * parentptr;
    bool computed;

public:
    joint(): name(""), parent(-1), flags(0), startIndex(0), localPos(0,0,0), localOrient(0,0,0), objectPos(0,0,0), objectOrient(0,0,0), parentptr(NULL), height(0){};
    joint(string nName, int nParent, vec3 nPos, quaternion nOrient);
    ~joint(){}

    string getName() const{ return name; };
    int getParent() const { return parent; };
    vec3 getLocalPos() const { return localPos; };
    quaternion & getLocalOrient() { return localOrient; };
    vec3 getObjectPos() const { return objectPos; };
    quaternion & getObjectOrient() { return objectOrient; };
    int getFlags() const { return flags; };
    int getStartIndex() const { return startIndex; };
    joint * getParentPtr(){ return parentptr; }
    int getHeight(){ return height; }
    bool beenComputed()const{ return computed; }


    void setFlags(int nFlags){ this->flags = nFlags; };
    void setStartIndex(int nStartIndex) { this->startIndex = nStartIndex; };
    void setName(string nName){ this->name = nName; };
    void setParent(int nParent){ this->parent = nParent; };
    void setLocalPos(vec3 nPos){ this->localPos = nPos; };
    void setLocalOrient(quaternion nOrient){ this->localOrient = nOrient; };
    void setObjectPos(vec3 nPos){ this->objectPos = nPos; };
    void setObjectOrient(quaternion nOrient){ this->objectOrient = nOrient; };
    void setParentPtr(joint * parent){ this->parentptr = parent; }
    void setHeight(int n){ this->height = n; }
    void setComputed(bool setting){ this->computed = setting; }

};

/*
 * Frame
 */
class frame{
private:
    int frameIndex;
    vector<float> componentVals;

public:
    frame(int nIndex,vector<float> nCvals);
    frame(const frame & f);
    ~frame(){ }

    int getFrameIndex() const { return frameIndex; }
    vector<float> & getComponentVals() { return componentVals; }

};


#endif // STRUCTS_H
