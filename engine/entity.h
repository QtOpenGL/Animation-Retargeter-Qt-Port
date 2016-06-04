#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "transform.hpp"
#include "model.h"

using std::vector;
using std::string;
using std::shared_ptr;
using glm::mat4;
using glm::vec3;
using std::map;


/*===============================
 * Entity
 * @description: the baseclass of a scene entity.
 *===============================*/

class Entity{

private:
    vec3 color;
    mat4 pos;
    mat4 orient;
    GLuint texBID;


public:
    Entity();
    virtual ~Entity();
    void setPos(mat4 newPos){ this->pos = newPos; }
    void setOrient(mat4 newOrient){ this->orient = newOrient; }
    void setColor(vec3 newColor) { this->color = newColor; }
    void setTexBID(GLuint newID){ this->texBID = newID; }
    void rotate(float degrees, vec3 axis);
    void translate(vec3 trans);

    mat4 getPos(){ return pos; }
    mat4 getOrient(){ return orient; }
    GLuint getTexBID(){ return texBID; }
    vec3 getColor() { return color; }

};

/*===============================
 * AnimSubEntity
 * @description: An animated sub-entity.
 *===============================*/
class AnimSubEntity : public Entity {

private:
    AnimModel * m_model;
    vector<float> vertices;
    GLuint VB;
    bool visible;

public:
    AnimSubEntity();
    AnimSubEntity(AnimModel * newModel);
    AnimSubEntity(const AnimSubEntity & a);
    ~AnimSubEntity();

    void setModel(AnimModel * newModel){ this->m_model = newModel; }
    AnimModel * getModel(){return m_model;}
    void rebufVerts(Skeleton & skel) { this->vertices = m_model->computeVerts(skel); }
    vector<float> getVertices() { return vertices; }
    void setBufferIDs(GLuint nVB){ VB = nVB; }
    void setVisible(bool setting){ visible = setting; }
    bool isVisible(){ return visible; }
    GLuint getVB(){ return VB; }
};


/*
 * State
 * Describes the state that an animated entity would be in.
 */
enum State{ BIND, PAUSE, PLAY };


/*===============================
 * AnimEntity
 * @description: a animated Entity.
 *===============================*/
class AnimEntity : public Entity {

private:
    vector<AnimSubEntity> SubEntities;
    Animation * anim;
    Skeleton bindPose;
    bool fChanged;
    int frameCount;
    double lastTime;
    float deltaTime;
    State state;

public:
    AnimEntity(vector<AnimSubEntity> nSubEntities, Skeleton bindPose);
    AnimEntity(const AnimEntity & a);
    ~AnimEntity(){}
    bool setAnim(Animation * nAnim);
    Animation * getAnim(){ return anim; }
    vector<AnimSubEntity> & getSubEntities(){ return SubEntities; }
    Skeleton & getBindPose(){return bindPose;}
    bool frameChanged(){ return fChanged; }
    void changedOff(){ fChanged = false; }
    void rebufVerts();
    void update(double time);

    void BindPose();
    void Pause();
    void Play();

    bool isPlaying(){return state == PLAY; }
    bool isPaused(){ return state == PAUSE; }
    bool isBindPose(){ return state == BIND; }


};


#endif // ENTITY_H
