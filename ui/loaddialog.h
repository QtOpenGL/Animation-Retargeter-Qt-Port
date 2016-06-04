#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include "engine/engine.h"

const static QString NO_TEXTURE_PATH = "/textures/noTexture.tga";


class LoadDialog : public QDialog
{

    Q_OBJECT

public:
    LoadDialog(QWidget *parent, Engine * model, EntityType type);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;


private slots:
    void onOpenTexture();
    void onOpenMesh();
    void onOpenAnim();
    void onDone();

private:
    void createModelDialog(QString title, QGridLayout * mainLayout);
    void createAnimationDialog(QGridLayout * mainLayout);

private:
    EntityType type;
    QLineEdit * AnimLine;
    QLineEdit * MeshLine;
    QLineEdit * TexLine;
    Engine * engine;
    QPushButton *doneButton;


};

#endif // LOADDIALOG_H
