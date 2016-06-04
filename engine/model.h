#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <QOpenGLFunctions>
#include <vector>
#include <string.h>
#include <memory.h>
#include <map>
#include "md5/structs.h"

using std::string;
using std::vector;
//using std::shared_ptr;
using std::map;

class Skeleton {
private:
    vector<joint> joints;
    vector<int> heights;

   void compAux(int parent, int height);

public:
    Skeleton() {};
    Skeleton(vector<joint> nJoints): joints(nJoints){}
    Skeleton(const Skeleton & skel): joints(skel.joints){}
    ~Skeleton(){}
    joint & operator[](const int i) const { return (joint &) joints[i]; }
    void push_back(const joint & data){ joints.push_back(data); }
    int size() const { return (int) joints.size(); }
    void reserve(int n){ joints.reserve(n); }
    const joint * data() const { return joints.data(); }
    void copy_data( const Skeleton & op ){ this->joints = op.joints; }
    void clear(){ joints.clear(); }
    void recomputeObjectSpace();
    void computeHeight();
    vector<int> & getHeights(){ return heights; }
    void printJoints();
};

class Model {
private:
    vector<GLuint> indices;
    vector<float> texture;
    bool inBuffer; //is currently in VRAM buffer.
    GLuint IB, TB;

public:
    Model( vector<GLuint> nIndices, vector<float> nTexture);
    Model(const Model & m);
    virtual ~Model();
    void setBufferIDs (GLuint newIB, GLuint newTB);
    void outOfBuffer() { inBuffer = false; };

    const vector<GLuint> getIndices() { return indices; };
    const vector<float> getTexture() { return texture; };

    GLuint getIB() { return IB; };
    GLuint getTB() { return TB; };
    bool isInBuffer() { return inBuffer; };

};


class AnimModel : public Model {
private:
    string name;
    vector<vert> vertices;
    vector<weight> weights;

public:
    AnimModel( string nName, vector<GLuint> nIndices, vector<float> nTexture, vector<vert> nVertices, vector<weight> nWeights );
    AnimModel(const AnimModel & m);
    ~AnimModel();
    vector<float> computeVerts(Skeleton & skel);
    vector<weight> getWeights(){ return weights; }
};

class Animation {
private:
    string name;
    int frameRate;
    vector<Skeleton> Skeletons;

public:
    Animation( string nName, int nFrameRate): name(nName), frameRate(nFrameRate) {}
    Animation(): name("Default"), frameRate(30){};
    Animation(const Animation & a);
    ~Animation(){}
    vector<Skeleton> & getSkeletons() { return Skeletons; };
    int getNumFrames(){ return (int)Skeletons.size() -1; };
    int getFrameRate(){return frameRate; };
    void setFrameRate(int rate){ this->frameRate = rate; };
    void setSkeletons(vector<Skeleton> skels){ Skeletons = skels; };
};



#endif // MODEL_H
