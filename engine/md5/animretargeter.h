#ifndef ANIMRETARGETER_H
#define ANIMRETARGETER_H

#include <stdio.h>
#include <map>
#include <utility>
#include <string>
#include <memory>
#include "engine/entity.h"

using std::string;
using std::pair;
using std::map;
using std::shared_ptr;

class AnimRetargeter {

    void buildBindPose(Skeleton &bind);
    quaternion calcRotationBetweenVecs(vec3 & A, vec3 & B);
    bool inVector(int target, vector<int> vector);

public:
    Animation * retargetAnim( Animation * anim, AnimEntity * ent, map<int, int> * userMap, vector<int> roots, Skeleton bind);
};

#endif // ANIMRETARGETER_H
