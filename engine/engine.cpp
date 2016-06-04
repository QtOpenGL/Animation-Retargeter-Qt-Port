#include "engine.h"
#include <iostream>
#include <QString>


Engine::Engine(QObject * parent): QObject(parent), toEntity(NULL), fromEntity(NULL), animation(NULL)
{
     camera = new Camera(vec3(0, 0, 10));
     modelLoader = new ModelLoader();
     animLoader = new AnimLoader();
     animRetargeter = new AnimRetargeter();
     userMap = new map<int, int>;
}

void Engine::onPlayFrom(){ std::cout << "onPlayFrom()" << std::endl; }
void Engine::onPlayTo(){ std::cout << "onPlayTo()" << std::endl; }
void Engine::onBindFrom(){ std::cout << "onBindFrom()" << std::endl; }
void Engine::onBindTo(){ std::cout << "onBindTo()" << std::endl; }
void Engine::onHideFrom(){ std::cout << "onHideFrom()" << std::endl; }
void Engine::onHideTo(){ std::cout << "onHideTo()" << std::endl; }


QVector<Skeleton> Engine::getSkeletonsForWindow(){

    QVector<Skeleton> skeletons;

    //in order to return skeletons we need: a toEntity, a fromEntity and a compatible animation.
    if(toEntity && animation && fromEntity && fromEntity->setAnim(animation)){
        skeletons.push_back(toEntity->getBindPose());
        skeletons.push_back(animation->getSkeletons()[0]);
    }

    return skeletons;
}

ProcedureResult Engine::LoadAnimation(QString animString){
    Animation * prevAnim = animation;

    animation = animLoader->loadAnim(animString);

    if(!animation){
        animation = prevAnim;
        return ProcedureResult(false, "File corrupted");
    }else{

        if(!fromEntity->setAnim(animation)){
            animation = prevAnim;
            return ProcedureResult(false, "The chosen animation is incompatible with the current 'From Model' please select a compatible animation or different 'From Model'");
        }else{
            delete prevAnim;
            return ProcedureResult(true, "");
        }

    }

}


GLuint Engine::storeTexture(QString filepath){

    /*
    TargaImage modelTexture;

    if (!modelTexture.load(filepath))
    {
        std::cerr << "Could not load the model texture" << std::endl; //could be replace with an exception.
        return false;
    }

    GLuint textureBuffer;
    glGenTextures(1, &textureBuffer);
    glActiveTexture(GL_TEXTURE0); //GL_TEXTURE0 or TEXTURE1 e.c.t defined by how the class handles this
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, modelTexture.getWidth(),
                 modelTexture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 modelTexture.getImageData());

   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, modelTexture.getWidth(),
                      modelTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      modelTexture.getImageData());



    TexIDs.push_back(textureBuffer);


    return textureBuffer;
    */

    //GLuint num = 0;

    return 0;
}

ProcedureResult Engine::LoadModel(QString meshString, QString texString, EntityType type){

    if(type == FROM){
        AnimEntity * prevEntity = fromEntity;

        fromEntity = modelLoader->loadModel(meshString);

        if(!fromEntity){
            fromEntity = prevEntity;
            return ProcedureResult(false, "File corrupted.");
        }else{
            fromEntity->getSubEntities()[0].setTexBID(storeTexture(texString));
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
            return ProcedureResult(false, "File corrupted.");
        }else{
            toEntity->getSubEntities()[0].setTexBID(storeTexture(texString));
            toEntity->getSubEntities()[1].setVisible(false);
            toEntity->getSubEntities()[2].setVisible(false);
            toEntity->getSubEntities()[3].setVisible(false);
            toEntity->rotate(30.0f, vec3(1,0,0));
            toEntity->translate(vec3(-100, -20, -120));
            toEntity->BindPose();
            delete prevEntity;
            return ProcedureResult(true, "");
        }
    }
}

void Engine::update(){
    std::cout << "engine::update()" << std::endl;
}

void Engine::cleanup()
{
    std::cout << "engine cleanup" << std::endl;
    m_logoVbo.destroy();
    delete m_program;
    m_program = 0;

    delete camera;
    delete modelLoader;
    delete animLoader;
    delete animRetargeter;
    delete toEntity;
    delete fromEntity;
    delete animation;
    delete userMap;
}


static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
    "}\n";

void Engine::initializeGL()
{

    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -1);

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
}

void Engine::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
}

void Engine::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

    m_program->release();

    std::cout << "Printing Frame!" << std::endl;
}

void Engine::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}


void Engine::moveCameraForward(){}
void Engine::moveCameraBackward(){}
void Engine::moveCameraLeft(){}
void Engine::moveCameraRight(){}
void Engine::moveCameraUpward(){}
void Engine::moveCameraDownward(){}
void Engine::pointCameraUpward(){}
void Engine::pointCameraDownward(){}
void Engine::pointCameraLeft(){}
void Engine::pointCameraRight(){}
