#include "engine.h"
#include <iostream>
#include <QString>


Engine::Engine(QObject * parent): QObject(parent), indexBuffer(QOpenGLBuffer::IndexBuffer), toEntity(NULL), fromEntity(NULL), animation(NULL)
{
     camera = new Camera(vec3(0, 0, 10));
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
            //QOpenGLTexture* texture = new QOpenGLTexture(QImage(texString));
            //fromEntity->getSubEntities()[0].setTexture(texture);
            //textures.push_back(texture);
            //toEntity->getSubEntities()[0].setTexBID(storeTexture(QImage(texString)));
            fromEntity->getSubEntities()[1].setVisible(false);
            fromEntity->getSubEntities()[2].setVisible(false);
            fromEntity->getSubEntities()[3].setVisible(false);
            fromEntity->rotate(30.0f, vec3(1,0,0));
            fromEntity->translate(vec3(-100, -20, -120));
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
            //QOpenGLTexture* texture = new QOpenGLTexture(QImage(texString));
            //toEntity->getSubEntities()[0].setTexture(texture);
            //textures.push_back(texture);
            toEntity->getSubEntities()[0].setTexBID(storeTexture(QImage(texString)));
            toEntity->getSubEntities()[1].setVisible(false);
            toEntity->getSubEntities()[2].setVisible(false);
            toEntity->getSubEntities()[3].setVisible(false);
            toEntity->rotate(30.0f, vec3(1,0,0));
            toEntity->translate(vec3(-15, -20, -120));
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

    deleteTextures();

    delete shaderProgram;
    shaderProgram = 0;
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_TEXTURE_2D);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    shaderProgram = new GLSLProgram();

    if(!shaderProgram->initialize(vertexShaderSource, fragmentShaderSource)){
        std::cerr << "Could not initialise shader program" << std::endl;
    }

    //bind the attributes
    shaderProgram->bindAttrib(0, "a_Vertex");
    shaderProgram->bindAttrib(1, "s_vTexCoord");

    shaderProgram->linkProgram(); //relink program
    shaderProgram->bindShader(); //enable the shader
}

/*
 * BufferedDrawTex
 * Send an animated animEntityity to the graphics card to be rendered to the screen
 * @param AnimEntity animEntity - the animEntity to be rendered.
 */
void Engine::bufferedDrawTex(AnimEntity *animEntity){
    for(int i = 0; i < animEntity->getSubEntities().size(); i++ ){
        AnimSubEntity & subEntity = animEntity->getSubEntities()[i];

        if(!subEntity.isVisible())
            continue;

        //is there data loaded into the graphics card already?
        if (!subEntity.getModel()->isInBuffer())
            sendDataToBufferTex(subEntity);

        else if(animEntity->frameChanged())
            resendVertsToBuffer(subEntity);


        finalMatrix = projMatrix * camera->getWorldToViewMatrix() * animEntity->getPos() * animEntity->getOrient();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, subEntity.getTexBID());
        //subEntity.getTexture()->bind();

        shaderProgram->sendUniform4x4("projection_matrix", &finalMatrix[0][0], GL_FALSE);
        shaderProgram->sendUniform("texture", 0.0f);

        //enable attrib arrays.
        glEnableVertexAttribArray(0); //Enable the vertex attribute
        glEnableVertexAttribArray(1); //Enable the texture coordinate attribute

        //texture code
        glBindBuffer(GL_ARRAY_BUFFER, subEntity.getModel()->getTB());
        glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);


        //vertex code.
        glBindBuffer(GL_ARRAY_BUFFER, subEntity.getVB());
        glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //index code.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subEntity.getModel()->getIB());


        glDrawElements (GL_TRIANGLES, subEntity.getModel()->getIndices().size(), GL_UNSIGNED_INT, 0);

        //disable attrib arrays.
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


    }

    if(animEntity->frameChanged()){
        animEntity->changedOff();
    }

}

GLuint Engine::storeTexture(QImage imageData){

    GLuint textureBuffer;
    glGenTextures(1, &textureBuffer);
    glActiveTexture(GL_TEXTURE0); //GL_TEXTURE0 or TEXTURE1 e.c.t defined by how the class handles this
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, imageData.width(),
                 imageData.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imageData.bits());

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, imageData.width(),
                      imageData.height(), GL_RGB, GL_UNSIGNED_BYTE,
                      imageData.bits());


    TexIDs.push_back(textureBuffer);

    return textureBuffer;
}

/*
 * deleteTextures
 * free the texture memory stored on the graphics card
 */
void Engine::deleteTextures(){
    glDeleteTextures((GLsizei)TexIDs.size(), TexIDs.data());
}

/*
 * sendDataToBufferTex
 * Send a animated sub-animEntityity to the graphics card.
 * @param AnimSubEntity animEntity - the animEntity to be rendered.
 */
void Engine::sendDataToBufferTex(AnimSubEntity &subEntity){

    GLuint VB, IB, TB;

    glGenBuffers(1, &VB);
    glGenBuffers(1, &IB);
    glGenBuffers(1, &TB);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * subEntity.getVertices().size(), subEntity.getVertices().data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * subEntity.getModel()->getIndices().size(), subEntity.getModel()->getIndices().data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, TB); //Bind the texture buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * subEntity.getModel()->getTexture().size(), subEntity.getModel()->getTexture().data(), GL_STATIC_DRAW); //Send the data

    subEntity.getModel()->setBufferIDs(IB, TB); //want this is be a joined operation, genbuffer then set model flags
    subEntity.setBufferIDs(VB);
}

/*
 * resendVertsToBuffer
 * Send a animated subanimEntityity to the graphics card.
 * @param AnimSubEntity animEntity - the animEntityity to be rendered.
 */
void Engine::resendVertsToBuffer(AnimSubEntity &subEntity){

    glBindBuffer(GL_ARRAY_BUFFER, subEntity.getVB());
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * subEntity.getVertices().size(), subEntity.getVertices().data(), GL_STATIC_DRAW);

}


void Engine::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //send the two models to render.
    if(fromEntity){
        bufferedDrawTex(fromEntity);
    }

    if(toEntity){
        bufferedDrawTex(toEntity);
    }

    std::cout << "Printing Frame!" << std::endl;
}

void Engine::resizeGL(int w, int h)
{
     glViewport(0, 0, w, h);
     projMatrix = glm::perspective(45.0f, (float)w/h, 0.1f, 1000.0f);
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
