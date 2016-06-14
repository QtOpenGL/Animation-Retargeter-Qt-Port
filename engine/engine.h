#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <QOpenGLFunctions>
#include <OpenGL/glu.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLShaderProgram>
#include <math.h>
#include "logo.h"
#include "utils/procedureresult.h"
#include "engine/camera.h"
#include "engine/md5/modelloader.h"
#include "engine/md5/animloader.h"
#include "engine/md5/animretargeter.h"
#include "engine/entity.h"
#include "engine/model.h"
#include "engine/glslprogram.h"

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

class Engine : public QObject
{
    Q_OBJECT
public:
    Engine(QObject * parent = 0);

    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraLeft();
    void moveCameraRight();
    void moveCameraUpward();
    void moveCameraDownward();
    void pointCameraUpward();
    void pointCameraDownward();
    void pointCameraLeft();
    void pointCameraRight();

    ProcedureResult LoadModel(QString meshString, QString texString, EntityType type);
    ProcedureResult LoadAnimation(QString animString);
    ProcedureResult retargetAnimation(map<int, int> boneMap, vector<int> roots);
    QVector<Skeleton> getSkeletonsForWindow();

    void bufferedDrawTex(AnimEntity *animEntity);
    void resendVertsToBuffer(AnimSubEntity &animEntity);
    void sendDataToBufferTex(AnimSubEntity &animEntity);

    void cleanup();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void update();

private:
    GLuint storeTexture(QImage imageData);
    void deleteTextures();


public slots:
    void onPlayFrom();
    void onPlayTo();
    void onBindFrom();
    void onBindTo();
    void onHideFrom();
    void onHideTo();

signals:

private:

    GLSLProgram *shaderProgram;
    QOpenGLBuffer indexBuffer;
    mat4 projMatrix;
    mat4 finalMatrix;

    Camera * camera;
    ModelLoader * modelLoader;
    AnimLoader * animLoader;
    AnimRetargeter * animRetargeter;
    AnimEntity * toEntity;
    AnimEntity * fromEntity;
    Animation * animation;
    QVector<QOpenGLTexture *> textures;
    vector<GLuint> TexIDs;

    string vertexShaderSource =
        "#version 120\n"
        "uniform mat4 projection_matrix;\n"
        "attribute vec3 a_Vertex;\n"
        "attribute vec2 s_vTexCoord;\n"
        "varying vec2 texCoord;\n"
        "void main(void)\n"
        "{\n"
        "    vec4 pos = vec4(a_Vertex, 1.0);\n"
        "    gl_Position = projection_matrix * pos;\n"
        "    texCoord = s_vTexCoord;\n"
        "}\n";

    string fragmentShaderSource =
        "#version 120\n"
        "uniform sampler2D texture;\n"
        "varying vec2 texCoord;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texCoord);\n"
        "}\n";

};

#endif // ENGINE_H
