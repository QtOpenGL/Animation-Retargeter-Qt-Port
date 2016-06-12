#include "engine.h"
#include <iostream>
#include <QString>


Engine::Engine(QObject * parent): QObject(parent), indexBuffer(QOpenGLBuffer::IndexBuffer), toEntity(NULL), fromEntity(NULL), animation(NULL)
{
     camera = new Camera(QVector3D(0, 0, 10));
     modelLoader = new ModelLoader();
     animLoader = new AnimLoader();
     animRetargeter = new AnimRetargeter();
}

void Engine::onPlayFrom(){

    if(fromEntity->isPaused()){
        fromEntity->Play(); //if in bindpose will have no effect.
    }else{
        fromEntity->Pause();
    }
}

void Engine::onPlayTo(){
    if(toEntity->isPaused()){
        toEntity->Play(); //if in bindpose will have no effect.
    }else{
        toEntity->Pause();
    }
}

void Engine::onBindFrom(){
    if(fromEntity->isBindPose()){
        fromEntity->Pause();
    }else{
        fromEntity->BindPose();
    }
}

void Engine::onBindTo(){
    if(toEntity->isBindPose()){
        toEntity->Pause();
    }else{
        toEntity->BindPose();
    }
}

void Engine::onHideFrom(){
    if(fromEntity->isVisible()){
        fromEntity->Pause(); //don't want to run an animation whilst hidden.
        fromEntity->setVisible(false);
    }else{
        fromEntity->setVisible(true);
    }
}

void Engine::onHideTo(){
    if(toEntity->isVisible()){
        toEntity->Pause(); //don't want to run an animation whilst hidden.
        toEntity->setVisible(false);
    }else{
        toEntity->setVisible(true);
    }
}


QVector<Skeleton> Engine::getSkeletonsForWindow(){

    QVector<Skeleton> skeletons;

    //in order to return skeletons we need: a toEntity, a fromEntity and a compatible animation.
    if(toEntity && animation && fromEntity && fromEntity->setAnim(animation)){
        skeletons.push_back(toEntity->getBindPose());
        skeletons.push_back(fromEntity->getBindPose());
    }

    return skeletons;
}

ProcedureResult Engine::retargetAnimation(map<int, int> boneMap, vector<int> roots){
    Animation * retargetAnimation;

    retargetAnimation = animRetargeter->retargetAnim(animation, toEntity, boneMap, roots, fromEntity->getBindPose());

    if(!retargetAnimation){
        return ProcedureResult(false, "Retargeting failed.");
    }else if(retargetAnimation->getSkeletons().empty()){
        return ProcedureResult(false, "Retargeting failed.");
    }else{

        if(!toEntity->setAnim(animation)){
            return ProcedureResult(false, "Retargeting failed.");
        }else{
            toEntity->Pause(); //this should set the first frame of animation
            return ProcedureResult(false, "Retargeting was a Success");
        }

    }


}

ProcedureResult Engine::LoadAnimation(QString animString){
    Animation * prevAnim = animation;

    animation = animLoader->loadAnim(animString);

    if(!animation){
        animation = prevAnim;
        return ProcedureResult(false, "File path incorrect");
    }else if(animation->getSkeletons().empty()){
        animation = prevAnim;
        return ProcedureResult(false, "File corrupted");
    }else{

        if(!fromEntity->setAnim(animation)){
            animation = prevAnim;
            return ProcedureResult(false, "The chosen animation is incompatible with the current 'From Model' please select a compatible animation or different 'From Model'");
        }else{
            fromEntity->Pause();
            delete prevAnim;
            return ProcedureResult(true, "");
        }

    }

}


ProcedureResult Engine::LoadModel(QString meshString, QString texString, EntityType type){

    if(type == FROM){
        AnimEntity * prevEntity = fromEntity;

        fromEntity = modelLoader->loadModel(meshString);

        if(!fromEntity){
            fromEntity = prevEntity;
            return ProcedureResult(false, "File path incorrect.");
        }else if(fromEntity->getSubEntities().empty()){
            fromEntity = prevEntity;
            return ProcedureResult(false, "File is corrupted.");
        }else{
            QOpenGLTexture* texture = new QOpenGLTexture(QImage(texString));
            fromEntity->getSubEntities()[0].setTexture(texture);
            textures.push_back(texture);
            fromEntity->getSubEntities()[1].setVisible(false);
            fromEntity->getSubEntities()[2].setVisible(false);
            fromEntity->getSubEntities()[3].setVisible(false);
            fromEntity->rotate(30.0f, QVector3D(1,0,0));
            fromEntity->translate(QVector3D(-100, -20, -120));
            fromEntity->BindPose();
            delete prevEntity;
            return ProcedureResult(true, "");
        }

    }else{
        AnimEntity * prevEntity = toEntity;

        toEntity = modelLoader->loadModel(meshString);

        if(!toEntity){
            toEntity = prevEntity;
            return ProcedureResult(false, "File path incorrect.");

        }else if(toEntity->getSubEntities().empty()){
            toEntity = prevEntity;
            return ProcedureResult(false, "File corrupted");
        }else{
            QOpenGLTexture* texture = new QOpenGLTexture(QImage(texString));
            toEntity->getSubEntities()[0].setTexture(texture);
            textures.push_back(texture);
            toEntity->getSubEntities()[1].setVisible(false);
            toEntity->getSubEntities()[2].setVisible(false);
            toEntity->getSubEntities()[3].setVisible(false);
            toEntity->rotate(30.0f, QVector3D(1,0,0));
            toEntity->translate(QVector3D(-15, -20, -120));
            toEntity->BindPose();
            delete prevEntity;
            return ProcedureResult(true, "");
        }
    }
}

