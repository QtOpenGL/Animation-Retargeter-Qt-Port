/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <iostream>

#include "QtWidgets"
#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

Window::Window(Engine * model): gridLayout(NULL)
{
    engine = model;
    glWidget = new GLWidget(this, engine);
    glWidget->setMinimumWidth(600);
    glWidget->setMinimumHeight(480);
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //scroll->setMaximumWidth(240);
    scroll->setMinimumWidth(300);

    viewport = new QWidget(this);
    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    resetRetargetWidgets();

    QPushButton * applyButton = new QPushButton("&Apply", this);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(retargetAnimation()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    mainLayout->addWidget(scroll);
    mainLayout->addWidget(applyButton);

    container->addLayout(mainLayout);
    container->addWidget(glWidget);

    setAttribute(Qt::WA_DeleteOnClose, true);

    setLayout(container);

    setWindowTitle(tr("Animation Retargeter 3.0"));

}

Window::~Window(){
    deleteRetargetWidgets();

}

void Window::deleteRetargetWidgets(){
    for(QComboBox *c : comboBoxes){
        delete c;
    }

    for(QLabel *l : labels){
        delete l;
    }

    for(QCheckBox *x : checkBoxes){
        delete x;
    }
}

void Window::resetRetargetWidgets(){

    deleteRetargetWidgets();
    comboBoxes.clear();
    labels.clear();
    checkBoxes.clear();
    delete gridLayout;
    gridLayout = new QGridLayout(viewport); //set viewport as parent.

    QVector<Skeleton> skeletonList = engine->getSkeletonsForWindow();

    if(!skeletonList.isEmpty()){

        QLabel * fromBoneLabel = new QLabel(QString("From Bone"));
        QLabel * toBoneLabel = new QLabel(QString("To Bone"));
        QLabel * rootLabel = new QLabel(QString("Root"));

        Skeleton toSkel = skeletonList[0];
        Skeleton fromSkel = skeletonList[1];

        QVector<QString> toBones;
        QStringList fromBonesList;
        QVector<QString> fromBones;

        for(int i = 0; i < toSkel.size(); i++){
            QString name = QString::fromStdString(toSkel[i].getName());
            toBones.push_back(name);
        }

        fromBones.push_back("None");
        fromBonesList << "None";

        for(int i = 0; i < fromSkel.size(); i++){
            QString name = QString::fromStdString(fromSkel[i].getName());
            fromBonesList << name;
            fromBones.push_back(name);
        }

        gridLayout->addWidget(toBoneLabel, 0, 0);
        gridLayout->addWidget(fromBoneLabel, 0, 1);
        gridLayout->addWidget(rootLabel, 0, 2);

        for(int i = 0; i < toBones.size(); i++){
            QComboBox * cbox = new QComboBox;
            cbox->addItems(fromBonesList);
            comboBoxes.push_back(cbox);

            //Set bones with similar names together.
            for(int j = 0; j < fromBones.size(); j++){

                QString to = toBones[i].toLower();
                QString from = fromBones[j].toLower();

                if(to == from){
                    cbox->setCurrentIndex(j);
                }

            }

            QLabel * label = new QLabel(toBones[i]);
            label->setBuddy(cbox);
            labels.push_back(label);

            QCheckBox * chkBox = new QCheckBox;
            checkBoxes.push_back(chkBox);

            gridLayout->addWidget(label, i + 1, 0);
            gridLayout->addWidget(cbox, i + 1, 1);
            gridLayout->addWidget(chkBox, i + 1, 2);
        }

    }


/*
    size = 300;
    BoxItems << "square" << "circle" << "triangle";

    for(int i = 1; i < size; i++){
        QComboBox * cbox = new QComboBox;
        cbox->addItems(BoxItems);
        comboBoxes.push_back(cbox);

        QLabel * label = new QLabel(QString("Label %1:").arg(i+1));
        label->setBuddy(cbox);
        labels.push_back(label);

        QCheckBox * chkBox = new QCheckBox;
        checkBoxes.push_back(chkBox);

        gridLayout->addWidget(label, i, 0);
        gridLayout->addWidget(cbox, i, 1);
        gridLayout->addWidget(chkBox, i, 2);
    }*/

    viewport->setLayout(gridLayout);
}

void Window::retargetAnimation(){

    map<int, int> boneMap;
    vector<int> roots;

    for(int i = 0; i < checkBoxes.size(); i++){
        if(checkBoxes[i]->isChecked())
            roots.push_back(i);
    }

    for(int i = 0; i < labels.size(); i++){
        std::cout << labels[i]->text().toStdString() << ": " << comboBoxes[i]->currentText().toStdString() << std::endl;

        if(comboBoxes[i]->currentIndex() > 0){
            boneMap.insert(std::pair<int, int>(i, comboBoxes[i]->currentIndex()-1));

            std::cout << i << " : " << comboBoxes[i]->currentIndex()-1 << std::endl;
        }
    }

    std::cout << "retarget pressed" << std::endl;

    ProcedureResult result = engine->retargetAnimation(boneMap, roots);

    if(!result.getResult()){
        QMessageBox::warning(this, tr("Problem retargeting the animation: "), result.getMessage(), QMessageBox::Ok);
    }

}

GLWidget * Window::getGLWidget(){
    return glWidget;
}


