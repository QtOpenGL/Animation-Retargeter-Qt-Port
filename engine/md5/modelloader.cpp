#include "ModelLoader.h"

//
//  ModelLoader.cpp
//  DrawEngine
//
// Based on the work by David Henry
// http://tfc.duke.free.fr/coding/md5-specs-en.html
//

#include "ModelLoader.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ios;

ModelLoader::ModelLoader() : numJoints(0), numMeshes(0){}

AnimEntity * ModelLoader::loadModel(QString filePath){

    string fpath = filePath.toStdString();

    //set up the file loader
    ifstream infile;
    infile.open(fpath.c_str(), ios::in);

    if(infile.fail()){
        return NULL;
    }

    vector<AnimSubEntity> SubEntities;
    Skeleton bindPose;

    //parse the file into the data structures
    while(!infile.eof()){
        string token, buffer;

        infile >> token;

        if(token == "//" || token == "MD5Version")
            std::getline(infile, buffer);

        else if (token == "numJoints"){
            infile >> numJoints;
        }

        else if (token == "numMeshes"){
            infile >> numMeshes;
        }

        else if (token == "joints"){
            infile >> token; //{

            bindPose = populateJoints(infile);

            infile >> token; //}
        }

        else if (token == "mesh"){

            infile >> token; //{

            SubEntities.push_back(AnimSubEntity(populateMesh(infile)));

        }

    }

    infile.close();

    AnimEntity * pointer = new AnimEntity(SubEntities, bindPose);

    return pointer;

}


Skeleton ModelLoader::populateJoints(ifstream &infile){

    string token, buffer;

    Skeleton bindPose;

    if(!infile.is_open())
        cerr << "Inputstream is not open!" << endl;

    for(int i = 0; i < numJoints; i++){
        string name;
        int parent;
        float pX, pY, pZ;
        float oX, oY, oZ;


        infile >> name;
        infile >> parent;
        infile >> token; //(

        //position.
        infile >> pX;
        infile >> pY;
        infile >> pZ;

        infile >> token; //)
        infile >> token; //(

        infile >> oX;
        infile >> oY;
        infile >> oZ;

        infile >> token; //)


        getline(infile, buffer);

        joint temp(name, parent, vec3(pX, pY, pZ), quaternion(oX, oY, oZ) );
        temp.getLocalOrient().computeW();
        temp.setObjectOrient(temp.getLocalOrient());
        temp.setObjectPos(temp.getLocalPos());

        bindPose.push_back(temp);

        if(parent > -1)
            bindPose[i].setParentPtr(&bindPose[parent]);

    }

    bindPose.computeHeight();

    return bindPose;
}

AnimModel * ModelLoader::populateMesh(ifstream &infile){
    string token, buffer;

    string name;
    vector<vert> vertices;
    vector<GLuint> indices;
    vector<weight> weights;
    vector<float> texture;

    if(!infile.is_open())
        cerr << "inputstream not open" << endl;

    do {
        infile >> token;


        if(token == "//"){
            infile >> buffer; //meshes:
            infile >> name;
        }


        else if(token == "numverts"){
            int numverts;
            infile >> numverts;
            vertices.reserve(numverts);
            texture.reserve(numverts * 2);
        }

        else if(token == "numtris"){
            int numtris;
            infile >> numtris;
            indices.reserve(numtris * 3);
        }

        else if(token == "numweights"){
            int numweights;
            infile >> numweights;
            weights.reserve(numweights);
        }

        else if(token == "vert"){
            int index, cWeight, sWeight;
            float coord;

            infile >> index; //index
            infile >> token; //(
            infile >> coord;
            texture.push_back(coord);
            infile >> coord;
            texture.push_back(-coord);
            infile >> token; //)
            infile >> sWeight;
            infile >> cWeight;

            vertices.push_back(vert(index, cWeight, sWeight));
        }

        else if(token == "tri"){
            int index;


            infile >> buffer; //index
            infile >> index; //vertIndex
            indices.push_back(index);
            infile >> index;
            indices.push_back(index);
            infile >> index;
            indices.push_back(index);


        }

        else if(token == "weight"){
            int index, joint;
            float pX, pY, pZ, bias;

            infile >> index; //index
            infile >> joint;

            infile >> bias;
            infile >> token; //(

            infile >> pX;
            infile >> pY;
            infile >> pZ;

            infile >> token; //)

            weights.push_back(weight(index, joint, bias, vec3(pX, pY, pZ)));
        }

        //skip the rest of the line
        getline(infile, buffer);

    } while((token != "}") && !infile.eof());

    AnimModel * temp = new AnimModel(name, indices, texture, vertices, weights);

    return temp;
}
