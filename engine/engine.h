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
    QVector<Skeleton> getSkeletonsForWindow();

    void cleanup();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void update();
    GLuint storeTexture(QString);

public slots:
    void onPlayFrom();
    void onPlayTo();
    void onBindFrom();
    void onBindTo();
    void onHideFrom();
    void onHideTo();

private slots:
    void setupVertexAttribs();

signals:

private:
    QPoint m_lastPos;
    Logo m_logo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    bool m_core;
    bool m_transparent;
    int m_xRot;
    int m_yRot;
    int m_zRot;

    Camera * camera;
    ModelLoader * modelLoader;
    AnimLoader * animLoader;
    AnimRetargeter * animRetargeter;
    AnimEntity * toEntity;
    AnimEntity * fromEntity;
    Animation * animation;
    std::map<int, int> * userMap;

};

#endif // ENGINE_H
