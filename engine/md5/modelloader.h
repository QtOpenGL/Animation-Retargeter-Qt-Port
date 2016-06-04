#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <fstream>
#include <utility>
#include "structs.h"
#include "engine/model.h"
#include "engine/entity.h"
#include "utils/md5exception.h"

using std::ifstream;
using std::pair;

class ModelLoader{
private:
    int numJoints, numMeshes;

    void buildFrameSkeletons();
    Skeleton populateJoints(ifstream &infile);
    AnimModel * populateMesh(ifstream &infile);

public:
    ModelLoader();
    AnimEntity * loadModel(QString filePath);

};

#endif // MODELLOADER_H
