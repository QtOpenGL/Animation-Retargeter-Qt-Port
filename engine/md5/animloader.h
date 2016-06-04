#ifndef ANIMLOADER_H
#define ANIMLOADER_H

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include "engine/model.h"
#include "utils/md5exception.h"

using std::map;
using std::pair;

class AnimLoader{
private:
    vector<frame> frames;
    Skeleton baseFrame;
    Animation* buildFrameSkeletons();

    void clearResources();

public:
    AnimLoader(){}
    ~AnimLoader(){}
    Animation* loadAnim(QString filePath);


};

#endif // ANIMLOADER_H