void Engine::update(){

    if(toEntity){
        toEntity->update();
    }

    if(fromEntity){
        fromEntity->update();
    }

}

void Engine::cleanup()
{
    std::cout << "engine cleanup" << std::endl;
    indexBuffer.destroy();

    for(QOpenGLTexture* texture: textures){
        delete texture;
    }

    delete m_program;
    m_program = 0;
    delete camera;
    delete modelLoader;
    delete animLoader;
    delete animRetargeter;
    delete toEntity;
    delete fromEntity;
    delete animation;
}

void Engine::initializeGL()
{

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    indexBuffer.create();

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, VertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, FragmentShaderSource);
    m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    m_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->link();
    m_program->bind();
    m_program->setUniformValue("texture", 0);

    m_program->release();
}

/*
 * BufferedDrawTex
 * Send an animated animEntityity to the graphics card to be rendered to the screen
 * @param AnimEntity animEntity - the animEntity to be rendered.
 */
void Engine::bufferedDrawTex(AnimEntity *animEntity){

    for(int i = 0; i < animEntity->getSubEntities().size(); i++ ){
        AnimSubEntity & subEntity = animEntity->getSubEntities()[i];

        if(!subEntity.isVisible()){
            continue;
        }

        //is there data loaded into the graphics card already?
        if (!subEntity.getModel()->isInBuffer()){
            sendDataToBufferTex(subEntity);
        }else if(animEntity->frameChanged()){
            resendVertsToBuffer(subEntity);
        }


        //QMatrix4x4 finalMatrix = projMatrix * camera->getWorldToViewMatrix() * animEntity->getPos() * animEntity->getOrient();

        QMatrix4x4 finalMatrix;

        finalMatrix = finalMatrix * camera->getWorldToViewMatrix();

        //finalMatrix.setToIdentity();

       /* GLfloat kingMat[4][4];

        GLfloat *pSource = (GLfloat*)glm::value_ptr(finalMatrix);

        int j, k;

        for(int i = j = k = 0; i < 16; i++){
            kingMat[k][j] = pSource[i];
            std::cout << kingMat[j][k] << " ";
            if(i != 0 && i % 4 == 0){
                std::cout << "\n";
                k = 0;
                j++;
            }else{
                k++;
            }
        } */


        //QMatrix4x4 basic;
        m_program->setUniformValue("matrix", finalMatrix);

        //texture
        subEntity.getTexture()->bind();

        //enable attrib arrays.
        m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);

        //texture code
        //textureDataVBO.bind();
        //glTexCoordPointer(2, GL_FLOAT, 0, 0);


        //vertex code.
        //vertexDataVBO.bind();
        //glVertexPointer(3, GL_FLOAT,0,0);

        //index code.
        indexBuffer.bind();


        glDrawElements(GL_TRIANGLES, subEntity.getModel()->getIndices().size(), GL_UNSIGNED_INT, 0);


        //disable attrib arrays.
        //m_program->disableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        //m_program->disableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);


    }

    if(animEntity->frameChanged()){
        animEntity->changedOff();
    }

}

/*
 * sendDataToBufferTex
 * Send a animated sub-animEntityity to the graphics card.
 * @param AnimSubEntity animEntity - the animEntity to be rendered.
 */
void Engine::sendDataToBufferTex(AnimSubEntity &animEntity){

    //Vertex Buffer
    //vertexDataVBO.bind();
   // vertexDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
    //vertexDataVBO.allocate(animEntity.getVertices().data(), sizeof(float) * animEntity.getVertices().size());
    m_program->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, animEntity.getVertices().data(), 3, 0);

    //Texture coordinates
    //textureDataVBO.bind();
    //textureDataVBO.setUsagePattern(QGLBuffer::StaticDraw);
    //textureDataVBO.allocate(animEntity.getModel()->getTexture().data(), sizeof(float) * animEntity.getModel()->getTexture().size());
    m_program->setAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE, animEntity.getModel()->getTexture().data(), 2, 0);

    // Indexes
    indexBuffer.bind();
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indexBuffer.allocate(animEntity.getModel()->getIndices().data(), sizeof(GLuint) * animEntity.getModel()->getIndices().size());

}

/*
 * resendVertsToBuffer
 * Send a animated subanimEntityity to the graphics card.
 * @param AnimSubEntity animEntity - the animEntityity to be rendered.
 */
void Engine::resendVertsToBuffer(AnimSubEntity &animEntity){

    m_program->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, animEntity.getVertices().data(), 3, 0);

}


void Engine::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_program->bind();

    //send the two models to render.
    if(fromEntity){
        bufferedDrawTex(fromEntity);
    }

    if(toEntity){
        bufferedDrawTex(toEntity);
    }

    m_program->release();

    std::cout << "Printing Frame!" << std::endl;
}

void Engine::resizeGL(int w, int h)
{
    projMatrix.setToIdentity();
    projMatrix.perspective(45.0f, GLfloat(w) / h, 0.1f, 100.0f);
     //projMatrix = glm::perspective(45.0f, (float)w/h, 0.1f, 1000.0f);
}


void Engine::moveCameraForward(){
    camera->moveForward();
}

void Engine::moveCameraBackward(){
    camera->moveBackward();
}

void Engine::moveCameraLeft(){
    camera->strafeLeft();
}

void Engine::moveCameraRight(){
    camera->strafeRight();
}

void Engine::moveCameraUpward(){
    camera->moveUp();
}

void Engine::moveCameraDownward(){
    camera->moveDown();
}

void Engine::pointCameraUpward(){
    camera->lookUp();
}

void Engine::pointCameraDownward(){
    camera->lookDown();
}

void Engine::pointCameraLeft(){
    camera->lookLeft();
}

void Engine::pointCameraRight(){
    camera->lookRight();
}
