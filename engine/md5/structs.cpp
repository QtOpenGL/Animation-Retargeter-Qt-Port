#include "structs.h"

/*
 * Vert
 */

vert::vert( int vIndex, int cWeight, int sWeight ){
    this->vertIndex = vIndex;
    this->countWeight = cWeight;
    this->startWeight = sWeight;
}


/*
 * Weight
 */
weight::weight(int wIndex, int njoint, float nbias, vec3 npos){
    this->weightIndex = wIndex;
    this->joint = njoint;
    this->bias = nbias;
    this->pos = npos;
}

/*
 * Joint
 */
joint::joint(string nName, int nParent, vec3 nPos, quaternion nOrient): objectPos(0, 0, 0), objectOrient(0, 0, 0), parentptr(NULL), height(0){
    this->name = nName;
    this->parent = nParent;
    this->localPos = nPos;
    this->localOrient = nOrient;
}



/*
 * Frame
 */
frame::frame(int nIndex, vector<float> cVals): frameIndex(nIndex), componentVals(cVals){}

frame::frame(const frame & f): frameIndex(f.frameIndex), componentVals(f.componentVals){}
