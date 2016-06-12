#ifndef ENGINE_H
#define ENGINE_H

#include <map>
#include <QOpenGLFunctions>
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

public slots:
    void onPlayFrom();
    void onPlayTo();
    void onBindFrom();
    void onBindTo();
    void onHideFrom();
    void onHideTo();

signals:

private:

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer indexBuffer;
    QMatrix4x4 projMatrix;

    Camera * camera;
    ModelLoader * modelLoader;
    AnimLoader * animLoader;
    AnimRetargeter * animRetargeter;
    AnimEntity * toEntity;
    AnimEntity * fromEntity;
    Animation * animation;
    QVector<QOpenGLTexture *> textures;

    const char *VertexShaderSource =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";

    const char *FragmentShaderSource =
        "uniform sampler2D texture;\n"
        "varying mediump vec4 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = texture2D(texture, texc.st);\n"
        "}\n";

};

#endif // ENGINE_H
