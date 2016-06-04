#include "model.h"

/*
 * Model
 */
Model::Model( vector<GLuint> nIndices, vector<float> nTexture ) : inBuffer(false), texture(nTexture), indices(nIndices) {}

Model::Model(const Model & m) : inBuffer(m.inBuffer), indices(m.indices), texture(m.texture){}

 Model::~Model(){}

void Model::setBufferIDs (GLuint newIB, GLuint newTB){
    inBuffer = true;
    IB = newIB;
    TB = newTB;
}

/*
 * AnimModel
 */
AnimModel::AnimModel( string nName, vector<GLuint> nIndices, vector<float> nTexture, vector<vert> nVertices, vector<weight> nWeights ) : name(nName), weights(nWeights), vertices(nVertices), Model(nIndices, nTexture){}

AnimModel::AnimModel(const AnimModel & m): name(m.name), Model(m){}

AnimModel::~AnimModel(){}

vector<float> AnimModel::computeVerts(Skeleton & skel){

    vector<float> glverts;

    for(int vertIndex = 0; vertIndex < vertices.size(); vertIndex++){

        vec3 finalPos(0, 0, 0);

        //iterate through each weight associated with this vert.
        for(int weightIndex = 0; weightIndex < vertices[vertIndex].getWeightCount(); ++weightIndex){

            weight currentW = weights[vertices[vertIndex].getStartWeight() + weightIndex];

            joint & temp = skel[currentW.getJoint()];

            vec3 rotatedPos = temp.getObjectOrient().rotateVec(currentW.getPos());

            //sum all the weights multiplied by their bias
            finalPos += (temp.getObjectPos() + rotatedPos) * currentW.getBias();


        }

        glverts.push_back(finalPos.x);
        glverts.push_back(finalPos.y);
        glverts.push_back(finalPos.z);
    }

    return glverts;

}

/*
 * Anim
 */
Animation::Animation(const Animation & a): name(a.name), frameRate(a.frameRate), Skeletons(a.Skeletons) {}


/*
 * Skeleton
 */
void Skeleton::recomputeObjectSpace(){

    for(int i = 0; i < joints.size(); i++){

        int parentIndex = joints[i].getParent();

        //does the joint have a parent?
        if(parentIndex > -1){

            //every joint is computed in order.
            joint* parent = &joints[parentIndex];

            //compute the object space pos of the joint.
            vec3 pos = parent->getObjectOrient().rotateVec(joints[i].getLocalPos());
            joints[i].setObjectPos(pos + parent->getObjectPos());

            //compute the object space orientation of the joint.
            joints[i].setObjectOrient(parent->getObjectOrient() * joints[i].getLocalOrient());
            //joints[i].getObjectOrient().normalize();

        }else{
            joints[i].setObjectPos(joints[i].getLocalPos());
            joints[i].setObjectOrient(joints[i].getLocalOrient());
        }


    }

}

void Skeleton::printJoints(){
    for(int i = 0; i < joints.size(); i++){
        std::cout << i << ": " << joints[i].getObjectOrient().toString() << joints[i].getObjectPos().x << " " << joints[i].getObjectPos().y << " " << joints[i].getObjectPos().z << std::endl;
    }
}


void Skeleton::computeHeight(){

    for(int i = 0; i < joints.size(); i++){
        compAux(joints[i].getParent(), 1);
    }

    for(int i = 0; i < joints.size(); i++){

        if(joints[i].getHeight() >= 4)
            heights.push_back(i);
    }
}

void Skeleton::compAux(int parent, int height){

    if(parent > -1 && joints[parent].getHeight() < height){
        joints[parent].setHeight(height);
        //cout << joints[parent].getName() << " height: " << joints[parent].getHeight() << endl;
        compAux(joints[parent].getParent(), height + 1);
    }

}
