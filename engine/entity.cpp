//
//  Entity.cpp
//  DrawEngine
//
//  Created by Alex Handby on 15/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#include "entity.h"

Entity::Entity(): visible(true){}

Entity::~Entity(){
}

/*
 * rotate
 * rotates the entity around a designated axis by a number of degrees.
 * @param float degrees - degrees
 * @param axis - axis
 */
void Entity::rotate(qreal degrees, QVector3D axis){
    orient.rotate(degrees, axis);
    //orient = glm::rotate(orient, degrees, axis);
}

/*
 * translate
 * moves an entity by a described translation
 * @param vec3 - a vector displacement.
 */
void Entity::translate(QVector3D trans){
    pos.translate(trans);
    //pos = glm::translate(pos, trans);
}

AnimSubEntity::AnimSubEntity(){}
AnimSubEntity::AnimSubEntity(AnimModel * newModel) : m_model(newModel){}
AnimSubEntity::~AnimSubEntity(){}

/*
 * AnimSubEntity Copy Constructor
 * @param AnimSubEntity & a
 */
AnimSubEntity::AnimSubEntity(const AnimSubEntity & a): m_model(a.m_model), VB(a.VB), Entity(a), vertices(a.vertices){}

/*
 * AnimEntity Constructor
 * @param vector SubEntities - sub-entities
 * @param Skeleton bindpose - the bindpose skeleton for this animated entity
 */
AnimEntity::AnimEntity(vector<AnimSubEntity> nSubEntities, Skeleton nbindPose) : fChanged(false), frameCount(0), anim(NULL), SubEntities(nSubEntities), bindPose(nbindPose), state(BIND), updateMillis(0)
{
    rebufVerts();
}


/*
 * AnimEntity Copy Constructor
 * @param AnimEntity & a
 */
AnimEntity::AnimEntity(const AnimEntity & a): anim(a.anim), bindPose(a.bindPose), fChanged(a.fChanged), frameCount(a.frameCount), SubEntities(a.SubEntities), state(BIND), updateMillis(0)
{
}

/*
 * BindPose
 * put the AnimEntity in BindPose
 */
void AnimEntity::BindPose(){

    state = BIND;
    rebufVerts();

}

/*
 * Play
 * set the AnimEntity to Play
 */
void AnimEntity::Play(){

    if(anim == NULL)
        return;

    state = PLAY;
    frameCount = 0;
    rebufVerts();

}

/*
 * Pause
 * set the AnimEntity to Pause
 */
void AnimEntity::Pause(){

    if(anim  == NULL)
        return;

    state = PAUSE;
    frameCount = 0;
    rebufVerts();
}

/*
 * update
 * buffers the next frame of the animation
 * @param double currentTime - used to ensure the animation is drawn at the desired frameRate.
 */
void AnimEntity::update(){

    updateMillis += 16;

    if(anim && updateMillis >= anim->getMillisPerFrame()){
        updateMillis = 0;

        if(state == PLAY){
            frameCount++;
            frameCount %= anim->getNumFrames();
            rebufVerts();
        }
    }

}

/*
 * setAnim
 * set the animation
 * @param Anim * nAnim - pointer to the animation.
 */
bool AnimEntity::setAnim(Animation * nAnim){
    this->anim = nAnim;
    Pause();
    return true;
}

/*
 * rebufVerts
 * recompute and buffer the vertices of all the AnimSubEntities.
 */
void AnimEntity::rebufVerts(){

    if(state == BIND){
        for(int i = 0; i < SubEntities.size(); i++)
            SubEntities[i].rebufVerts(bindPose);
    }else{
        for(int i = 0; i < SubEntities.size(); i++)
            SubEntities[i].rebufVerts(anim->getSkeletons()[frameCount]);

    }

    fChanged = true;
}

